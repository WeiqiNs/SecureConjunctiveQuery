#pragma once

#include <openssl/rand.h>
#include "helper.hpp"

static int HASH_SIZE = 16;

class Aes{
    char_vec key;
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
    char_vec get_key();

    /**
     * 
     * @param plaintext 
     * @return 
     */
    [[nodiscard]] char_vec encrypt(const char_vec& plaintext) const;

    /**
     * 
     * @param ciphertext 
     * @return 
     */
    [[nodiscard]] char_vec decrypt(const char_vec& ciphertext) const;
};

class Hash{
    char_vec iv;
    char_vec key;

public:
    /**
     * 
     */
    Hash();


    /**
     * 
     * @return 
     */
    char_vec get_iv();

    /**
     * 
     * @return 
     */
    char_vec get_key();

    /**
     * 
     * @param data 
     * @return 
     */
    [[nodiscard]] char_vec digest(const char_vec& data) const;

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
