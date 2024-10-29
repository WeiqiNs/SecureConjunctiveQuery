#pragma once

#include "crypto.hpp"

struct pp{
    int d = 0;
    BP* pairing_group = nullptr;
};

struct FilterMsk{
    Fp d;
    Fp di;
    FpVec r;
    FpVec rp;
    FpVec b;
    FpVec bi;
    FpVec bp;
    FpVec bpi;
    Hash hash;
};

class Filter{
public:
    static pp pp_gen(const int& degree, const bool& pre = true);

    static FilterMsk msk_gen(const pp& pp, const int& input_len);

    static G1Vec enc(const pp& pp, const FilterMsk& msk, const IntVec& x);

    static G1Vec enc(const pp& pp, const FilterMsk& msk, const StrVec& x);

    static G1Vec enc(const pp& pp, const FilterMsk& msk, const FpVec& x);


    //
    // static zp_vec poly_msg(const pp& pp, const zp_vec& x);
    //
    // static zp_vec poly_key(const pp& pp, const zp_mat& y);


    // static g2_vec keygen(const pp& pp, const FilterMsk& msk, const int_mat& y);

    // static g2_vec keygen(const pp& pp, const FilterMsk& msk, const str_mat& y);

    // static g2_vec keygen(const pp& pp, const FilterMsk& msk, const int_mat& y, const int_vec& sel);
    //
    // static g2_vec keygen(const pp& pp, const FilterMsk& msk, const str_mat& y, const int_vec& sel);

    // static bool dec(g1_vec& ct, g2_vec& sk);

    // static bool dec(const pp& pp, g1_vec& ct, g2_vec& sk, const int_vec& sel);



    // static g2_vec keygen(const pp& pp, const FilterMsk& msk, const zp_mat& y);

    // static g2_vec keygen(const pp& pp, const FilterMsk& msk, const zp_mat& y, const int_vec& sel);

};
