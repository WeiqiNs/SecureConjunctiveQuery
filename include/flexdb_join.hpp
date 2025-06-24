#pragma once

#include "crypto.hpp"

struct FlexDbEjPp{
    // Store the degree and input length.
    int d;
    int l;
    // Create a pointer to hold the bilinear pairing group object.
    std::unique_ptr<BP> pairing_group;
};

struct FlexDbEjMsk{
    Fp k;
    Fp d;
    Fp di;
    FpVec r;
    FpVec b;
    FpVec bi;
    int k_int;
    int d_int;
    int r_int;
    int b_int;
    bool compress;
    std::unique_ptr<HMAC> hmac;
};

class FlexDbEj{
public:
    /**
     * Generate the required public parameters.
     * @param degree the degree of the polynomial.
     * @param length the length of vectors to the encryption algorithm.
     * @param pre whether we want to use the precomputed table for group exponentiation.
     * @return the generated public parameters.
     */
    static FlexDbEjPp pp_gen(int degree, int length, bool pre = true);

    /**
     * Generate master secret key.
     * @param pp the public parameters.
     * @param key the HMAC key to use.
     * @param compress boolean to indicate whether to compress the private keys.
     * @return the generated master secret key.
     */
    static FlexDbEjMsk msk_gen(const FlexDbEjPp& pp, const CharVec& key = {}, const bool& compress = false);

    /**
     * Perform the Equal-Join encryption.
     * @param pp the public parameters.
     * @param msk the master secret key.
     * @param x a vector of strings or integers.
     * @param join_index indicate which position to check for equality.
     * @return the ciphertext.
     */
    static G1Vec enc(const FlexDbEjPp& pp, const FlexDbEjMsk& msk, const Vec& x, int join_index);

    /**
     * Perform the Equal-Join key generation with selecting columns.
     * @param pp the public parameters.
     * @param msk the master secret key.
     * @param y a vector or matrix of strings or integers. Use matrix only when you are selecting set of inputs.
     * @param sel a vector of integers indicating which columns to select, by default is empty.
     * @return 
     */
    static G2Vec keygen(const FlexDbEjPp& pp, const FlexDbEjMsk& msk, const VecOrMat& y, const IntVec& sel = {});

    /**
     * Perform the Equal-Join FE decryption with selecting columns.
     * @param pp the public parameters.
     * @param ct the ciphertext.
     * @param sk the function key.
     * @param sel a vector of integers indicating which columns to select, by default is empty.
     * @return a Gt element of the pairing, needs to be compared with another to determine whether join can happen.
     */
    static Gt dec(const FlexDbEjPp& pp, const G1Vec& ct, const G2Vec& sk, const IntVec& sel = {});
};
