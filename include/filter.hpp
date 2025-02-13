#pragma once

#include "crypto.hpp"

struct FilterPP{
    // Suppose by default the degree is 1.
    int d = 1;
    // Suppose by default the length is 1.
    int l = 1;
    // Create a pointer to hold the bilinear pairing group object.
    std::unique_ptr<BP> pairing_group;
};

struct FilterMsk{
    Fp d;
    Fp di;
    FpVec r;
    FpVec b;
    FpVec bi;
    std::unique_ptr<HMAC> hmac;
};

class Filter{
public:
    /**
     * Generate the required public parameters.
     * @param degree the degree of the polynomial.
     * @param length the length of vectors to the encryption algorithm.
     * @param pre whether we want to use the precomputed table for group exponentiation.
     * @return the generated public parameters.
     */
    static FilterPP pp_gen(int degree, int length, bool pre = true);

    /**
     * Generate master secret key.
     * @param pp the public parameters.
     * @param key the HMAC key to use.
     * @return the generated master secret key.
     */
    static FilterMsk msk_gen(const FilterPP& pp, const CharVec& key = {});

    /**
     * Perform the Filter FE encryption.
     * @param pp the public parameters.
     * @param msk the master secret key.
     * @param x a vector of strings or integers.
     * @return the ciphertext.
     */
    static G1Vec enc(const FilterPP& pp, const FilterMsk& msk, const Vec& x);

    /**
     * Perform the Filter FE key generation with select selecting columns.
     * @param pp the public parameters.
     * @param msk the master secret key.
     * @param y a vector or matrix of strings or integers. Use matrix only when you are selecting set of inputs.
     * @param sel a vector of integers indicating which columns to select, by default is empty.
     * @return the function key.
     */
    static G2Vec keygen(const FilterPP& pp, const FilterMsk& msk, const VecOrMat& y, const IntVec& sel = {});

    /**
     * Perform the Filter FE decryption with selecting columns.
     * @param pp the public parameters.
     * @param ct the ciphertext.
     * @param sk the function key.
     * @param sel a vector of integers indicating which columns to select, by default is empty.
     * @return a boolean indicating the result of Filter.
     */
    static bool dec(const FilterPP& pp, const G1Vec& ct, const G2Vec& sk, const IntVec& sel = {});
};
