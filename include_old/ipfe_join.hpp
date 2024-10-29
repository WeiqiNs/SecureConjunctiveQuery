#pragma once

#include "ipfe_filter.hpp"

struct ipfe_join_msk{
    zp k;
    zp det;
    zp_mat b;
    zp_mat bi;
    Hash hash;
};

class IpfeJoin{
public:
    static pp pp_gen(const int& degree, const bool& pre = true);

    static ipfe_join_msk msk_gen(const pp& pp, const int& input_len);

    static g1_vec enc(const pp& pp, const ipfe_join_msk& msk, const int_vec& x);

    static g1_vec enc(const pp& pp, const ipfe_join_msk& msk, const str_vec& x);

    static g2_vec keygen(const pp& pp, const ipfe_join_msk& msk, const int_mat& y);

    static g2_vec keygen(const pp& pp, const ipfe_join_msk& msk, const str_mat& y);

    static void dec(gt& r, const g1_vec& ct, const g2_vec& sk);

private:
    static g1_vec enc(const pp& pp, const ipfe_join_msk& msk, const zp_vec& x);

    static g2_vec keygen(const pp& pp, const ipfe_join_msk& msk, const zp_mat& y);
};
