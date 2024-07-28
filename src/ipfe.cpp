#include "ipfe.hpp"

#include <iostream>

ipfe_pp Ipfe::pp_gen(const int& degree, const bool& pre){
    zp p;
    init_get_order(p);

    ipfe_pp pp;
    pp.d = degree;
    pp.field_zp.init(p);
    pp.group_bp.init(pre);

    return pp;
}

ipfe_msk Ipfe::msk_gen(const ipfe_pp& pp, const int& input_len){
    const int mat_size = (input_len - 1) * (pp.d + 1) + 3;

    ipfe_msk msk;
    Field::init_zp(msk.k);
    Field::init_zp(msk.det);

    pp.field_zp.rand(msk.k);
    msk.b = pp.field_zp.mat_rand(mat_size, mat_size);
    msk.bi = pp.field_zp.mat_inv_with_det(msk.b, msk.det);
    msk.bi = Field::mat_transpose(msk.bi);
    msk.bi = pp.field_zp.mat_mul(msk.bi, msk.det);

    return msk;
}

zp_vec Ipfe::prepare_msg(const ipfe_pp& pp, const ipfe_msk& msk, const int_vec& x){
    // We sample the random points.
    zp r;
    Field::init_zp(r);
    pp.field_zp.rand(r);

    // The vector would be of length 1 (H(a)) + degree * (m - 1) + 1 (r) + 1 (0).
    zp_vec output((x.size() - 1) * (pp.d + 1) + 3);

    // We hash the first element.
    Helper::vec_to_zp(output[0], msk.hash.digest( Helper::int_to_vec(x[0])));

    // Copy the polynomial over to the result vector.
    for (int i = 1; i < x.size(); ++i){
        for (int j = 0; j <= pp.d; ++j){
            // Compute x_i^j.
            pp.field_zp.from_int(output[1 + (i - 1) * (pp.d + 1) + j], static_cast<int>(pow(x[i], j)));
            // Compute r * x_i^j.
            pp.field_zp.mul(output[1 + (i - 1) * (pp.d + 1) + j], output[1 + (i - 1) * (pp.d + 1) + j], r);
        }
    }

    // Finally set the second to last element to random.
    pp.field_zp.rand(output[output.size() - 2]);
    // Set the last element to zero.
    Field::zero(output[output.size() - 1]);

    // Return the output vector.
    return output;
}

zp_vec Ipfe::prepare_key(const ipfe_pp& pp, const ipfe_msk& msk, const int_vec& x){
    // We sample the random points.
    zp r;
    Field::init_zp(r);
    pp.field_zp.rand(r);

    // The vector would be of length 1 (H(a)) + degree * (m - 1) + 1 (r) + 1 (0).
    zp_vec output(1);
    Field::copy(output[0], msk.k);

    std::cout << output.size() << std::endl;

    // Attach the polynomial coefficients.
    for (const int i : x){
        int_vec temp;
        for (int j = 1; j <= pp.d; ++j){
            temp.push_back(static_cast<int>(pow(i, j)));
        }
        output = Field::vec_join(output, pp.field_zp.find_coeff(pp.field_zp.from_int_vec(temp)));
        std::cout << output.size() << std::endl;
    }

    // Create another vector of length 2 and attach to the output vector.
    zp_vec last_output(2);
    Field::zero(last_output[0]);
    pp.field_zp.rand(last_output[1]);

    std::cout << output.size() << std::endl;

    return Field::vec_join(output, last_output);
}

g1_vec Ipfe::enc(const ipfe_pp& pp, const ipfe_msk& msk, const int_vec& x){
    const zp_vec r = prepare_msg(pp, msk, x);
    const zp_vec raise = pp.field_zp.mat_mul(msk.b, r);
    return pp.group_bp.g1_raise(raise);
}

g2_vec Ipfe::keygen(const ipfe_pp& pp, const ipfe_msk& msk, const int_vec& y){
    const zp_vec r = prepare_key(pp, msk, y);
    const zp_vec raise = pp.field_zp.mat_mul(msk.bi, r);
    return pp.group_bp.g2_raise(raise);
}

bool Ipfe::dec(const g1_vec& ct1, const g2_vec& sk1, const g1_vec& ct2, const g2_vec& sk2){
    gt r1, r2;

    Group::pair(r1, ct1, sk1);
    Group::pair(r2, ct2, sk2);

    return Group::cmp_gt(r1, r2);
}

