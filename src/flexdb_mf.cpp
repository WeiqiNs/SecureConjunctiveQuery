#include "flexdb_mf.hpp"

FlexDbMfPp FlexDbMf::pp_gen(const int degree, const int length, const bool pre){
    // Create the pp instance.
    FlexDbMfPp pp{};
    // Update the degree according to input.
    pp.d = degree;
    // Update the input length according to input.
    pp.l = length;
    // Save the created pairing group.
    pp.pairing_group = std::make_unique<BP>(pre);

    return pp;
}

FlexDbMfMsk FlexDbMf::msk_gen(const FlexDbMfPp& pp, const CharVec& key, const bool& compress){
    // Create the msk instance.
    FlexDbMfMsk msk{};
    // Save whether the values need to be compressed.
    msk.compress = compress;
    // Get the unique point for HMAC.
    msk.hmac = std::make_unique<HMAC>(key);

    // Only when degree is larger than 1, the vectors are needed.
    if (compress){
        // Sample the starting point.
        msk.d_int = Helper::rand_int();
        msk.r_int = Helper::rand_int();
        msk.b_int = Helper::rand_int();
    }
    else{
        // Sample a random point and find its inverse.
        msk.d = pp.pairing_group->Zp->rand();
        msk.di = pp.pairing_group->Zp->inv(msk.d);

        // The length of each random vector should be 2 * (length * degree + 1).
        msk.r = pp.pairing_group->Zp->rand_vec(2 * (pp.l * pp.d + 1));
        msk.b = pp.pairing_group->Zp->rand_vec(2 * (pp.l * pp.d + 1));
        msk.bi = pp.pairing_group->Zp->vec_inv(msk.b);
    }

    // Return the master secret key.
    return msk;
}

G1Vec FlexDbMf::enc(const FlexDbMfPp& pp, const FlexDbMfMsk& msk, const Vec& x){
    // Convert the input vector to Fp.
    const auto fp_x = Helper::vec_to_fp(*pp.pairing_group, x);

    // Sample the random point alpha.
    const auto alpha = pp.pairing_group->Zp->rand();

    // Create pointers for values that needs to be used.
    const Fp* d;
    const FpVec *r, *b;
    Fp temp_d;
    FpVec temp_r, temp_b;

    // In this case, first figure out whether the msk values needs to be sampled.
    if (msk.compress){
        // Only one value is generated.
        temp_d = msk.hmac->digest_int_to_fp_vec_mod(*pp.pairing_group, msk.d_int, 1)[0];
        // Sample r and b.
        temp_r = msk.hmac->digest_int_to_fp_vec_mod(*pp.pairing_group, msk.r_int, 2 * (pp.l * pp.d + 1));
        temp_b = msk.hmac->digest_int_to_fp_vec_mod(*pp.pairing_group, msk.b_int, 2 * (pp.l * pp.d + 1));
        // Assign the pointers with generated values.
        d = &temp_d;
        r = &temp_r;
        b = &temp_b;
    }
    else{
        // Assign the pointers with values from msk.
        d = &msk.d;
        r = &msk.r;
        b = &msk.b;
    }

    // Here is the case where the degree is higher than 1, we compute the value for evaluating polynomial at x.
    const auto poly_x = Helper::power_poly(pp.d, *pp.pairing_group, fp_x);
    // Duplicate the poly(x).
    const auto xx = Field::vec_join(poly_x, poly_x);
    // Compute poly(x)||poly(x) + r.
    const auto xxr = pp.pairing_group->Zp->vec_add(xx, *r);
    // Compute alpha * (poly(x)||poly(x) + r).
    const auto axxr = pp.pairing_group->Zp->vec_mul(xxr, alpha);
    // Compute alpha * vec_b * (poly(x)||poly(x) + r).
    auto abxxr = pp.pairing_group->Zp->vec_mul(axxr, *b);
    // Attach the last point -alpha * delta.
    abxxr.push_back(pp.pairing_group->Zp->neg(pp.pairing_group->Zp->mul(alpha, *d)));

    // Raise the vector to g1 and return.
    return pp.pairing_group->Gp->g1_raise(abxxr);
}

G2Vec FlexDbMf::keygen(const FlexDbMfPp& pp, const FlexDbMfMsk& msk, const Mat& y, const IntVec& sel){
    // Convert the input matrix to Fp.
    const auto fp_y = Helper::mat_to_fp(*pp.pairing_group, y);

    // Sample the random point beta.
    const auto beta = pp.pairing_group->Zp->rand();

    // Create pointers for values that needs to be used and static variables to hold computed values.
    const Fp* di;
    const FpVec *r, *bi;
    Fp temp_di;
    FpVec temp_r, temp_b, temp_bi;

    // In this case, first figure out whether the msk values needs to be sampled.
    if (msk.compress){
        // Only one value is generated.
        temp_di = pp.pairing_group->Zp->inv(
            msk.hmac->digest_int_to_fp_vec_mod(*pp.pairing_group, msk.d_int, 1)[0]
        );
        // Sample r and b.
        temp_r = msk.hmac->digest_int_to_fp_vec_mod(*pp.pairing_group, msk.r_int, 2 * (pp.l * pp.d + 1));
        // Compute the bi.
        temp_bi = pp.pairing_group->Zp->vec_inv(
            msk.hmac->digest_int_to_fp_vec_mod(*pp.pairing_group, msk.b_int, 2 * (pp.l * pp.d + 1))
        );

        // Assign the pointers with generated values.
        di = &temp_di;
        r = &temp_r;
        bi = &temp_bi;
    }
    else{
        // Assign the pointers with values from msk.
        di = &msk.di;
        r = &msk.r;
        bi = &msk.bi;
    }

    // We compute the coefficient for when mat y equals to zero.
    auto coeff = Helper::coeff_poly(pp.d, *pp.pairing_group, fp_y);
    // Split the coefficient to two parts.
    coeff = Helper::split_poly(*pp.pairing_group, coeff);

    // Depends on whether sel is provided, we use the correct set of randomness.
    if (sel.empty()){
        // Sample a random vector gamma.
        const auto gamma = pp.pairing_group->Zp->rand_vec(2 * (pp.l * pp.d + 1));
        // Compute beta * c.
        const auto bc = pp.pairing_group->Zp->vec_mul(coeff, beta);
        // Compute beta * bi * c.
        auto bbic = pp.pairing_group->Zp->vec_mul(bc, *bi);
        // Compute beta * bi * c * gamma.
        auto bbicg = pp.pairing_group->Zp->vec_mul(bbic, gamma);
        // Compute the last point to join to the vector.
        auto temp = pp.pairing_group->Zp->vec_mul(coeff, gamma);
        auto last = pp.pairing_group->Zp->vec_ip(coeff, *r);
        last = pp.pairing_group->Zp->mul(last, beta);
        bbic.push_back(pp.pairing_group->Zp->mul(last, *di));
        // Raise to g2 and return.
        return pp.pairing_group->Gp->g2_raise(bbic);
    }

    // Create the selected vectors.
    FpVec sel_r, sel_bi;

    // Get the selected index.
    const auto sel_index = Helper::get_sel_index(pp.d, pp.l, sel);
    for (const auto i : sel_index){
        sel_r.push_back(r->at(i));
        sel_bi.push_back(bi->at(i));
    }
    for (const auto i : sel_index){
        sel_r.push_back(r->at(r->size() / 2 + i));
        sel_bi.push_back(bi->at(r->size() / 2 + i));
    }

    // Sample a random vector gamma.
    const auto gamma = pp.pairing_group->Zp->rand_vec(static_cast<int>(sel_r.size()));
    // Compute beta * c.
    const auto bc = pp.pairing_group->Zp->vec_mul(coeff, beta);
    // Compute beta * bi * c.
    auto bbic = pp.pairing_group->Zp->vec_mul(bc, sel_bi);
    // Compute beta * bi * c * gamma.
    auto bbicg = pp.pairing_group->Zp->vec_mul(bbic, gamma);

    // Compute the last point to join to the vector.
    auto temp = pp.pairing_group->Zp->vec_mul(coeff, gamma);
    auto last = pp.pairing_group->Zp->vec_ip(coeff, sel_r);
    last = pp.pairing_group->Zp->mul(last, beta);
    bbic.push_back(pp.pairing_group->Zp->mul(last, *di));

    // Raise to g2 and return.
    return pp.pairing_group->Gp->g2_raise(bbic);
}

bool FlexDbMf::dec(const FlexDbMfPp& pp, const G1Vec& ct, const G2Vec& sk, const IntVec& sel){
    // If sel is empty.
    if (sel.empty()) return Group::check_gt_unity(Group::pair(ct, sk));

    // Get the selected index.
    const auto sel_index = Helper::get_sel_index(pp.d, pp.l, sel);

    // Create the holder for selected ct.
    G1Vec sel_ct;
    for (const auto i : sel_index) sel_ct.push_back(ct[i]);
    for (const auto i : sel_index) sel_ct.push_back(ct[ct.size() / 2 + i]);
    // We also need to add the last point in ct.
    sel_ct.push_back(ct.back());
    return Group::check_gt_unity(Group::pair(sel_ct, sk));
}
