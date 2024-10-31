#pragma once

#include "helper.hpp"

struct AggrePP{
    // Suppose by default the length is 1.
    int l = 1;
    // Create a pointer to hold the bilinear pairing group object.
    std::unique_ptr<BP> pairing_group;
};

struct AggreMsk{
    Fp d;
    Fp di;
    FpVec r;
    FpVec b;
    FpVec bi;
};

class Aggre{
public:
    static AggrePP pp_gen(int length, bool pre = true);

    static AggreMsk msk_gen(const AggrePP& pp);

    static G1Vec enc(const AggrePP& pp, const AggreMsk& msk, const IntVec& x);

    static G2Vec keygen(const AggrePP& pp, const AggreMsk& msk, const IntVec& y, int p);

    static bool dec(const G1Vec& ct, const G2Vec& sk);
};
