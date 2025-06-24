#pragma once

#include "crypto.hpp"

struct FlexDbEtPp{
    // Store the input length.
    int l;
    // Create a pointer to hold the bilinear pairing group object.
    std::unique_ptr<BP> pairing_group;
};

struct FlexDbEtMsk{
    Fp d;
    Fp di;
    FpVec r;
    FpVec b;
    FpVec bi;
    int d_int;
    int r_int;
    int b_int;
    bool compress;
    std::unique_ptr<HMAC> hmac;
};

class FlexDbEt{
public:
    /**
     * Generate the public parameters for FE with the Aggre functionality.
     * @param length the input length to the encryption algorithm.
     * @param pre whether we want to use the precomputed table for group exponentiation.
     * @return the generated public parameters.
     */
    static FlexDbEtPp pp_gen(int length, bool pre = true);

    /**
     * Generate master secret key.
     * @param pp the public parameters.
    * @param key the HMAC key to use.
     * @param compress boolean to indicate whether to compress the private keys.
     * @return the generated master secret key.
     */
    static FlexDbEtMsk msk_gen(const FlexDbEtPp& pp, const CharVec& key = {}, const bool& compress = false);

    /**
     * Perform the Aggre FE encryption.
     * @param pp the public parameters.
     * @param msk the master secret key.
     * @param x a vector of integers.
     * @return the ciphertext.
     */
    static G1Vec enc(const FlexDbEtPp& pp, const FlexDbEtMsk& msk, const IntVec& x);

    /**
     * Perform the Aggre FE key generation.
     * @param pp the public parameters.
     * @param msk the master secret key.
     * @param y a vector of integers.
     * @param p an integer.
     * @param sel a vector of integers indicating which columns to select, by default is empty.
     * @return the function key.
     */
    static G2Vec keygen(const FlexDbEtPp& pp, const FlexDbEtMsk& msk, const IntVec& y, int p, const IntVec& sel = {});

    /**
     * Perform the Aggre FE decryption.
     * @param ct the ciphertext.
     * @param sk the function key.
     * @param sel a vector of integers indicating which columns to select, by default is empty.
     * @return a boolean indicating the result of Aggre.
     */
    static bool dec(const G1Vec& ct, const G2Vec& sk, const IntVec& sel = {});
};
