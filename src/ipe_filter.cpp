#include "ipe_filter.hpp"

IpeFilterPP IpeFilter::pp_gen(const int degree, const int length, const bool pre){
    // Create the pp instance.
    IpeFilterPP pp;
    // Update the degree according to input.
    pp.d = degree;
    // Update the input length according to input.
    pp.l = length;
    // Save the created pairing group.
    pp.pairing_group = std::make_unique<BP>(pre);

    return pp;
}

IpeFilterMsk IpeFilter::msk_gen(const IpeFilterPP& pp, const CharVec& key){
    // Create the msk instance.
    IpeFilterMsk msk;
    // Get the unique point for HMAC.
    msk.hmac = std::make_unique<HMAC>(key);
    // Compute the matrix size.
    const int mat_size = pp.l * pp.d + 3;
    // Generate the random matrix of desired size.
    msk.b = pp.pairing_group->Zp->rand_mat(mat_size, mat_size);
    // Find the inverse and the determinant.
    Fp det;
    msk.bi = pp.pairing_group->Zp->mat_inv_with_det(msk.b, det);
    // Find its transpose multiplied with determinant.
    msk.bi = Field::mat_transpose(pp.pairing_group->Zp->mat_mul(msk.bi, det));

    return msk;
}

G1Vec IpeFilter::enc(const IpeFilterPP& pp, const IpeFilterMsk& msk, const Vec& x){
    // Generate hash of the input x vector.
    const auto x_digest = msk.hmac->digest_vec_to_fp_mod(*pp.pairing_group, x);

    // We compute polynomial of x.
    auto poly_x = Helper::power_poly(pp.d, *pp.pairing_group, x_digest);

    // Attach (r, 0).
    poly_x.push_back(pp.pairing_group->Zp->rand());
    poly_x.emplace_back(0);

    // Multiply with the matrix B.
    const auto xb = pp.pairing_group->Zp->mat_mul(poly_x, msk.b);

    // Raise to g1 and return.
    return pp.pairing_group->Gp->g1_raise(xb);
}

G2Vec IpeFilter::keygen(const IpeFilterPP& pp, const IpeFilterMsk& msk, const Mat& y){
    // Generate hash of the input y matrix.
    const auto y_digest = msk.hmac->digest_mat_to_fp_mod(*pp.pairing_group, y);

    // We compute the polynomial of y.
    auto poly_y = Helper::coeff_poly(pp.d, *pp.pairing_group, y_digest);

    // Attach (0, r).
    poly_y.emplace_back(0);
    poly_y.push_back(pp.pairing_group->Zp->rand());

    // Multiply with the matrix Bi.
    const auto ybi = pp.pairing_group->Zp->mat_mul(poly_y, msk.bi);

    // Raise to g2 and return.
    return pp.pairing_group->Gp->g2_raise(ybi);
}

bool IpeFilter::dec(const G1Vec& ct, const G2Vec& sk){ return Group::check_gt_unity(Group::pair(ct, sk)); }
