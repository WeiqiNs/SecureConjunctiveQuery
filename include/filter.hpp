#pragma once

#include "group.hpp"
#include "crypto.hpp"

struct pp{
    int d{};
    Field field_zp;
    Group group_bp;
};

struct filter_msk{
    zp d;
    zp di;
    zp_vec r;
    zp_vec b;
    zp_vec bi;
    Hash hash;
};

class Filter{
public:
    static pp pp_gen(const int& degree, const bool& pre = true);

    static filter_msk msk_gen(const pp& pp, const int& input_len);

    static zp_vec poly_msg(const pp& pp, const zp_vec& x);

    static zp_vec poly_key(const pp& pp, const zp_mat& y);

    static g1_vec enc(const pp& pp, const filter_msk& msk, const int_vec& x);

    static g1_vec enc(const pp& pp, const filter_msk& msk, const str_vec& x);

    static g2_vec keygen(const pp& pp, const filter_msk& msk, const int_mat& y);

    static g2_vec keygen(const pp& pp, const filter_msk& msk, const str_mat& y);

    static g2_vec keygen(const pp& pp, const filter_msk& msk, const int_mat& y, const int_vec& sel);

    static g2_vec keygen(const pp& pp, const filter_msk& msk, const str_mat& y, const int_vec& sel);

    static bool dec(g1_vec& ct, g2_vec& sk);

    static bool dec(const pp& pp, g1_vec& ct, g2_vec& sk, const int_vec& sel);

private:
    static g1_vec enc(const pp& pp, const filter_msk& msk, const zp_vec& x);

    static g2_vec keygen(const pp& pp, const filter_msk& msk, const zp_mat& y);

    static g2_vec keygen(const pp& pp, const filter_msk& msk, const zp_mat& y, const int_vec& sel);

};
