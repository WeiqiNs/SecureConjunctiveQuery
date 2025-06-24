#pragma once

#include "crypto.hpp"

struct IpeEjPp{
    // Store the degree and input length.
    int d;
    int l;
    // Create a pointer to hold the bilinear pairing group object.
    std::unique_ptr<BP> pairing_group;
};

struct IpeEjMsk{
    Fp k;
    FpMat b;
    FpMat bi;
    std::unique_ptr<HMAC> hmac;
};

class IpeEj{
public:
    /**
     * Generate the required public parameters.
     * @param degree the degree of the polynomial.
     * @param length the length of vectors to the encryption algorithm.
     * @param pre whether we want to use the precomputed table for group exponentiation.
     * @return the generated public parameters.
     */
    static IpeEjPp pp_gen(int degree, int length, bool pre = true);

    /**
     * Generate master secret key.
     * @param pp the public parameters.
     * @param key the HMAC key to use.
     * @return the generated master secret key.
     */
    static IpeEjMsk msk_gen(const IpeEjPp& pp, const CharVec& key = {});

    /**
     * Perform the Equal-Join FE encryption.
     * @param pp the public parameters.
     * @param msk the master secret key.
     * @param x a vector of strings or integers.
     * @param join_index indicate which position to check for equality.
     * @return the ciphertext.
     */
    static G1Vec enc(const IpeEjPp& pp, const IpeEjMsk& msk, const Vec& x, int join_index);

    /**
     * Perform the Equal-Join FE key generation.
     * @param pp the public parameters.
     * @param msk the master secret key.
     * @param y a matrix of strings or integers.
     * @return the function key.
     */
    static G2Vec keygen(const IpeEjPp& pp, const IpeEjMsk& msk, const Mat& y);

    /**
     * Perform the Equal-Join decryption.
     * @param ct the ciphertext.
     * @param sk the function key.
     * @return a Gt element of the pairing, needs to be compared with another to determine whether join can happen.
     */
    static Gt dec(const G1Vec& ct, const G2Vec& sk);
};
