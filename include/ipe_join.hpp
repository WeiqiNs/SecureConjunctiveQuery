#pragma once

#include "crypto.hpp"

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
    std::unique_ptr<HMAC> hmac;
};

class IpeJoin{
public:
    /**
     * Generate the required public parameters.
     * @param degree the degree of the polynomial.
     * @param length the length of vectors to the encryption algorithm.
     * @param pre whether we want to use the precomputed table for group exponentiation.
     * @return the generated public parameters.
     */
    static IpeJoinPP pp_gen(int degree, int length, bool pre = true);

    /**
     * Generate master secret key.
     * @param pp the public parameters.
     * @param key the HMAC key to use.
     * @return the generated master secret key.
     */
    static IpeJoinMsk msk_gen(const IpeJoinPP& pp, const CharVec& key = {});

    /**
     * Perform the Equal-Join FE encryption.
     * @param pp the public parameters.
     * @param msk the master secret key.
     * @param x a vector of strings or integers.
     * @param join_index indicate which position to check for equality.
     * @return the ciphertext.
     */
    static G1Vec enc(const IpeJoinPP& pp, const IpeJoinMsk& msk, const Vec& x, int join_index);

    /**
     * Perform the Equal-Join FE key generation.
     * @param pp the public parameters.
     * @param msk the master secret key.
     * @param y a matrix of strings or integers.
     * @return the function key.
     */
    static G2Vec keygen(const IpeJoinPP& pp, const IpeJoinMsk& msk, const Mat& y);

    /**
     * Perform the Equal-Join decryption.
     * @param ct the ciphertext.
     * @param sk the function key.
     * @return a Gt element of the pairing, needs to be compared with another to determine whether join can happen.
     */
    static Gt dec(const G1Vec& ct, const G2Vec& sk);
};
