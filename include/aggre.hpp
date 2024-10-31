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
    /**
     * Generate the public parameters for FE with the Aggre functionality.
     * @param length the input length to the encryption algorithm.
     * @param pre whether we want to use the precomputed table for group exponentiation.
     * @return the generated public parameters.
     */
    static AggrePP pp_gen(int length, bool pre = true);

    /**
     * Generate master secret key.
     * @param pp the public parameters.
     * @return the generated master secret key.
     */
    static AggreMsk msk_gen(const AggrePP& pp);

    /**
     * Perform the Aggre FE encryption.
     * @param pp the public parameters.
     * @param msk the master secret key.
     * @param x a vector of integers.
     * @return the ciphertext.
     */
    static G1Vec enc(const AggrePP& pp, const AggreMsk& msk, const IntVec& x);

    /**
     * Perform the Aggre FE key generation.
     * @param pp the public parameters.
     * @param msk the master secret key.
     * @param y a vector of integers.
     * @param p an integer.
     * @return the function key.
     */
    static G2Vec keygen(const AggrePP& pp, const AggreMsk& msk, const IntVec& y, int p);

    /**
     * Perform the Aggre FE decryption.
     * @param ct the ciphertext.
     * @param sk the function key.
     * @return a boolean indicating the result of Aggre.
     */
    static bool dec(const G1Vec& ct, const G2Vec& sk);
};
