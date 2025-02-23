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

AggreMsk Aggre::msk_gen(const AggrePP& pp){
    // Create the msk instance.
    AggreMsk msk;

    // Sample a random point and find its inverse.
    msk.d = pp.pairing_group->Zp->rand();
    msk.di = pp.pairing_group->Zp->inv(msk.d);

    // Sample two random vectors and find one of the inverse.
    msk.r = pp.pairing_group->Zp->rand_vec(pp.l);
    msk.b = pp.pairing_group->Zp->rand_vec(pp.l);
    msk.bi = pp.pairing_group->Zp->vec_inv(msk.b);

    return msk;
}

G1Vec Aggre::enc(const AggrePP& pp, const AggreMsk& msk, const IntVec& x){
    // First convert x to field elements.
    const auto x_vec = pp.pairing_group->Zp->from_int(x);

    // Sample the random point alpha.
    const Fp alpha = pp.pairing_group->Zp->rand();

    // Compute x + r.
    const auto xr = pp.pairing_group->Zp->vec_add(x_vec, msk.r);
    // Compute b * (x + r).
    const auto bxr = pp.pairing_group->Zp->vec_mul(xr, msk.b);
    // Compute alpha * b * (x + r).
    auto abxr = pp.pairing_group->Zp->vec_mul(bxr, alpha);
    // Add the last point -alpha * delta.
    abxr.push_back(pp.pairing_group->Zp->neg(pp.pairing_group->Zp->mul(alpha, msk.d)));

    // Raise the vector to g1 and return.
    return pp.pairing_group->Gp->g1_raise(abxr);
}

G2Vec Aggre::keygen(const AggrePP& pp, const AggreMsk& msk, const IntVec& y, int p, const IntVec& sel){
    // Convert the input y integer vector to FpVec.
    FpVec y_vec = pp.pairing_group->Zp->from_int(y);

    // Select r and bi based on the input sel.
    FpVec sel_r, sel_bi;
    if (sel.empty()){
        sel_r = msk.r;
        sel_bi = msk.bi;
    }
    else{
        for (auto& i : sel){
            sel_r.push_back(msk.r[i]);
            sel_bi.push_back(msk.bi[i]);
        }
    }

    // Sample the random point beta.
    const Fp beta = pp.pairing_group->Zp->rand();

    // Compute b' * y.
    const auto biy = pp.pairing_group->Zp->vec_mul(y_vec, sel_bi);
    // Compute beta * b' * y.
    auto bbiy = pp.pairing_group->Zp->vec_mul(biy, beta);
    // Compute the last point beta * delta' * (p + <r, y>);
    auto temp = pp.pairing_group->Zp->vec_ip(y_vec, sel_r);
    temp = pp.pairing_group->Zp->add(temp, Fp(p));
    temp = pp.pairing_group->Zp->mul(temp, msk.di);
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
