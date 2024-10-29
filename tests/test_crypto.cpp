#include <gtest/gtest.h>
#include "crypto.hpp"

TEST(CryptoTests, AES128){
    const auto aes = Aes(128);

    const str plaintext = "This is a secret message.";

    const CharVec plaintext_vec = Helper::str_to_char_vec(plaintext);

    const CharVec ciphertext = aes.encrypt(plaintext_vec);

    EXPECT_EQ(aes.decrypt(ciphertext), plaintext_vec);
}

TEST(CryptoTests, AES256){
    const auto aes = Aes(256);

    const str plaintext = "This is another secret message.";

    const CharVec plaintext_vec = Helper::str_to_char_vec(plaintext);

    const CharVec ciphertext = aes.encrypt(plaintext_vec);

    EXPECT_EQ(aes.decrypt(ciphertext), plaintext_vec);
}

TEST(CryptoTests, Hash){
    const auto hash = Hash();

    const CharVec data_vec = Helper::str_to_char_vec("Something to hash.");

    const CharVec hash_one = hash.digest(data_vec);

    const CharVec hash_two = hash.digest(data_vec);

    EXPECT_EQ(hash_one, hash_two);
}
