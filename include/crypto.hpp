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
     * The default constructor for the AES class.
     * @param key_length the number of bits in the AES key.
     */
    explicit Aes(const int& key_length);

    /// Get the key used for AES encryption.
    CharVec get_key();

    /**
     * Perform AES encryption.
     * @param plaintext a vector of unsigned characters.
     * @return a vector of unsigned characters.
     */
    [[nodiscard]] CharVec encrypt(const CharVec& plaintext) const;

    /**
     * Perform AES decryption.
     * @param ciphertext a vector of unsigned characters.
     * @return a vector of unsigned characters.
     */
    [[nodiscard]] CharVec decrypt(const CharVec& ciphertext) const;

private:
    /// Variables to hold the key and its byte length.
    CharVec key;
    int byte_length;

    /// Select proper AES object to use base on the key length.
    [[nodiscard]] const EVP_CIPHER* get_cipher() const;
};

class Hash{
public:
    /// Default constructor of the hash function, note we use AES128CBC.
    Hash();


    /// Return the IV used. (IV is fixed so that hash can be deterministic.)
    CharVec get_iv();

    /// Return the key used.
    CharVec get_key();

    /**
     * Perform the hash digestion.
     * @param data a vector of unsigned characters.
     * @return a vector of unsigned characters.
     */
    [[nodiscard]] CharVec digest(const CharVec& data) const;

    /**
     * Hash a vector of integers to field elements.
     * @param x a vector of integers.
     * @return a vector of field elements.
     */
    [[nodiscard]] FpVec digest_int_vec(const IntVec& x) const;

    /**
     * Hash a vector of strings to field elements.
     * @param x a vector of strings.
     * @return a vector of field elements.
     */
    [[nodiscard]] FpVec digest_str_vec(const StrVec& x) const;

    /**
     * Digest a string or an integer vector to a vector of field element.
     * @param pairing_group a bilinear pairing group object.
     * @param x a string or an integer vector.
     * @return a vector of field elements.
     */
    [[nodiscard]] FpVec digest_vec_to_fp(const BP& pairing_group, const Vec& x) const;

    /**
     * Digest a string or an integer matrix to a vector of field element.
     * @param pairing_group a bilinear pairing group object.
     * @param x a string or an integer matrix.
     * @return a vector of field elements.
     */
    [[nodiscard]] FpMat digest_mat_to_fp(const BP& pairing_group, const Mat& x) const;

private:
    /// Vectors of characters to hold the IV and the key.
    CharVec iv;
    CharVec key;
};
