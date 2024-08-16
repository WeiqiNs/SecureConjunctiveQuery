#include "filter.hpp"

pp Filter::pp_gen(const int& degree, const bool& pre){
    zp p;
    init_get_order(p);

    pp pp;
    pp.d = degree;
    pp.field_zp.init(p);
    pp.group_bp.init(pre);

    return pp;
}

filter_msk Filter::msk_gen(const pp& pp, const int& input_len){
    filter_msk msk;
    Field::init_zp(msk.d);
    Field::init_zp(msk.di);
    pp.field_zp.rand(msk.d);
    pp.field_zp.inv(msk.di, msk.d);

    msk.r = pp.field_zp.vec_rand(2 * input_len * (pp.d + 1));
    msk.b = pp.field_zp.vec_rand(2 * input_len * (pp.d + 1));
    msk.bi = pp.field_zp.vec_inv(msk.b);

    return msk;
}

zp_vec Filter::poly_msg(const pp& pp, const zp_vec& x){
    // The vector would be of length degree * len(x).
    zp_vec output(x.size() * (pp.d + 1));

    // Set values for the power.
    zp_vec power(pp.d + 1);
    for (int i = 0; i < power.size(); ++i) pp.field_zp.from_int(power[i], i);

    // Copy the polynomial over to the result vector.
    for (int i = 0; i < x.size(); ++i){
        for (int j = 0; j <= pp.d; ++j){
            // Compute x_i^j.
            pp.field_zp.exp(output[i * (pp.d + 1) + j], x[i], power[j]);
        }
    }

    // Return the output vector.
    return output;
}

zp_vec Filter::poly_key(const pp& pp, const zp_mat& y){
    // The vector would be of length degree * len(x).
    zp_vec output;

    // Attach the polynomial coefficients.
    for (const auto& roots : y){
        output = Field::vec_join(output, pp.field_zp.find_coeff(pp.d, roots));
    }

    return output;
}

g1_vec Filter::enc(const pp& pp, const filter_msk& msk, const int_vec& x){
    return enc(pp, msk, msk.hash.digest_int_vec(x));
}

g1_vec Filter::enc(const pp& pp, const filter_msk& msk, const str_vec& x){
    return enc(pp, msk, msk.hash.digest_str_vec(x));
}


g2_vec Filter::keygen(const pp& pp, const filter_msk& msk, const int_mat& y){
    zp_mat hashed_zp(y.size());

    for (int i = 0; i < y.size(); ++i){
        hashed_zp[i] = msk.hash.digest_int_vec(y[i]);
    }

    return keygen(pp, msk, hashed_zp);
}

g2_vec Filter::keygen(const pp& pp, const filter_msk& msk, const str_mat& y){
    zp_mat hashed_zp(y.size());

    for (int i = 0; i < y.size(); ++i){
        hashed_zp[i] = msk.hash.digest_str_vec(y[i]);
    }

    return keygen(pp, msk, hashed_zp);
}

g2_vec Filter::keygen(const pp& pp, const filter_msk& msk, const int_mat& y, const int_vec& sel){
    zp_mat hashed_zp(y.size());

    for (int i = 0; i < y.size(); ++i){
        hashed_zp[i] = msk.hash.digest_int_vec(y[i]);
    }

    return keygen(pp, msk, hashed_zp, sel);
}

g2_vec Filter::keygen(const pp& pp, const filter_msk& msk, const str_mat& y, const int_vec& sel){
    zp_mat hashed_zp(y.size());

    for (int i = 0; i < y.size(); ++i){
        hashed_zp[i] = msk.hash.digest_str_vec(y[i]);
    }

    return keygen(pp, msk, hashed_zp, sel);
}

bool Filter::dec(g1_vec& ct, g2_vec& sk){
    // Decalre two variables in Gt.
    gt x, y;

    // First compute a x byyr.
    Group::pair(x, ct.back(), sk.back());

    // Remove them from the vector.
    ct.pop_back();
    sk.pop_back();

    // Compute ip pairing.
    Group::pair(y, ct, sk);

    // Check for equality.
    return Group::cmp_gt(x, y);
}

bool Filter::dec(const pp& pp, g1_vec& ct, g2_vec& sk, const int_vec& sel){
    // Decalre two variables in Gt.
    gt x, y;

    // First compute a x byyr.
    Group::pair(x, ct.back(), sk.back());

    // Remove them from the vector.
    ct.pop_back();
    sk.pop_back();

    // Copy the selected randomness.
    g1_vec sel_ct(sel.size() * (pp.d + 1) * 2);
    for (int i = 0; i < sel_ct.size(); ++i){
        g1_null(sel_ct[i]);
        g1_new(sel_ct[i]);
    }

    // Copy over the values now.
    for (int i = 0; i < sel.size(); ++i){
        for (int j = 0; j <= pp.d; ++j){
            g1_copy(sel_ct[i * (pp.d + 1) + j], ct[sel[i] * (pp.d + 1) + j]);
            g1_copy(
                sel_ct[sel.size() / 2 + i * (pp.d + 1) + j],
                ct[ct.size() / 2 + sel[i] * (pp.d + 1) + j]
            );
        }
    }

    // Compute ip pairing.
    Group::pair(y, sel_ct, sk);

    // Check for equality.
    return Group::cmp_gt(x, y);
}

g1_vec Filter::enc(const pp& pp, const filter_msk& msk, const zp_vec& x){
    // Sample the random point alpha.
    zp alpha;
    Field::init_zp(alpha);
    pp.field_zp.rand(alpha);

    // We compute the value for evaluating polynomial at x.
    const auto poly_x = poly_msg(pp, x);
    // Duplicate the poly(x).
    const auto xx = Field::vec_join(poly_x, poly_x);
    // Compute poly(x)||poly(x) + r.
    const auto xxr = pp.field_zp.vec_add(xx, msk.r);
    // Compute alpha * (poly(x)||poly(x) + r).
    const auto axxr = pp.field_zp.vec_mul(xxr, alpha);
    // Compute alpha * b * (poly(x)||poly(x) + r).
    const auto abxxr = pp.field_zp.vec_mul(axxr, msk.b);

    // Compute the last point to join to the vector.
    zp last;
    Field::init_zp(last);
    pp.field_zp.mul(last, alpha, msk.d);

    // Raise to g1 and return.
    return pp.group_bp.g1_raise(Field::vec_join(abxxr, last));
}

g2_vec Filter::keygen(const pp& pp, const filter_msk& msk, const zp_mat& y){
    // Sample the random point beta.
    zp beta;
    Field::init_zp(beta);
    pp.field_zp.rand(beta);

    // We compute the coefficient for when mat y equals to zero.
    const auto coeff = poly_key(pp, y);

    // Sample the first set of coefficients.
    zp_vec c1(coeff.size());
    // Make sure c1 != coeff.
    int i = 0;
    while (i < coeff.size()){
        if (Field::cmp(c1[i], coeff[i])) pp.field_zp.rand(c1[i]);
        else ++i;
    }

    // Compute the second set of coefficients.
    const auto c2 = pp.field_zp.vec_sub(coeff, c1);
    // Join the full set of coefficients.
    const auto full_coeff = Field::vec_join(c1, c2);
    // Compute beta * c.
    const auto bc = pp.field_zp.vec_mul(full_coeff, beta);
    // Compute beta * bi * c.
    const auto bbic = pp.field_zp.vec_mul(bc, msk.bi);

    // Compute the last point to join to the vector.
    zp last;
    Field::init_zp(last);
    pp.field_zp.vec_ip(last, full_coeff, msk.r);
    pp.field_zp.mul(last, last, msk.di);
    pp.field_zp.mul(last, last, beta);

    // Raise to g2 and return.
    return pp.group_bp.g2_raise(Field::vec_join(bbic, last));
}

g2_vec Filter::keygen(const pp& pp, const filter_msk& msk, const zp_mat& y, const int_vec& sel){
    // Sample the random point beta.
    zp beta;
    Field::init_zp(beta);
    pp.field_zp.rand(beta);

    // We compute the coefficient for when mat y equals to zero.
    const auto coeff = poly_key(pp, y);

    // Sample the first set of coefficients.
    zp_vec c1(coeff.size());
    // Make sure c1 != coeff.
    int i = 0;
    while (i < coeff.size()){
        if (Field::cmp(c1[i], coeff[i])) pp.field_zp.rand(c1[i]);
        else ++i;
    }

    // Compute the second set of coefficients.
    const auto c2 = pp.field_zp.vec_sub(coeff, c1);
    // Join the full set of coefficients.
    const auto full_coeff = Field::vec_join(c1, c2);

    // Copy the selected randomness.
    zp_vec sel_r(full_coeff.size());
    zp_vec sel_bi(full_coeff.size());
    for (i = 0; i < full_coeff.size(); ++i){
        Field::init_zp(sel_r[i]);
        Field::init_zp(sel_bi[i]);
    }

    // Copy over the values now.
    for (i = 0; i < sel.size(); ++i){
        for (int j = 0; j <= pp.d; ++j){
            Field::copy(sel_r[i * (pp.d + 1) + j], msk.r[sel[i] * (pp.d + 1) + j]);
            Field::copy(
                sel_r[sel.size() / 2 + i * (pp.d + 1) + j],
                msk.r[msk.r.size() / 2 + sel[i] * (pp.d + 1) + j]
            );
            Field::copy(sel_bi[i * (pp.d + 1) + j], msk.bi[sel[i] * (pp.d + 1) + j]);
            Field::copy(
                sel_bi[sel_bi.size() / 2 + i * (pp.d + 1) + j],
                msk.bi[msk.bi.size() / 2 + sel[i] * (pp.d + 1) + j]
            );
        }
    }

    // Compute beta * c.
    const auto bc = pp.field_zp.vec_mul(full_coeff, beta);
    // Compute beta * bi * c.
    const auto bbic = pp.field_zp.vec_mul(bc, sel_bi);
    // Compute the last point to join to the vector.
    zp last;
    Field::init_zp(last);
    pp.field_zp.vec_ip(last, full_coeff, sel_r);
    pp.field_zp.mul(last, last, msk.di);
    pp.field_zp.mul(last, last, beta);

    // Raise to g2 and return.
    return pp.group_bp.g2_raise(Field::vec_join(bbic, last));
}
