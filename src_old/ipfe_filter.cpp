#include "ipfe_filter.hpp"

pp IpfeFilter::pp_gen(const int& degree, const bool& pre){
    return Filter::pp_gen(degree, pre);
}

ipfe_filter_msk IpfeFilter::msk_gen(const pp& pp, const int& input_len){
    const int mat_size = input_len * (pp.d + 1) + 2;

    ipfe_filter_msk msk;
    Field::init_zp(msk.det);

    msk.b = pp.field_zp.mat_rand(mat_size, mat_size);
    msk.bi = pp.field_zp.mat_inv_with_det(msk.b, msk.det);
    msk.bi = Field::mat_transpose(msk.bi);
    msk.bi = pp.field_zp.mat_mul(msk.bi, msk.det);

    return msk;
}

g1_vec IpfeFilter::enc(const pp& pp, const ipfe_filter_msk& msk, const int_vec& x){
    return enc(pp, msk, msk.hash.digest_int_vec(x));
}

g1_vec IpfeFilter::enc(const pp& pp, const ipfe_filter_msk& msk, const str_vec& x){
    return enc(pp, msk, msk.hash.digest_str_vec(x));
}

g2_vec IpfeFilter::keygen(const pp& pp, const ipfe_filter_msk& msk, const int_mat& y){
    zp_mat hashed_zp(y.size());

    for (int i = 0; i < y.size(); ++i){
        hashed_zp[i] = msk.hash.digest_int_vec(y[i]);
    }

    return keygen(pp, msk, hashed_zp);
}

g2_vec IpfeFilter::keygen(const pp& pp, const ipfe_filter_msk& msk, const str_mat& y){
    zp_mat hashed_zp(y.size());

    for (int i = 0; i < y.size(); ++i){
        hashed_zp[i] = msk.hash.digest_str_vec(y[i]);
    }

    return keygen(pp, msk, hashed_zp);
}

bool IpfeFilter::dec(const g1_vec& ct, const g2_vec& sk){
    gt r;
    Group::pair(r, ct, sk);
    return gt_is_unity(r);
}

g1_vec IpfeFilter::enc(const pp& pp, const ipfe_filter_msk& msk, const zp_vec& x){
    // We sample the random points.
    zp r;
    Field::init_zp(r);
    pp.field_zp.rand(r);

    // We compute the constant r times the vector poly(x).
    const auto poly_vec = pp.field_zp.vec_mul(Filter::poly_msg(pp, x), r);

    // Create a vector of length 2 and attach to the output vector.
    zp_vec last_vec(2);
    Field::zero(last_vec[1]);
    pp.field_zp.rand(last_vec[0]);

    // Multiply with the matrix B.
    const auto raise = pp.field_zp.mat_mul(msk.b, Field::vec_join(poly_vec, last_vec));

    return pp.group_bp.g1_raise(raise);
}

g2_vec IpfeFilter::keygen(const pp& pp, const ipfe_filter_msk& msk, const zp_mat& y){
    // Create a vector of length 2 and attach to the output vector.
    zp_vec last_vec(2);
    Field::zero(last_vec[0]);
    pp.field_zp.rand(last_vec[1]);

    // Compute the coefficients of the input matrix.
    const auto full_vec = Field::vec_join(Filter::poly_key(pp, y), last_vec);

    // Multiply with the matrix B.
    const auto raise = pp.field_zp.mat_mul(msk.bi, full_vec);
    return pp.group_bp.g2_raise(raise);
}
