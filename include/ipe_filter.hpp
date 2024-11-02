#pragma once

#include "crypto.hpp"

struct IpeFilterPP{
    // Suppose by default the degree is 1.
    int d = 1;
    // Suppose by default the length is 1.
    int l = 1;
    // Create a pointer to hold the bilinear pairing group object.
    std::unique_ptr<BP> pairing_group;
};

struct IpeFilterMsk{
    FpMat b;
    FpMat bi;
    Hash hash;
};

class IpeFilter{
public:
    static IpeFilterPP pp_gen(int degree, int length, bool pre = true);

    static IpeFilterMsk msk_gen(const IpeFilterPP& pp);

    static G1Vec enc(const IpeFilterPP& pp, const IpeFilterMsk& msk, const Vec& x);

    static G2Vec keygen(const IpeFilterPP& pp, const IpeFilterMsk& msk, const Mat& y);

    static bool dec(const G1Vec& ct, const G2Vec& sk);
};
