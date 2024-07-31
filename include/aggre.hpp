#pragma once

#include "filter.hpp"

struct aggre_pp{
    Field field_zp;
    Group group_bp;
};

struct aggre_msk{
    zp d;
    zp dp;
    zp_vec r;
    zp_vec b;
    zp_vec bp;
};

class Aggre{
public:
    static aggre_pp pp_gen(const bool& pre = true);

    static aggre_msk msk_gen(const aggre_pp& pp, const int& input_len);

    static g1_vec enc(const aggre_pp& pp, const aggre_msk& msk, const int_vec& x);

    static g2_vec keygen(const aggre_pp& pp, const aggre_msk& msk, const int_vec& y, const int& p);

    static bool dec(g1_vec& ct, g2_vec& sk);
};
