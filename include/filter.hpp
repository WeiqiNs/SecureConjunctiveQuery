#pragma once

#include "group.hpp"
#include "crypto.hpp"

struct pp{
    Field field_zp;
    Group group_bp;
};

struct filter_msk{
    zp_vec r;
    Hash hash;
};

class Filter{
public:
    static pp pp_gen(const bool& pre = true);

    static filter_msk msk_gen(const pp& pp, const int& input_len);

    static g1_vec enc(const pp& pp, const filter_msk& msk, const int_vec& x);

    static g1_vec enc(const pp& pp, const filter_msk& msk, const str_vec& x);

    static g2_vec keygen(const pp& pp, const filter_msk& msk, const int_vec& y);

    static g2_vec keygen(const pp& pp, const filter_msk& msk, const str_vec& y);

    static bool dec(g1_vec& ct, g2_vec& sk);

private:
    static g1_vec enc(const pp& pp, const zp_vec& r, const zp_vec& x);

    static g2_vec keygen(const pp& pp, const zp_vec& r, const zp_vec& y);
};
