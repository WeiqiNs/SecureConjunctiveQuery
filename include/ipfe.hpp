#pragma once

#include <cmath>
#include "crypto.hpp"
#include "field.hpp"
#include "group.hpp"

struct ipfe_pp{
    int d{};
    Field field_zp;
    Group group_bp;
};

struct ipfe_msk{
    zp k;
    zp det;
    zp_mat b;
    zp_mat bi;
    Hash hash;
};

class Ipfe{
public:
    static ipfe_pp pp_gen(const int& degree, const bool& pre = true);

    static ipfe_msk msk_gen(const ipfe_pp& pp, const int& input_len);

    static zp_vec prepare_msg(const ipfe_pp& pp, const ipfe_msk& msk, const int_vec& x);

    static zp_vec prepare_key(const ipfe_pp& pp, const ipfe_msk& msk, const int_vec& x);

    static g1_vec enc(const ipfe_pp& pp, const ipfe_msk& msk, const int_vec& x);

    static g2_vec keygen(const ipfe_pp& pp, const ipfe_msk& msk, const int_vec& y);

    static bool dec(const g1_vec& ct1, const g2_vec& sk1, const g1_vec& ct2, const g2_vec& sk2);
};
