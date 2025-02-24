#pragma once

#include "crypto.hpp"

struct SseFilterMsk{
    int counter;
    std::unique_ptr<PRF> prf;
};

class SseFilter{
public:
    /**
     * Perform the SSE filter master secret key generation.
     * @param key a vector of characters.
     * @return the SSE filter master secret key instance.
     */
    static SseFilterMsk msk_gen(const CharVec& key = {});

    /**
     * Perform the Filter FE encryption.
     * @param msk the master secret key.
     * @param x a vector of strings or integers.
     * @return the ciphertext.
     */
    static CharMat enc(SseFilterMsk& msk, const Vec& x);

    /**
     * Perform the Filter FE key generation.
     * @param msk the master secret key.
     * @param y a vector of strings or integers.
     * @param row the number of rows to select from.
     * @return the function key.
     */
    static CharMat keygen(const SseFilterMsk& msk, const Vec& y, int row);

    /**
     * Perform the Filter FE decryption.
     * @param ct the ciphertext.
     * @param sk the function key.
     * @return a boolean indicating the result of Filter.
     */
    static bool dec(const CharMat& ct, const CharMat& sk);

private:
    /// Make unique pointer for the PRF.
    std::unique_ptr<PRF> prf_;
};
