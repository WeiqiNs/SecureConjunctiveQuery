#pragma once

#include "crypto.hpp"

struct FlexDbSfPp{
    // Store the input length.
    int l;
    // Create a pointer to hold the bilinear pairing group object.
    std::unique_ptr<BP> pairing_group;
};

struct FlexDbSfMsk{
    Fp d;
    Fp di;
    FpVec b;
    FpVec r;
    int d_int;
    int b_int;
    int r_int;
    bool compress;
    std::unique_ptr<HMAC> hmac;
};


class FlexDbSf{
public:
    /**
     * Generate the required public parameters.
     * @param length the length of vectors to the encryption algorithm.
     * @param pre whether we want to use the precomputed table for group exponentiation.
     * @return the generated public parameters.
     */
    static FlexDbSfPp pp_gen(int length, bool pre = true);

    /**
     * Generate master secret key.
     * @param pp the public parameters.
     * @param key the HMAC key to use.
     * @param compress boolean to indicate whether to compress the private keys.
     * @return the generated master secret key.
     */
    static FlexDbSfMsk msk_gen(const FlexDbSfPp& pp, const CharVec& key = {}, const bool& compress = false);

    /**
     * Perform the Filter FE encryption.
     * @param pp the public parameters.
     * @param msk the master secret key.
     * @param x a vector of strings or integers.
     * @return the ciphertext.
     */
    static G1Vec enc(const FlexDbSfPp& pp, const FlexDbSfMsk& msk, const Vec& x);

    /**
     * Perform the Filter FE key generation with select selecting columns.
     * @param pp the public parameters.
     * @param msk the master secret key.
     * @param y a vector of strings or integers.
     * @param sel a vector of integers indicating which columns to select, by default is empty.
     * @return the function key.
     */
    static G2Vec keygen(const FlexDbSfPp& pp, const FlexDbSfMsk& msk, const Vec& y, const IntVec& sel = {});

    /**
     * Perform the Filter FE decryption with selecting columns.
     * @param ct the ciphertext.
     * @param sk the function key.
     * @param sel a vector of integers indicating which columns to select, by default is empty.
     * @return a boolean indicating the result of Filter.
     */
    static bool dec(const G1Vec& ct, const G2Vec& sk, const IntVec& sel = {});
};
