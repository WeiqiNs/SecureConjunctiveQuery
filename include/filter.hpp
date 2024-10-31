#pragma once

#include "crypto.hpp"

struct pp{
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
    Hash hash;
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
    static pp pp_gen(int degree, int length, bool pre = true);

    /**
     * Generate master secret key.
     * @param pp the public parameters.
     * @return the generated master secret key.
     */
    static FilterMsk msk_gen(const pp& pp);

    /**
     * Perform the Filter FE encryption.
     * @param pp the public parameters.
     * @param msk the master secret key.
     * @param x a vector of strings or integers.
     * @return the ciphertext.
     */
    static G1Vec enc(const pp& pp, const FilterMsk& msk, const Vec& x);

    /**
     * Perform the Filter FE key generation.
     * @param pp the public parameters.
     * @param msk the master secret key.
     * @param y a vector or matrix of strings or integers. Use matrix only when you are selecting set of inputs.
     * @return the function key.
     */
    static G2Vec keygen(const pp& pp, const FilterMsk& msk, const VecOrMat& y);

    /**
     * Perform the Filter FE key generation with select selecting columns.
     * @param pp the public parameters.
     * @param msk the master secret key.
     * @param y a vector or matrix of strings or integers. Use matrix only when you are selecting set of inputs.
     * @param sel a vector of integers indicating which columns to select.
     * @return the function key.
     */
    static G2Vec keygen(const pp& pp, const FilterMsk& msk, const VecOrMat& y, const IntVec& sel);

    /**
     * Perform the Filter FE decryption.
     * @param ct the ciphertext.
     * @param sk the secret key.
     * @return a boolean indicating the result of Filter.
     */
    static bool dec(const G1Vec& ct, const G2Vec& sk);

    /**
     * Perform the Filter FE decryption with selecting columns.
     * @param pp the public parameters.
     * @param ct the ciphertext.
     * @param sk the secret key.
     * @param sel a vector of integers indicating which columns to select.
     * @return a boolean indicating the result of Filter.
     */
    static bool dec(const pp& pp, const G1Vec& ct, const G2Vec& sk, const IntVec& sel);
};
