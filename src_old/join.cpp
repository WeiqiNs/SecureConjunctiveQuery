#include "join.hpp"

pp Join::pp_gen(const int& degree, const bool& pre){
    return Filter::pp_gen(degree, pre);
}

join_msk Join::msk_gen(const pp& pp, const int& input_len){
    join_msk msk;
    Field::init_zp(msk.fr);
    pp.field_zp.rand(msk.fr);
    msk.r = pp.field_zp.vec_rand(2 * (input_len - 1) * (pp.d + 1));

    return msk;
}

g1_vec Join::enc(const pp& pp, const join_msk& msk, const int_vec& x){
    return enc(pp, msk, msk.hash.digest_int_vec(x));
}

g1_vec Join::enc(const pp& pp, const join_msk& msk, const str_vec& x){
    return enc(pp, msk, msk.hash.digest_str_vec(x));
}

g2_vec Join::keygen(const pp& pp, const join_msk& msk, const int_mat& y){
    zp_mat hashed_zp(y.size());

    for (int i = 0; i < y.size(); ++i){
        hashed_zp[i] = msk.hash.digest_int_vec(y[i]);
    }

    return keygen(pp, msk, hashed_zp);
}

g2_vec Join::keygen(const pp& pp, const join_msk& msk, const str_mat& y){
    zp_mat hashed_zp(y.size());

    for (int i = 0; i < y.size(); ++i){
        hashed_zp[i] = msk.hash.digest_str_vec(y[i]);
    }

    return keygen(pp, msk, hashed_zp);
}

void Join::dec(gt& r, const g1_vec& ct, const g2_vec& sk){
    Group::pair(r, ct, sk);
}

g1_vec Join::enc(const pp& pp, const join_msk& msk, const zp_vec& x){
    // Save the first vector as the vector to join.
    zp_vec first_ele(1);
    Field::copy(first_ele[0], x[0]);

    // For the first position compute x + ar.
    zp a, ar;
    Field::init_zp(a);
    Field::init_zp(ar);
    pp.field_zp.rand(a);
    pp.field_zp.mul(ar, a, msk.fr);
    pp.field_zp.add(first_ele[0], first_ele[0], ar);

    // Create the join vector which may stay empty.
    zp_vec mid_vec;

    // If the vector has filter condition.
    if (x.size() > 1){
        // Copy x[1:] to a new vector.
        zp_vec filter_x(x.size() - 1);
        for (int i = 0; i < filter_x.size(); ++i) Field::copy(filter_x[i], x[i + 1]);
        auto const poly_x = Filter::poly_msg(pp, filter_x);
        mid_vec = Field::vec_join(poly_x, poly_x);
        mid_vec = pp.field_zp.vec_add(mid_vec, msk.r);
        mid_vec = pp.field_zp.vec_mul(mid_vec, a);
    }

    const auto full_vec = Field::vec_join(first_ele, mid_vec);
    const auto raise = Field::vec_join(full_vec, a);

    return pp.group_bp.g1_raise(raise);
}

g2_vec Join::keygen(const pp& pp, const join_msk& msk, const zp_mat& y){
    zp b, g, temp, last;
    Field::init_zp(b);
    Field::init_zp(g);
    Field::init_zp(temp);
    Field::init_zp(last);

    // Sample the random points.
    pp.field_zp.rand(b);
    pp.field_zp.rand(g);

    // Create a vector to hold g.
    zp_vec first_ele(1);
    Field::copy(first_ele[0], g);

    // We compute the coefficient for when mat y equals to zero.
    const auto coeff = Filter::poly_key(pp, y);

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

    // Compute the coefficients of the input matrix.
    const auto by = pp.field_zp.vec_mul(full_coeff, b);

    // Compute the last point.
    pp.field_zp.vec_ip(last, full_coeff, msk.r);
    pp.field_zp.mul(last, last, b);
    pp.field_zp.mul(temp, g, msk.fr);
    pp.field_zp.add(last, last, temp);
    pp.field_zp.neg(last, last);

    const auto full_vec = Field::vec_join(first_ele, by);
    const auto raise = Field::vec_join(full_vec, last);

    // Multiply with the matrix B.
    return pp.group_bp.g2_raise(raise);
}
