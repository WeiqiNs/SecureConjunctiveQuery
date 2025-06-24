#include "ipe_et.hpp"

IpeEtPp IpeEt::pp_gen(const int length, const bool pre){
    // Create the pp instance.
    IpeEtPp pp{};
    // Update the input length according to input.
    pp.l = length;
    // Save the created pairing group.
    pp.pairing_group = std::make_unique<BP>(pre);

    return pp;
}

IpeEtMsk IpeEt::msk_gen(const IpeEtPp& pp){
    // Create the msk instance.
    IpeEtMsk msk{};

    // Compute the matrix size, length + 1 to hold
    const int mat_size = pp.l + 3;
    // Generate the random matrix of desired size.
    msk.b = pp.pairing_group->Zp->rand_mat(mat_size, mat_size);
    // Find the inverse and the determinant.
    Fp det;
    msk.bi = pp.pairing_group->Zp->mat_inv_with_det(msk.b, det);
    // Find its transpose multiplied with determinant.
    msk.bi = Field::mat_transpose(pp.pairing_group->Zp->mat_mul(msk.bi, det));

    return msk;
}

G1Vec IpeEt::enc(const IpeEtPp& pp, const IpeEtMsk& msk, const IntVec& x){
    // First convert x to field elements.
    auto x_vec = pp.pairing_group->Zp->from_int(x);

    // Add a trailing 1 to the vector.
    x_vec.push_back(pp.pairing_group->Zp->from_int(-1));

    // Sample the random point alpha.
    const auto alpha = pp.pairing_group->Zp->rand();

    // We compute a * x.
    auto ax = pp.pairing_group->Zp->vec_mul(x_vec, alpha);
    // Attach (r, 0).
    ax.push_back(pp.pairing_group->Zp->rand());
    ax.emplace_back(0);

    // Multiply with the matrix Bi.
    const auto axb = pp.pairing_group->Zp->mat_mul(ax, msk.b);

    // Raise the vector to g1 and return.
    return pp.pairing_group->Gp->g1_raise(axb);
}

G2Vec IpeEt::keygen(const IpeEtPp& pp, const IpeEtMsk& msk, const IntVec& y, const int p){
    // First convert y to field elements.
    auto y_vec = pp.pairing_group->Zp->from_int(y);

    // Add p to the vector.
    y_vec.push_back(pp.pairing_group->Zp->from_int(p));

    // Attach (0, r).
    y_vec.emplace_back(0);
    y_vec.push_back(pp.pairing_group->Zp->rand());

    // Compute y * bi.
    const auto ybi = pp.pairing_group->Zp->mat_mul(y_vec, msk.bi);

    // Raise the vector to g2 and return.
    return pp.pairing_group->Gp->g2_raise(ybi);
}

bool IpeEt::dec(const G1Vec& ct, const G2Vec& sk){ return gt_is_unity(Group::pair(ct, sk).value); }
