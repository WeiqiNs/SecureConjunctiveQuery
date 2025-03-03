#include "aggre.hpp"

AggrePP Aggre::pp_gen(const int length, const bool pre){
    // Create the pp instance.
    AggrePP pp;
    // Update the input length according to input.
    pp.l = length;
    // Save the created pairing group.
    pp.pairing_group = std::make_unique<BP>(pre);

    return pp;
}

AggreMsk Aggre::msk_gen(const AggrePP& pp, const CharVec& key, const bool& compress){
    // Create the msk instance.
    AggreMsk msk;
    // Save whether the values need to be compressed.
    msk.compress = compress;

    if (compress){
        // Sample the starting point.
        msk.d_int = Helper::rand_int();
        msk.r_int = Helper::rand_int();
        msk.b_int = Helper::rand_int();
        // Get the unique point for HMAC.
        msk.hmac = std::make_unique<HMAC>(key);
    }
    else{
        // Sample a random point and find its inverse.
        msk.d = pp.pairing_group->Zp->rand();
        msk.di = pp.pairing_group->Zp->inv(msk.d);

        // Sample two random vectors and find one of the inverse.
        msk.r = pp.pairing_group->Zp->rand_vec(pp.l);
        msk.b = pp.pairing_group->Zp->rand_vec(pp.l);
        msk.bi = pp.pairing_group->Zp->vec_inv(msk.b);
    }
    return msk;
}

G1Vec Aggre::enc(const AggrePP& pp, const AggreMsk& msk, const IntVec& x){
    // First convert x to field elements.
    const auto x_vec = pp.pairing_group->Zp->from_int(x);

    // Sample the random point alpha.
    const Fp alpha = pp.pairing_group->Zp->rand();

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
        temp_r = msk.hmac->digest_int_to_fp_vec_mod(*pp.pairing_group, msk.r_int, pp.l);
        temp_b = msk.hmac->digest_int_to_fp_vec_mod(*pp.pairing_group, msk.b_int, pp.l);
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

    // Compute x + r.
    const auto xr = pp.pairing_group->Zp->vec_add(x_vec, *r);
    // Compute b * (x + r).
    const auto bxr = pp.pairing_group->Zp->vec_mul(xr, *b);
    // Compute alpha * b * (x + r).
    auto abxr = pp.pairing_group->Zp->vec_mul(bxr, alpha);
    // Add the last point -alpha * delta.
    abxr.push_back(pp.pairing_group->Zp->neg(pp.pairing_group->Zp->mul(alpha, *d)));

    // Raise the vector to g1 and return.
    return pp.pairing_group->Gp->g1_raise(abxr);
}

G2Vec Aggre::keygen(const AggrePP& pp, const AggreMsk& msk, const IntVec& y, int p, const IntVec& sel){
    // Convert the input y integer vector to FpVec.
    const FpVec y_vec = pp.pairing_group->Zp->from_int(y);

    // Sample the random point beta.
    const Fp beta = pp.pairing_group->Zp->rand();

    // Create pointers for values that needs to be used and static variables to hold computed values.
    const Fp* di;
    const FpVec *r, *bi;
    Fp temp_di;
    FpVec temp_r, temp_bi;

    // In this case, first figure out whether the msk values needs to be sampled.
    if (msk.compress){
        // Only one value is generated.
        temp_di = pp.pairing_group->Zp->inv(
            msk.hmac->digest_int_to_fp_vec_mod(*pp.pairing_group, msk.d_int, 1)[0]
        );
        // Sample r and b.
        temp_r = msk.hmac->digest_int_to_fp_vec_mod(*pp.pairing_group, msk.r_int, pp.l);
        // Compute the bi.
        temp_bi = pp.pairing_group->Zp->vec_inv(
            msk.hmac->digest_int_to_fp_vec_mod(*pp.pairing_group, msk.b_int, pp.l)
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

    // Depends on whether sel is provided, we use the correct set of randomness.
    if (sel.empty()){
        // Compute b' * y.
        const auto biy = pp.pairing_group->Zp->vec_mul(y_vec, *bi);
        // Compute beta * b' * y.
        auto bbiy = pp.pairing_group->Zp->vec_mul(biy, beta);
        // Compute the last point beta * delta' * (p + <r, y>);
        auto temp = pp.pairing_group->Zp->vec_ip(y_vec, *r);
        temp = pp.pairing_group->Zp->add(temp, Fp(p));
        temp = pp.pairing_group->Zp->mul(temp, *di);
        temp = pp.pairing_group->Zp->mul(temp, beta);
        // Add the last point.
        bbiy.push_back(temp);

        // Raise the vector to g2 and return.
        return pp.pairing_group->Gp->g2_raise(bbiy);
    }

    // Create the selected vectors.
    FpVec sel_r, sel_bi;
    for (auto& i : sel){
        sel_r.push_back(r->at(i));
        sel_bi.push_back(bi->at(i));
    }

    // Compute b' * y.
    const auto biy = pp.pairing_group->Zp->vec_mul(y_vec, sel_bi);
    // Compute beta * b' * y.
    auto bbiy = pp.pairing_group->Zp->vec_mul(biy, beta);
    // Compute the last point beta * delta' * (p + <r, y>);
    auto temp = pp.pairing_group->Zp->vec_ip(y_vec, sel_r);
    temp = pp.pairing_group->Zp->add(temp, Fp(p));
    temp = pp.pairing_group->Zp->mul(temp, *di);
    temp = pp.pairing_group->Zp->mul(temp, beta);
    // Add the last point.
    bbiy.push_back(temp);

    // Raise the vector to g2 and return.
    return pp.pairing_group->Gp->g2_raise(bbiy);
}

bool Aggre::dec(const G1Vec& ct, const G2Vec& sk, const IntVec& sel){
    // Select ciphertext that should be used.
    G1Vec sel_ct;
    if (sel.empty()) sel_ct = G1Vec(ct.begin(), ct.end() - 1);
    else for (const auto i : sel) sel_ct.push_back(ct[i]);

    // Also add the last point int ct.
    sel_ct.push_back(ct.back());

    // Compute the pairing and output filter result.
    return Group::check_gt_unity(Group::pair(sel_ct, sk));
}
