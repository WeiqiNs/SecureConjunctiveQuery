#pragma once

#include "crypto.hpp"

struct JoinPP{
    // Suppose by default the degree is 1.
    int d = 1;
    // Suppose by default the length is 1.
    int l = 1;
    // Create a pointer to hold the bilinear pairing group object.
    std::unique_ptr<BP> pairing_group;
};

struct JoinMsk{
    Fp k;
    Fp l;
    Fp d;
    Fp di;
    FpVec r;
    FpVec b;
    FpVec bi;
    Hash hash;
};

class Join {
public:
    static JoinPP pp_gen(int degree, int length, bool pre = true);

    static JoinMsk msk_gen(const JoinPP& pp);

    static G1Vec enc(const JoinPP& pp, const JoinMsk& msk, const Vec& x, int join_index);

    static G2Vec keygen(const JoinPP& pp, const JoinMsk& msk, const VecOrMat& y);

    static G2Vec keygen(const JoinPP& pp, const JoinMsk& msk, const VecOrMat& y, const IntVec& sel);

    static Gt dec(const G1Vec& ct, const G2Vec& sk);

    static Gt dec(const JoinPP& pp, const G1Vec& ct, const G2Vec& sk, const IntVec& sel);
};
