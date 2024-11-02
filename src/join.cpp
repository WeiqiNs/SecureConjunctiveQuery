#include "join.hpp"


// pp Join::pp_gen(const int& degree, const bool& pre){
//     return Filter::pp_gen(degree, pre);
// }
//
// join_msk Join::msk_gen(const pp& pp, const int& input_len){
//     join_msk msk;
//     Field::init_zp(msk.fr);
//     pp.field_zp.rand(msk.fr);
//     msk.r = pp.field_zp.vec_rand(2 * (input_len - 1) * (pp.d + 1));
//
//     return msk;
// }
//
// g1_vec Join::enc(const pp& pp, const join_msk& msk, const int_vec& x){
//     return enc(pp, msk, msk.hash.digest_int_vec(x));
// }
//
// g1_vec Join::enc(const pp& pp, const join_msk& msk, const str_vec& x){
//     return enc(pp, msk, msk.hash.digest_str_vec(x));
// }
//
// g2_vec Join::keygen(const pp& pp, const join_msk& msk, const int_mat& y){
//     zp_mat hashed_zp(y.size());
//
//     for (int i = 0; i < y.size(); ++i){
//         hashed_zp[i] = msk.hash.digest_int_vec(y[i]);
//     }
//
//     return keygen(pp, msk, hashed_zp);
// }
//
// g2_vec Join::keygen(const pp& pp, const join_msk& msk, const str_mat& y){
//     zp_mat hashed_zp(y.size());
//
//     for (int i = 0; i < y.size(); ++i){
//         hashed_zp[i] = msk.hash.digest_str_vec(y[i]);
//     }
//
//     return keygen(pp, msk, hashed_zp);
// }
//
// void Join::dec(gt& r, const g1_vec& ct, const g2_vec& sk){
//     Group::pair(r, ct, sk);
// }
//
// g1_vec Join::enc(const pp& pp, const join_msk& msk, const zp_vec& x){
//     // Save the first vector as the vector to join.
//     zp_vec first_ele(1);
//     Field::copy(first_ele[0], x[0]);
//
//     // For the first position compute x + ar.
//     zp a, ar;
//     Field::init_zp(a);
//     Field::init_zp(ar);
//     pp.field_zp.rand(a);
//     pp.field_zp.mul(ar, a, msk.fr);
//     pp.field_zp.add(first_ele[0], first_ele[0], ar);
//
//     // Create the join vector which may stay empty.
//     zp_vec mid_vec;
//
//     // If the vector has filter condition.
//     if (x.size() > 1){
//         // Copy x[1:] to a new vector.
//         zp_vec filter_x(x.size() - 1);
//         for (int i = 0; i < filter_x.size(); ++i) Field::copy(filter_x[i], x[i + 1]);
//         auto const poly_x = Filter::poly_msg(pp, filter_x);
//         mid_vec = Field::vec_join(poly_x, poly_x);
//         mid_vec = pp.field_zp.vec_add(mid_vec, msk.r);
//         mid_vec = pp.field_zp.vec_mul(mid_vec, a);
//     }
//
//     const auto full_vec = Field::vec_join(first_ele, mid_vec);
//     const auto raise = Field::vec_join(full_vec, a);
//
//     return pp.group_bp.g1_raise(raise);
// }
//
// g2_vec Join::keygen(const pp& pp, const join_msk& msk, const zp_mat& y){
//     zp b, g, temp, last;
//     Field::init_zp(b);
//     Field::init_zp(g);
//     Field::init_zp(temp);
//     Field::init_zp(last);
//
//     // Sample the random points.
//     pp.field_zp.rand(b);
//     pp.field_zp.rand(g);
//
//     // Create a vector to hold g.
//     zp_vec first_ele(1);
//     Field::copy(first_ele[0], g);
//
//     // We compute the coefficient for when mat y equals to zero.
//     const auto coeff = Filter::poly_key(pp, y);
//
//     // Sample the first set of coefficients.
//     zp_vec c1(coeff.size());
//     // Make sure c1 != coeff.
//     int i = 0;
//     while (i < coeff.size()){
//         if (Field::cmp(c1[i], coeff[i])) pp.field_zp.rand(c1[i]);
//         else ++i;
//     }
//
//     // Compute the second set of coefficients.
//     const auto c2 = pp.field_zp.vec_sub(coeff, c1);
//     // Join the full set of coefficients.
//     const auto full_coeff = Field::vec_join(c1, c2);
//
//     // Compute the coefficients of the input matrix.
//     const auto by = pp.field_zp.vec_mul(full_coeff, b);
//
//     // Compute the last point.
//     pp.field_zp.vec_ip(last, full_coeff, msk.r);
//     pp.field_zp.mul(last, last, b);
//     pp.field_zp.mul(temp, g, msk.fr);
//     pp.field_zp.add(last, last, temp);
//     pp.field_zp.neg(last, last);
//
//     const auto full_vec = Field::vec_join(first_ele, by);
//     const auto raise = Field::vec_join(full_vec, last);
//
//     // Multiply with the matrix B.
//     return pp.group_bp.g2_raise(raise);
// }
JoinPP Join::pp_gen(const int degree, const int length, const bool pre){
    // Create the pp instance.
    JoinPP pp;
    // Update the degree according to input.
    pp.d = degree;
    // Update the input length according to input.
    pp.l = length;
    // Save the created pairing group.
    pp.pairing_group = std::make_unique<BP>(pre);

    return pp;
}

JoinMsk Join::msk_gen(const JoinPP& pp){
    // Create the msk instance.
    JoinMsk msk;

    if (pp.d == 1){
        // Sample the random point k and l.
        msk.k = pp.pairing_group->Zp->rand();
        msk.l = pp.pairing_group->Zp->rand();
        // If degree is 1, only r is needed.
        msk.r = pp.pairing_group->Zp->rand_vec(pp.l - 1);
    }
    else{
        // Sample the random point k and l.
        msk.k = pp.pairing_group->Zp->rand();
        msk.l = pp.pairing_group->Zp->rand();
        // Sample a random point and find its inverse.
        msk.d = pp.pairing_group->Zp->rand();
        msk.di = pp.pairing_group->Zp->inv(msk.d);
        // If degree is higher than 1, we need the length to be 2 * ((length - 1) * degree + 1).
        // This utilizes the optimization of adding the constant together.
        msk.r = pp.pairing_group->Zp->rand_vec(2 * ((pp.l - 1) * pp.d + 1));
        msk.b = pp.pairing_group->Zp->rand_vec(2 * ((pp.l - 1) * pp.d + 1));
        msk.bi = pp.pairing_group->Zp->vec_inv(msk.b);
    }

    // Return the master secret key.
    return msk;
}

G1Vec Join::enc(const JoinPP& pp, const JoinMsk& msk, const Vec& x, int join_index){
    // Generate hash of the input x vector.
    auto x_digest = msk.hash.digest_vec_to_fp(*pp.pairing_group, x);

    // Get the value at the join position and remove it.
    const auto join_on = x_digest[join_index];
    x_digest.erase(x_digest.begin() + join_index);

    // Sample the random point alpha.
    const auto alpha = pp.pairing_group->Zp->rand();

    // Depends on whether the degree is 1 or higher, we perform encryption differently.
    if (pp.d == 1){
        // Find x + r.
        const auto xr = pp.pairing_group->Zp->vec_add(x_digest, msk.r);
        // Find a(x + r).
        auto axr = pp.pairing_group->Zp->vec_mul(xr, alpha);
        // Append the last point negative a.
        axr.push_back(pp.pairing_group->Zp->neg(alpha));
        // Append join + ar.
        axr.push_back(pp.pairing_group->Zp->add(join_on, pp.pairing_group->Zp->mul(msk.k, alpha)));

        // Raise the vector to g1 and return.
        return pp.pairing_group->Gp->g1_raise(axr);
    }

    // Here is the case where the degree is higher than 1, we compute the value for evaluating polynomial at x.
    const auto poly_x = Helper::power_poly(pp.d, *pp.pairing_group, x_digest);
    // Duplicate the poly(x).
    const auto xx = Field::vec_join(poly_x, poly_x);
    // Compute poly(x)||poly(x) + r.
    const auto xxr = pp.pairing_group->Zp->vec_add(xx, msk.r);
    // Compute alpha * (poly(x)||poly(x) + r).
    const auto axxr = pp.pairing_group->Zp->vec_mul(xxr, alpha);
    // Compute alpha * vec_b * (poly(x)||poly(x) + r).
    auto abxxr = pp.pairing_group->Zp->vec_mul(axxr, msk.b);
    // Attach the last point -alpha * delta.
    abxxr.push_back(pp.pairing_group->Zp->neg(pp.pairing_group->Zp->mul(alpha, msk.d)));
    // Also attach join + ar.
    abxxr.push_back(pp.pairing_group->Zp->add(join_on, pp.pairing_group->Zp->mul(msk.k, alpha)));

    // Raise the vector to g1 and return.
    return pp.pairing_group->Gp->g1_raise(abxxr);
}

G2Vec Join::keygen(const JoinPP& pp, const JoinMsk& msk, const VecOrMat& y){
    // Sample the random point beta.
    const auto beta = pp.pairing_group->Zp->rand();

    // Depends on whether the degree is 1 or higher, we perform key generation differently.
    if (pp.d == 1){
        // Generate the hash of the input y vector.
        FpVec y_digest;
        // Make sure the input y is vector type.
        std::visit([&pp, &msk, &y_digest](auto&& input_y){
            using T = std::decay_t<decltype(input_y)>;
            if constexpr (std::is_same_v<T, IntVec> || std::is_same_v<T, StrVec>)
                y_digest = msk.hash.digest_vec_to_fp(*pp.pairing_group, input_y);
            else throw std::invalid_argument("The degree must be 1 when inputting a y vector.");
        }, y);

        // Compute by.
        auto by = pp.pairing_group->Zp->vec_mul(y_digest, beta);
        // Compute the last element beta * (<y, y> + <y, r>) + phi * k.
        auto temp = pp.pairing_group->Zp->add(
            pp.pairing_group->Zp->vec_ip(y_digest, y_digest),
            pp.pairing_group->Zp->vec_ip(y_digest, msk.r)
        );
        temp = pp.pairing_group->Zp->mul(temp, beta);
        // Append it to the result.
        by.push_back(pp.pairing_group->Zp->add(temp, pp.pairing_group->Zp->mul(msk.k, msk.l)));
        // Also append the extra term phi.
        by.push_back(msk.l);

        // Raise the vector to g2 and return.
        return pp.pairing_group->Gp->g2_raise(by);
    }

    // Generate the hash of the input y vector.
    FpMat y_digest;
    // Make sure the input y is vector type.
    std::visit([&pp, &msk, &y_digest](auto&& input_y){
        using T = std::decay_t<decltype(input_y)>;
        if constexpr (std::is_same_v<T, IntMat> || std::is_same_v<T, StrMat>)
            y_digest = msk.hash.digest_mat_to_fp(*pp.pairing_group, input_y);
        else throw std::invalid_argument("The degree must be more than 1 when inputting a y matrix.");
    }, y);

    // We compute the coefficient for when mat y equals to zero.
    auto coeff = Helper::coeff_poly(pp.d, *pp.pairing_group, y_digest);
    // Split the coefficient to two parts.
    coeff = Helper::split_poly(*pp.pairing_group, coeff);

    // Compute beta * c.
    const auto bc = pp.pairing_group->Zp->vec_mul(coeff, beta);
    // Compute beta * bi * c.
    auto bbic = pp.pairing_group->Zp->vec_mul(bc, msk.bi);

    // Compute the last point to join to the vector.
    auto temp = pp.pairing_group->Zp->vec_ip(coeff, msk.r);
    temp = pp.pairing_group->Zp->mul(temp, beta);
    temp = pp.pairing_group->Zp->add(temp, pp.pairing_group->Zp->mul(msk.k, msk.l));
    bbic.push_back(pp.pairing_group->Zp->mul(temp, msk.di));
    // Also append the extra term phi.
    bbic.push_back(msk.l);

    // Raise to g2 and return.
    return pp.pairing_group->Gp->g2_raise(bbic);
}

G2Vec Join::keygen(const JoinPP& pp, const JoinMsk& msk, const VecOrMat& y, const IntVec& sel){
    // Sample the random point beta.
    const auto beta = pp.pairing_group->Zp->rand();

    // Depends on whether the degree is 1 or higher, we perform key generation differently.
    if (pp.d == 1){
        // Generate the hash of the input y vector.
        FpVec y_digest;
        // Make sure the input y is vector type.
        std::visit([&pp, &msk, &y_digest](auto&& input_y){
            using T = std::decay_t<decltype(input_y)>;
            if constexpr (std::is_same_v<T, IntVec> || std::is_same_v<T, StrVec>)
                y_digest = msk.hash.digest_vec_to_fp(*pp.pairing_group, input_y);
            else throw std::invalid_argument("The degree must be 1 when inputting a y vector.");
        }, y);

        // Select desired r.
        FpVec sel_r;
        for (const auto i : sel) sel_r.push_back(msk.r[i]);

        // Compute by.
        auto by = pp.pairing_group->Zp->vec_mul(y_digest, beta);
        // Compute the last element <y, y> + <y, r> and append it to the result.
        auto temp = pp.pairing_group->Zp->add(
            pp.pairing_group->Zp->vec_ip(y_digest, y_digest),
            pp.pairing_group->Zp->vec_ip(y_digest, sel_r)
        );
        temp = pp.pairing_group->Zp->mul(temp, beta);
        // Append it to the result.
        by.push_back(pp.pairing_group->Zp->add(temp, pp.pairing_group->Zp->mul(msk.k, msk.l)));
        // Also append the extra term phi.
        by.push_back(msk.l);

        // Raise the vector to g2 and return.
        return pp.pairing_group->Gp->g2_raise(by);
    }

    // Generate the hash of the input y vector.
    FpMat y_digest;
    // Make sure the input y is vector type.
    std::visit([&pp, &msk, &y_digest](auto&& input_y){
        using T = std::decay_t<decltype(input_y)>;
        if constexpr (std::is_same_v<T, IntMat> || std::is_same_v<T, StrMat>)
            y_digest = msk.hash.digest_mat_to_fp(*pp.pairing_group, input_y);
        else throw std::invalid_argument("The degree must be more than 1 when inputting a y matrix.");
    }, y);

    // We compute the coefficient for when mat y equals to zero.
    auto coeff = Helper::coeff_poly(pp.d, *pp.pairing_group, y_digest);
    // Split the coefficient to two parts.
    coeff = Helper::split_poly(*pp.pairing_group, coeff);

    // Get the selected index.
    const auto sel_index = Helper::get_sel_index(pp.d, pp.l - 1, sel);

    // Select r and bi.
    FpVec sel_r, sel_bi;
    for (const auto i : sel_index){
        sel_r.push_back(msk.r[i]);
        sel_bi.push_back(msk.bi[i]);
    }
    for (const auto i : sel_index){
        sel_r.push_back(msk.r[msk.r.size() / 2 + i]);
        sel_bi.push_back(msk.bi[msk.r.size() / 2 + i]);
    }

    // Compute beta * c.
    const auto bc = pp.pairing_group->Zp->vec_mul(coeff, beta);
    // Compute beta * bi * c.
    auto bbic = pp.pairing_group->Zp->vec_mul(bc, sel_bi);

    // Compute the last point to join to the vector.
    auto temp = pp.pairing_group->Zp->vec_ip(coeff, sel_r);
    temp = pp.pairing_group->Zp->mul(temp, beta);
    temp = pp.pairing_group->Zp->add(temp, pp.pairing_group->Zp->mul(msk.k, msk.l));
    bbic.push_back(pp.pairing_group->Zp->mul(temp, msk.di));
    // Also append the extra term phi.
    bbic.push_back(msk.l);

    // Raise to g2 and return.
    return pp.pairing_group->Gp->g2_raise(bbic);
}

Gt Join::dec(const G1Vec& ct, const G2Vec& sk){ return Group::pair(ct, sk); }

Gt Join::dec(const JoinPP& pp, const G1Vec& ct, const G2Vec& sk, const IntVec& sel){
    if (pp.d == 1){
        // We select desired things from ct.
        G1Vec sel_ct;
        for (const auto i : sel) sel_ct.push_back(ct[i]);
        // We also need to add the last two points in ct.
        sel_ct.push_back(ct[ct.size() - 2]);
        sel_ct.push_back(ct[ct.size() - 1]);
        return dec(sel_ct, sk);
    }
    // Get the selected index.
    const auto sel_index = Helper::get_sel_index(pp.d, pp.l - 1, sel);

    // Create the holder for selected ct.
    G1Vec sel_ct;
    for (const auto i : sel_index) sel_ct.push_back(ct[i]);
    for (const auto i : sel_index) sel_ct.push_back(ct[ct.size() / 2 + i - 1]);
    // We also need to add the last two points in ct.
    sel_ct.push_back(ct[ct.size() - 2]);
    sel_ct.push_back(ct[ct.size() - 1]);

    return dec(sel_ct, sk);
}
