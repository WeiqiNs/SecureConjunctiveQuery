#include "ipe_join.hpp"

IpeJoinPP IpeJoin::pp_gen(const int degree, const int length, const bool pre){
    // Create the pp instance.
    IpeJoinPP pp;
    // Update the degree according to input.
    pp.d = degree;
    // Update the input length according to input.
    pp.l = length;
    // Save the created pairing group.
    pp.pairing_group = std::make_unique<BP>(pre);

    return pp;
}

IpeJoinMsk IpeJoin::msk_gen(const IpeJoinPP& pp){
    // Create the msk instance.
    IpeJoinMsk msk;

    // Sample a random point k.
    msk.k = pp.pairing_group->Zp->rand();

    // Compute the matrix size.
    const int mat_size = (pp.l - 1) * pp.d + 4;
    // Generate the random matrix of desired size.
    msk.b = pp.pairing_group->Zp->rand_mat(mat_size, mat_size);
    // Find the inverse and the determinant.
    Fp det;
    msk.bi = pp.pairing_group->Zp->mat_inv_with_det(msk.b, det);
    // Find its transpose multiplied with determinant.
    msk.bi = Field::mat_transpose(pp.pairing_group->Zp->mat_mul(msk.bi, det));

    return msk;
}

G1Vec IpeJoin::enc(const IpeJoinPP& pp, const IpeJoinMsk& msk, const Vec& x, const int join_index){
    // Generate hash of the input x vector.
    auto x_digest = msk.hash.digest_vec_to_fp(*pp.pairing_group, x);

    // Get the value at the join position and remove it.
    const auto join_on = x_digest[join_index];
    x_digest.erase(x_digest.begin() + join_index);

    // Sample the random point alpha.
    const auto r = pp.pairing_group->Zp->rand();

    // We compute polynomial of x.
    const auto poly_x = Helper::power_poly(pp.d, *pp.pairing_group, x_digest);
    // We compute a * x.
    auto ax = pp.pairing_group->Zp->vec_mul(poly_x, r);
    // Attach (r, 0).
    ax.push_back(pp.pairing_group->Zp->rand());
    ax.emplace_back(0);

    // Insert the join on.
    ax.push_back(join_on);

    // Multiply with the matrix Bi.
    const auto axb = pp.pairing_group->Zp->mat_mul(ax, msk.b);

    // Raise to g1 and return.
    return pp.pairing_group->Gp->g1_raise(axb);
}

G2Vec IpeJoin::keygen(const IpeJoinPP& pp, const IpeJoinMsk& msk, const Mat& y){
    // Generate hash of the input y matrix.
    const auto y_digest = msk.hash.digest_mat_to_fp(*pp.pairing_group, y);

    // We compute the polynomial of y.
    auto poly_y = Helper::coeff_poly(pp.d, *pp.pairing_group, y_digest);
    // Attach (0, r).
    poly_y.emplace_back(0);
    poly_y.push_back(pp.pairing_group->Zp->rand());
    // Push the randomness in the key to the back.
    poly_y.push_back(msk.k);

    // Multiply with the matrix B.
    const auto yb = pp.pairing_group->Zp->mat_mul(poly_y, msk.bi);

    // Raise to g2 and return.
    return pp.pairing_group->Gp->g2_raise(yb);
}

Gt IpeJoin::dec(const G1Vec& ct, const G2Vec& sk){ return Group::pair(ct, sk); }
