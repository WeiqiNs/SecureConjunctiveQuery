#pragma once

#include <openssl/rand.h>
#include "helper.hpp"

static int HASH_SIZE = 16;

class Aes{
    vec key;
    int byte_length;

    /**
     * 
     * @return 
     */
    [[nodiscard]] const EVP_CIPHER* get_cipher() const;

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
    vec get_key();

    /**
     * 
     * @param plaintext 
     * @return 
     */
    [[nodiscard]] vec encrypt(const vec& plaintext) const;

    /**
     * 
     * @param ciphertext 
     * @return 
     */
    [[nodiscard]] vec decrypt(const vec& ciphertext) const;
};

class Hash{
    vec iv;
    vec key;

public:
    /**
     * 
     */
    Hash();


    /**
     * 
     * @return 
     */
    vec get_iv();

    /**
     * 
     * @return 
     */
    vec get_key();

    /**
     * 
     * @param data 
     * @return 
     */
    [[nodiscard]] vec digest(const vec& data) const;

    /**
     * 
     * @param x 
     * @return 
     */
    [[nodiscard]] zp_vec digest_int_vec(const int_vec& x) const;

    /**
     * 
     * @param x 
     * @return 
     */
    [[nodiscard]] zp_vec digest_str_vec(const str_vec& x) const;
};
