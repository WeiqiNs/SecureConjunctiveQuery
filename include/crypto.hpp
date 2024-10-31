#pragma once

#include <variant>
#include <openssl/rand.h>
#include "helper.hpp"

// Set the hash output size to be 128 bits.
static int HASH_SIZE = 16;

// Set name alias for the commonly used type.
using Vec = std::variant<IntVec, StrVec>;
using Mat = std::variant<IntMat, StrMat>;
using VecMat = std::variant<IntVec, StrVec, IntMat, StrMat>;

class Aes{
public:
    /**
     * 
     * @param key_length 
     */
    explicit Aes(const int& key_length);

    /**
     * 
     * @return 
     */
    CharVec get_key();

    /**
     * 
     * @param plaintext 
     * @return 
     */
    [[nodiscard]] CharVec encrypt(const CharVec& plaintext) const;

    /**
     * 
     * @param ciphertext 
     * @return 
     */
    [[nodiscard]] CharVec decrypt(const CharVec& ciphertext) const;

private:
    /// Variables to hold the key and its byte length.
    CharVec key;
    int byte_length;

    /**
     *
     * @return
     */
    [[nodiscard]] const EVP_CIPHER* get_cipher() const;
};

class Hash{
public:
    /**
     * 
     */
    Hash();


    /**
     * 
     * @return 
     */
    CharVec get_iv();

    /**
     * 
     * @return 
     */
    CharVec get_key();

    /**
     * 
     * @param data 
     * @return 
     */
    [[nodiscard]] CharVec digest(const CharVec& data) const;

    /**
     * 
     * @param x 
     * @return 
     */
    [[nodiscard]] FpVec digest_int_vec(const IntVec& x) const;

    /**
     * 
     * @param x 
     * @return 
     */
    [[nodiscard]] FpVec digest_str_vec(const StrVec& x) const;

    /**
     *
     * @param pairing_group
     * @param x
     * @return
     */
    [[nodiscard]] FpVec digest_vec_to_fp(const BP& pairing_group, const Vec& x) const;

    /**
     *
     * @param pairing_group
     * @param x
     * @return
     */
    [[nodiscard]] FpMat digest_mat_to_fp(const BP& pairing_group, const Mat& x) const;

private:
    /// Vectors of characters to hold the IV and the key.
    CharVec iv;
    CharVec key;
};
