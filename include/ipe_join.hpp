#pragma once

#include "crypto.hpp"
#include "ipe_join.hpp"


struct IpeJoinPP{
    // Suppose by default the degree is 1.
    int d = 1;
    // Suppose by default the length is 1.
    int l = 1;
    // Create a pointer to hold the bilinear pairing group object.
    std::unique_ptr<BP> pairing_group;
};

struct IpeJoinMsk{
    Fp k;
    FpMat b;
    FpMat bi;
    Hash hash;
};

class IpeJoin{
public:
    static IpeJoinPP pp_gen(int degree, int length, bool pre = true);

    static IpeJoinMsk msk_gen(const IpeJoinPP& pp);

    static G1Vec enc(const IpeJoinPP& pp, const IpeJoinMsk& msk, const Vec& x, int join_index);

    static G2Vec keygen(const IpeJoinPP& pp, const IpeJoinMsk& msk, const Mat& y);

    static Gt dec(const G1Vec& ct, const G2Vec& sk);
};
