#include "ipe_aggre.hpp"

IpeAggrePP IpeAggre::pp_gen(const int length, bool pre){
    // Create the pp instance.
    IpeAggrePP pp;
    // Update the input length according to input.
    pp.l = length;
    // Save the created pairing group.
    pp.pairing_group = std::make_unique<BP>(pre);

    return pp;
}

IpeAggreMsk IpeAggre::msk_gen(const IpeAggrePP& pp){
    // Create the msk instance.
    IpeAggreMsk msk;

    // Compute the matrix size, length + 1 to hold
    const int mat_size = pp.l + 1;
    // Generate the random matrix of desired size.
    msk.b = pp.pairing_group->Zp->rand_mat(mat_size, mat_size);
    // Find the inverse and the determinant.
    Fp det;
    msk.bi = pp.pairing_group->Zp->mat_inv_with_det(msk.b, det);
    // Find its transpose multiplied with determinant.
    msk.bi = Field::mat_transpose(pp.pairing_group->Zp->mat_mul(msk.bi, det));

    return msk;
}

G1Vec IpeAggre::enc(const IpeAggrePP& pp, const IpeAggreMsk& msk, const IntVec& x){
    // First convert x to field elements.
    auto x_vec = pp.pairing_group->Zp->from_int(x);

    // Add a trailing 1 to the vector.
    x_vec.push_back(pp.pairing_group->Zp->from_int(-1));

    // Sample the random point alpha.
    const Fp alpha = pp.pairing_group->Zp->rand();

    // Compute xb.
    const auto xb = pp.pairing_group->Zp->mat_mul(x_vec, msk.b);
    // Compute axb.
    const auto axb = pp.pairing_group->Zp->vec_mul(xb, alpha);

    // Raise the vector to g1 and return.
    return pp.pairing_group->Gp->g1_raise(axb);
}

G2Vec IpeAggre::keygen(const IpeAggrePP& pp, const IpeAggreMsk& msk, const IntVec& y, const int p){
    // First convert y to field elements.
    auto y_vec = pp.pairing_group->Zp->from_int(y);

    // Add p to the vector.
    y_vec.push_back(pp.pairing_group->Zp->from_int(p));

    // Sample the random point alpha.
    const Fp beta = pp.pairing_group->Zp->rand();

    // Compute b' * y.
    const auto biy = pp.pairing_group->Zp->mat_mul(y_vec, msk.bi);
    // Compute beta * b' * y.
    const auto bbiy = pp.pairing_group->Zp->vec_mul(biy, beta);

    // Raise the vector to g2 and return.
    return pp.pairing_group->Gp->g2_raise(bbiy);
}

bool IpeAggre::dec(const G1Vec& ct, const G2Vec& sk){ return gt_is_unity(Group::pair(ct, sk).value); }
