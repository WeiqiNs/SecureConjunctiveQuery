#include "filter.hpp"

FilterPP Filter::pp_gen(const int degree, const int length, const bool pre){
    // Create the pp instance.
    FilterPP pp;
    // Update the degree according to input.
    pp.d = degree;
    // Update the input length according to input.
    pp.l = length;
    // Save the created pairing group.
    pp.pairing_group = std::make_unique<BP>(pre);

    return pp;
}

FilterMsk Filter::msk_gen(const FilterPP& pp){
    // Create the msk instance.
    FilterMsk msk;

    if (pp.d == 1){
        // If degree is 1, only r is needed.
        msk.r = pp.pairing_group->Zp->rand_vec(pp.l);
    }
    else{
        // Sample a random point and find its inverse.
        msk.d = pp.pairing_group->Zp->rand();
        msk.di = pp.pairing_group->Zp->inv(msk.d);
        // If degree is higher than 1, we need the length to be 2 * (length * degree + 1).
        // This utilizes the optimization of adding the constant together.
        msk.r = pp.pairing_group->Zp->rand_vec(2 * (pp.l * pp.d + 1));
        msk.b = pp.pairing_group->Zp->rand_vec(2 * (pp.l * pp.d + 1));
        msk.bi = pp.pairing_group->Zp->vec_inv(msk.b);
    }

    // Return the master secret key.
    return msk;
}

G1Vec Filter::enc(const FilterPP& pp, const FilterMsk& msk, const Vec& x){
    // Generate hash of the input x vector.
    const FpVec x_digest = msk.hash.digest_vec_to_fp(*pp.pairing_group, x);

    // Sample the random point alpha.
    const Fp alpha = pp.pairing_group->Zp->rand();

    // Depends on whether the degree is 1 or higher, we perform encryption differently.
    if (pp.d == 1){
        // Find x + r.
        const auto xr = pp.pairing_group->Zp->vec_add(x_digest, msk.r);
        // Find a(x + r).
        auto axr = pp.pairing_group->Zp->vec_mul(xr, alpha);
        // Append the last point negative a.
        axr.push_back(pp.pairing_group->Zp->neg(alpha));

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

    // Raise the vector to g1 and return.
    return pp.pairing_group->Gp->g1_raise(abxxr);
}

G2Vec Filter::keygen(const FilterPP& pp, const FilterMsk& msk, const VecOrMat& y){
    // Sample the random point beta.
    const Fp beta = pp.pairing_group->Zp->rand();

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
        // Compute the last element <y, y> + <y, r> and append it to the result.
        const auto temp = pp.pairing_group->Zp->add(
            pp.pairing_group->Zp->vec_ip(y_digest, y_digest),
            pp.pairing_group->Zp->vec_ip(y_digest, msk.r)
        );
        by.push_back(pp.pairing_group->Zp->mul(temp, beta));

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
    bbic.push_back(pp.pairing_group->Zp->mul(temp, msk.di));

    // Raise to g2 and return.
    return pp.pairing_group->Gp->g2_raise(bbic);
}

G2Vec Filter::keygen(const FilterPP& pp, const FilterMsk& msk, const VecOrMat& y, const IntVec& sel){
    // Sample the random point beta.
    const Fp beta = pp.pairing_group->Zp->rand();

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
        const auto temp = pp.pairing_group->Zp->add(
            pp.pairing_group->Zp->vec_ip(y_digest, y_digest),
            pp.pairing_group->Zp->vec_ip(y_digest, sel_r)
        );
        by.push_back(pp.pairing_group->Zp->mul(temp, beta));

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
    const auto sel_index = Helper::get_sel_index(pp.d, pp.l, sel);

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
    bbic.push_back(pp.pairing_group->Zp->mul(temp, msk.di));

    // Raise to g2 and return.
    return pp.pairing_group->Gp->g2_raise(bbic);
}

bool Filter::dec(const G1Vec& ct, const G2Vec& sk){ return gt_is_unity(Group::pair(ct, sk).value); }

bool Filter::dec(const FilterPP& pp, const G1Vec& ct, const G2Vec& sk, const IntVec& sel){
    if (pp.d == 1){
        // We select desired things from ct.
        G1Vec sel_ct;
        for (const auto i : sel) sel_ct.push_back(ct[i]);
        // We also need to add the last point in ct.
        sel_ct.push_back(ct.back());
        return dec(sel_ct, sk);
    }
    // Get the selected index.
    const auto sel_index = Helper::get_sel_index(pp.d, pp.l, sel);

    // Create the holder for selected ct.
    G1Vec sel_ct;
    for (const auto i : sel_index) sel_ct.push_back(ct[i]);
    for (const auto i : sel_index) sel_ct.push_back(ct[ct.size() / 2 + i]);
    // We also need to add the last point in ct.
    sel_ct.push_back(ct.back());

    return dec(sel_ct, sk);
}
