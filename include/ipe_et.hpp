#pragma once

#include "helper.hpp"

struct IpeEtPp{
    // Store the input length.
    int l;
    // Create a pointer to hold the bilinear pairing group object.
    std::unique_ptr<BP> pairing_group;
};

struct IpeEtMsk{
    FpMat b;
    FpMat bi;
};


class IpeEt{
public:
    /**
     * Generate the required public parameters.
     * @param length the length of vectors to the encryption algorithm.
     * @param pre whether we want to use the precomputed table for group exponentiation.
     * @return the generated public parameters.
     */
    static IpeEtPp pp_gen(int length, bool pre = true);

    /**
     * Generate master secret key.
     * @param pp the public parameters.
     * @return the generated master secret key.
     */
    static IpeEtMsk msk_gen(const IpeEtPp& pp);

    /**
     * Perform the Aggre FE encryption.
     * @param pp the public parameters.
     * @param msk the master secret key.
     * @param x a vector of integers.
     * @return the ciphertext.
     */
    static G1Vec enc(const IpeEtPp& pp, const IpeEtMsk& msk, const IntVec& x);

    /**
     * Perform the Aggre FE key generation.
     * @param pp the public parameters.
     * @param msk the master secret key.
     * @param y a vector of integers.
     * @param p an integer.
     * @return the function key.
     */
    static G2Vec keygen(const IpeEtPp& pp, const IpeEtMsk& msk, const IntVec& y, int p);

    /**
     * Perform the Aggre FE decryption.
     * @param ct the ciphertext.
     * @param sk the function key.
     * @return a boolean indicating the result of Aggre.
     */
    static bool dec(const G1Vec& ct, const G2Vec& sk);
};
