#include "flexdb_sf.hpp"

FlexDbSfPp FlexDbSf::pp_gen(const int length, bool pre){
    // Create the pp instance.
    FlexDbSfPp pp{};
    // Update the input length according to input.
    pp.l = length;
    // Save the created pairing group.
    pp.pairing_group = std::make_unique<BP>(pre);
    return pp;
}

FlexDbSfMsk FlexDbSf::msk_gen(const FlexDbSfPp& pp, const CharVec& key, const bool& compress){
    // Create the msk instance.
    FlexDbSfMsk msk{};
    // Save whether the values need to be compressed.
    msk.compress = compress;
    // Get the unique point for HMAC.
    msk.hmac = std::make_unique<HMAC>(key);

    // Only when degree is larger than 1, the vectors are needed.
    if (compress){
        // Sample the starting point.
        msk.d_int = Helper::rand_int();
        msk.b_int = Helper::rand_int();
        msk.r_int = Helper::rand_int();
    }
    else{
        // Sample a random point and find its inverse.
        msk.d = pp.pairing_group->Zp->rand();
        msk.di = pp.pairing_group->Zp->inv(msk.d);
        // Sample random vectors.
        msk.b = pp.pairing_group->Zp->rand_vec(pp.l);
        msk.r = pp.pairing_group->Zp->rand_vec(pp.l);
    }

    // Return the master secret key.
    return msk;
}

G1Vec FlexDbSf::enc(const FlexDbSfPp& pp, const FlexDbSfMsk& msk, const Vec& x){
    // Convert the input vector to Fp.
    const auto fp_x = Helper::vec_to_fp(*pp.pairing_group, x);

    // Sample the random point alpha.
    const auto alpha = pp.pairing_group->Zp->rand();

    // Create pointers for values that needs to be used.
    const Fp* d;
    const FpVec *b, *r;
    // Create objects that may hold the compressed value.
    Fp temp_d;
    FpVec temp_b, temp_r;

    // First figure out whether the msk values needs to be sampled.
    if (msk.compress){
        // Sample the value d.
        temp_d = msk.hmac->digest_int_to_fp_vec_mod(*pp.pairing_group, msk.d_int, 1)[0];
        // Sample the vectors.
        temp_b = msk.hmac->digest_int_to_fp_vec_mod(*pp.pairing_group, msk.r_int, pp.l);
        temp_r = msk.hmac->digest_int_to_fp_vec_mod(*pp.pairing_group, msk.r_int, pp.l);
        // Assign the pointers with generated values.
        d = &temp_d;
        b = &temp_b;
        r = &temp_r;
    }
    else{
        // Assign the pointers with values from msk.
        d = &msk.d;
        b = &msk.b;
        r = &msk.r;
    }

    // Find x + r.
    const auto xr = pp.pairing_group->Zp->vec_add(fp_x, *r);
    // Find b * (x + r).
    const auto bxr = pp.pairing_group->Zp->vec_mul(xr, *b);
    // Find alpha * b * (x + r).
    auto abxr = pp.pairing_group->Zp->vec_mul(bxr, alpha);
    // Add the last point.
    abxr.push_back(pp.pairing_group->Zp->neg(pp.pairing_group->Zp->mul(*d, alpha)));
    // Raise the vector to g1 and return.
    return pp.pairing_group->Gp->g1_raise(abxr);
}

G2Vec FlexDbSf::keygen(const FlexDbSfPp& pp, const FlexDbSfMsk& msk, const Vec& y, const IntVec& sel){
    // Convert the input vector to Fp.
    const auto fp_y = Helper::vec_to_fp(*pp.pairing_group, y);

    // Sample the random point beta.
    const auto beta = pp.pairing_group->Zp->rand();

    // Create pointers for values that needs to be used and static variables to hold computed values.
    const Fp* di;
    const FpVec *b, *r;
    // Create objects that may hold the compressed value.
    Fp temp_di;
    FpVec temp_b, temp_r;

    // First figure out whether the msk values needs to be sampled.
    if (msk.compress){
        // Sample the value d and find its inverse.
        temp_di = pp.pairing_group->Zp->inv(
            msk.hmac->digest_int_to_fp_vec_mod(*pp.pairing_group, msk.d_int, 1)[0]
        );
        // Sample the vectors.
        temp_b = msk.hmac->digest_int_to_fp_vec_mod(*pp.pairing_group, msk.r_int, pp.l);
        temp_r = msk.hmac->digest_int_to_fp_vec_mod(*pp.pairing_group, msk.r_int, pp.l);
        // Assign the pointers with generated values.
        di = &temp_di;
        b = &temp_b;
        r = &temp_r;
    }
    else{
        // Assign the pointers with values from msk.
        di = &msk.di;
        b = &msk.b;
        r = &msk.r;
    }

    // Create holder for the y + r value.
    Fp yr;

    // Depends on whether sel is provided, we use the correct set of randomness.
    if (sel.empty()){
        // Compute y + r.
        auto temp = pp.pairing_group->Zp->vec_add(fp_y, *r);
        // Compute b * (y + r).
        temp = pp.pairing_group->Zp->vec_mul(temp, *b);
        // Sum the vector to get yr.
        yr = pp.pairing_group->Zp->vec_sum(temp);
    }
    else{
        // Get the selected index.
        FpVec sel_b, sel_r;
        for (const auto i : sel){
            sel_b.push_back(b->at(i));
            sel_r.push_back(r->at(i));
        }
        // Compute y + sel_r.
        auto temp = pp.pairing_group->Zp->vec_add(fp_y, sel_r);
        // Compute sel_b * (y + r).
        temp = pp.pairing_group->Zp->vec_mul(temp, sel_b);
        // Sum the vector to get yr.
        yr = pp.pairing_group->Zp->vec_sum(temp);
    }

    // Compute beta * b * (y + r).
    const auto byr = pp.pairing_group->Zp->mul(beta, yr);
    // Compute di * beta * b * (y + r).
    const auto dibyr = pp.pairing_group->Zp->mul(byr, *di);
    // Raise the vector to g2 and return.
    return pp.pairing_group->Gp->g2_raise(FpVec{beta, dibyr});
}

bool FlexDbSf::dec(const G1Vec& ct, const G2Vec& sk, const IntVec& sel){
    // Select ciphertext that should be used.
    G1Vec sel_ct;
    if (sel.empty()) sel_ct = G1Vec(ct.begin(), ct.end() - 1);
    else for (const auto i : sel) sel_ct.push_back(ct[i]);

    // Sum the selected ct and add the last point.
    const G1Vec pair_ct{Group::g1_add_vec(sel_ct), ct.back()};

    // Compute the pairing and output filter result.
    return Group::check_gt_unity(Group::pair(pair_ct, sk));
}
