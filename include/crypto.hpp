#pragma once

#include <openssl/core_names.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include "helper.hpp"

class AES{
public:
    /**
     * The default constructor for the AES class.
     * @param key_length the number of bits in the AES key.
     */
    explicit AES(const int& key_length = 256);

    /**
     * Initialize the AES class with a given key.
     * @param key some char vector, should have required size.
     */
    explicit AES(const CharVec& key);

    /// Deconstructor for AES class.
    ~AES();

    /// Get the key used for AES encryption.
    CharVec get_key();

    /// Set the key used for AES encryption.
    void set_key(const CharVec& key);

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
    CharVec key_;
    EVP_CIPHER_CTX* ctx_;
    const EVP_CIPHER* cipher_;

    /// Select proper AES object to use base on the key length.
    [[nodiscard]] static const EVP_CIPHER* get_cipher(size_t byte_length);
};


class PRF{
public:
    /**
     * Initialize the PRF class with a given key.
     * @param key some char vector, should have required size.
     */
    explicit PRF(const CharVec& key = {});

    /// Deconstructor for PRF class.
    ~PRF();

    /// Get the key used for PRF digestion.
    CharVec get_key();

    /// Set the key used for PRF digestion.
    void set_key(const CharVec& key);

    /**
     * Perform PRF digestion.
     * @param data a vector of unsigned characters.
     * @return a vector of unsigned characters.
     */
    [[nodiscard]] CharVec digest(const CharVec& data) const;

private:
    /// Declare variables used for HMAC.
    CharVec key_;
    EVP_MAC* mac_;
    EVP_MAC_CTX* ctx_;
    /// Define the digest that we will use.
    std::string cipher_ = "AES-256-CBC";
};


class HMAC{
public:
    /// Constructor for HMAC class.
    explicit HMAC(const CharVec& key = {});

    /// Deconstructor for HMAC class.
    ~HMAC();

    /// Get the key used for AES encryption.
    CharVec get_key();

    /// Set the key used for AES encryption.
    void set_key(const CharVec& key);

    /**
     * Perform the hash digestion.
     * @param data a vector of unsigned characters.
     * @return a vector of unsigned characters.
     */
    [[nodiscard]] CharVec digest(const CharVec& data) const;


    /**
     * Digest a string or an integer vector to a vector of field element.
     * @param x a string or an integer vector.
     * @param sel an integer vector to indicate the index of input vector.
     * @return a vector of field elements.
     */
    [[nodiscard]] FpVec digest_vec_to_fp(const Vec& x, const IntVec& sel = {}) const;

    /**
     * Digest a string or an integer matrix to a vector of field element.
     * @param x a string or an integer matrix.
     * @param sel an integer vector to indicate the index of input matrix.
     * @return a matrix of field elements.
     */
    [[nodiscard]] FpMat digest_mat_to_fp(const Mat& x, const IntVec& sel = {}) const;

    /**
     * Given an integer, generate len number of random field number.
     * @param x the starting integer.
     * @param len the number of desired random values to be generated.
     * @return len number of random field numbers.
     */
    [[nodiscard]] FpVec digest_int_to_fp_vec(const int& x, const int& len) const;

    /**
     * Digest a string or an integer vector to a vector of field element (in the generated group in pp).
     * @param pairing_group a bilinear pairing group object.
     * @param x a string or an integer vector.
     * @param sel an integer vector to indicate the index of input vector.
     * @return a vector of field elements.
     */
    [[nodiscard]] FpVec digest_vec_to_fp_mod(const BP& pairing_group, const Vec& x, const IntVec& sel = {}) const;

    /**
     * Digest a string or an integer matrix to a vector of field element (in the generated group in pp).
     * @param pairing_group a bilinear pairing group object.
     * @param x a string or an integer matrix.
     * @param sel an integer vector to indicate the index of input matrix.
     * @return a vector of field elements.
     */
    [[nodiscard]] FpMat digest_mat_to_fp_mod(const BP& pairing_group, const Mat& x, const IntVec& sel = {}) const;

    /**
     * Given an integer, generate len number of random field number.
     * @param pairing_group a bilinear pairing group object.
     * @param x the starting integer.
     * @param len the number of desired random values to be generated.
     * @return len number of random field numbers.
     */
    [[nodiscard]] FpVec digest_int_to_fp_vec_mod(const BP& pairing_group, const int& x, const int& len) const;

private:
    /// Declare variables used for HMAC.
    CharVec key_;
    EVP_MAC* mac_;
    EVP_MAC_CTX* ctx_;
    /// Define the digest that we will use.
    std::string digest_ = "BLAKE2B-512";
};
