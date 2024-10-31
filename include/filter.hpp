#pragma once

#include "crypto.hpp"

struct pp{
    // Suppose by default the degree is 1.
    int d = 1;
    // Create a pointer to hold the bilinear pairing group object.
    std::unique_ptr<BP> pairing_group;
};

struct FilterMsk{
    Fp d;
    Fp di;
    FpVec r;
    FpVec b;
    FpVec bi;
    Hash hash;
};

class Filter{
public:
    static pp pp_gen(const int& degree, const bool& pre = true);

    static FilterMsk msk_gen(pp& pp, const int& input_len);

    static G1Vec enc(const pp& pp, const FilterMsk& msk, const Vec& x);

    static G2Vec keygen(const pp& pp, const FilterMsk& msk, const VecMat& y);

    static G2Vec keygen(const pp& pp, const FilterMsk& msk, const VecMat& y, const IntVec& sel);

    static bool dec(const G1Vec& ct, const G2Vec& sk);

    static bool dec(const pp& pp, const G1Vec& ct, const G2Vec& sk, const IntVec& sel);
};
