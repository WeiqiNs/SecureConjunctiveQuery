#include "ipfe_join.hpp"

#include <iostream>

pp IpfeJoin::pp_gen(const int& degree, const bool& pre){
    return Filter::pp_gen(degree, pre);
}

ipfe_join_msk IpfeJoin::msk_gen(const pp& pp, const int& input_len){
    const int mat_size = (input_len - 1) * (pp.d + 1) + 3;

    ipfe_join_msk msk;
    Field::init_zp(msk.k);
    Field::init_zp(msk.det);

    pp.field_zp.rand(msk.k);
    msk.b = pp.field_zp.mat_rand(mat_size, mat_size);
    msk.bi = pp.field_zp.mat_inv_with_det(msk.b, msk.det);
    msk.bi = Field::mat_transpose(msk.bi);
    msk.bi = pp.field_zp.mat_mul(msk.bi, msk.det);

    return msk;
}

g1_vec IpfeJoin::enc(const pp& pp, const ipfe_join_msk& msk, const int_vec& x){
    return enc(pp, msk, msk.hash.digest_int_vec(x));
}

g1_vec IpfeJoin::enc(const pp& pp, const ipfe_join_msk& msk, const str_vec& x){
    return enc(pp, msk, msk.hash.digest_str_vec(x));
}

g2_vec IpfeJoin::keygen(const pp& pp, const ipfe_join_msk& msk, const int_mat& y){
    zp_mat hashed_zp(y.size());

    for (int i = 0; i < y.size(); ++i){
        hashed_zp[i] = msk.hash.digest_int_vec(y[i]);
    }

    return keygen(pp, msk, hashed_zp);
}

g2_vec IpfeJoin::keygen(const pp& pp, const ipfe_join_msk& msk, const str_mat& y){
    zp_mat hashed_zp(y.size());

    for (int i = 0; i < y.size(); ++i){
        hashed_zp[i] = msk.hash.digest_str_vec(y[i]);
    }

    return keygen(pp, msk, hashed_zp);
}

void IpfeJoin::dec(gt& r, const g1_vec& ct, const g2_vec& sk){
    Group::pair(r, ct, sk);
}

g1_vec IpfeJoin::enc(const pp& pp, const ipfe_join_msk& msk, const zp_vec& x){
    // Save the first vector as the vector to join.
    zp_vec join_vec(1);
    Field::copy(join_vec[0], x[0]);

    // If the vector has filter condition.
    if (x.size() > 1){
        // Copy x[1:] to a new vector.
        zp_vec filter_x(x.size() - 1);
        for (int i = 0; i < filter_x.size(); ++i) Field::copy(filter_x[i], x[i + 1]);
        join_vec = Field::vec_join(join_vec, Filter::poly_msg(pp, filter_x));
    }

    // Create a vector of length 2 and attach to the output vector.
    zp_vec last_vec(2);
    Field::zero(last_vec[1]);
    pp.field_zp.rand(last_vec[0]);

    // Join the H(x0) and polynomial filter and then the last two elements.
    const auto full_vec = Field::vec_join(join_vec, last_vec);

    // Multiply with the matrix B.
    const auto raise = pp.field_zp.mat_mul(msk.b, full_vec);

    return pp.group_bp.g1_raise(raise);
}

g2_vec IpfeJoin::keygen(const pp& pp, const ipfe_join_msk& msk, const zp_mat& y){
    // Create a vector to hold k.
    zp_vec join_vec(1);
    Field::copy(join_vec[0], msk.k);

    // Create a vector of length 2 and attach to the output vector.
    zp_vec last_vec(2);
    Field::zero(last_vec[0]);
    pp.field_zp.rand(last_vec[1]);

    // Compute the coefficients of the input matrix.
    auto full_vec = Field::vec_join(join_vec, Filter::poly_key(pp, y));
    full_vec = Field::vec_join(full_vec, last_vec);

    // Multiply with the matrix B.
    const auto raise = pp.field_zp.mat_mul(msk.bi, full_vec);
    return pp.group_bp.g2_raise(raise);
}


//
// g2_vec Ipfe::keygen(const pp& pp, const ipfe_filter_msk& msk, const zp_vec& join_vec, const zp_vec& filter_vec){
//
// }
//
// zp_vec Ipfe::prepare_key(const ipfe_pp& pp, const ipfe_msk& msk, const int_mat& y){
//     // We sample the random points.
//     zp r;
//     Field::init_zp(r);
//     pp.field_zp.rand(r);
//
//     // The vector would be of length 1 (H(a)) + degree * (m - 1) + 1 (r) + 1 (0).
//     zp_vec output(1);
//     Field::copy(output[0], msk.k);
//
//     // Attach the polynomial coefficients.
//     for (const auto& i : y){
//         output = Field::vec_join(output, pp.field_zp.find_coeff(pp.d, pp.field_zp.from_int_vec(i)));
//     }
//
//     // Create another vector of length 2 and attach to the output vector.
//     zp_vec last_output(2);
//     Field::zero(last_output[0]);
//     pp.field_zp.rand(last_output[1]);
//
//     return Field::vec_join(output, last_output);
// }
//
// g1_vec Ipfe::enc(const ipfe_pp& pp, const ipfe_msk& msk, const int_vec& x){
//     const zp_vec r = prepare_msg(pp, msk, x);
//     const zp_vec raise = pp.field_zp.mat_mul(msk.b, r);
//     return pp.group_bp.g1_raise(raise);
// }
//
// g2_vec Ipfe::keygen(const ipfe_pp& pp, const ipfe_msk& msk, const int_mat& y){
//     const zp_vec r = prepare_key(pp, msk, y);
//     const zp_vec raise = pp.field_zp.mat_mul(msk.bi, r);
//     return pp.group_bp.g2_raise(raise);
// }
//
// bool Ipfe::dec(const g1_vec& ct1, const g2_vec& sk1, const g1_vec& ct2, const g2_vec& sk2){
//     gt r1, r2;
//
//     Group::pair(r1, ct1, sk1);
//     Group::pair(r2, ct2, sk2);
//
//     return Group::cmp_gt(r1, r2);
// }
//
