#pragma once

#include "crypto.hpp"

struct IpeFilterPp{
    // Store the degree and input length.
    int d;
    int l;
    // Create a pointer to hold the bilinear pairing group object.
    std::unique_ptr<BP> pairing_group;
};

struct IpeFilterMsk{
    FpMat b;
    FpMat bi;
    std::unique_ptr<HMAC> hmac;
};

class IpeFilter{
public:
    /**
     * Generate the required public parameters.
     * @param degree the degree of the polynomial.
     * @param length the length of vectors to the encryption algorithm.
     * @param pre whether we want to use the precomputed table for group exponentiation.
     * @return the generated public parameters.
     */
    static IpeFilterPp pp_gen(int degree, int length, bool pre = true);

    /**
     * Generate master secret key.
     * @param pp the public parameters.
     * @param key the HMAC key to use.
     * @return the generated master secret key.
     */
    static IpeFilterMsk msk_gen(const IpeFilterPp& pp, const CharVec& key = {});

    /**
     * Perform the Filter FE encryption.
     * @param pp the public parameters.
     * @param msk the master secret key.
     * @param x a vector of strings or integers.
     * @return the ciphertext.
     */
    static G1Vec enc(const IpeFilterPp& pp, const IpeFilterMsk& msk, const Vec& x);

    /**
     * Perform the Filter FE key generation.
     * @param pp the public parameters.
     * @param msk the master secret key.
     * @param y a vector or matrix of strings or integers. Use matrix only when you are selecting set of inputs.
     * @return the function key.
     */
    static G2Vec keygen(const IpeFilterPp& pp, const IpeFilterMsk& msk, const Mat& y);

    /**
     * Perform the Filter FE decryption.
     * @param ct the ciphertext.
     * @param sk the function key.
     * @return a boolean indicating the result of Filter.
     */
    static bool dec(const G1Vec& ct, const G2Vec& sk);
};
