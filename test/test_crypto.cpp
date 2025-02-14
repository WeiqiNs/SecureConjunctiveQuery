#include <gtest/gtest.h>
#include "crypto.hpp"

TEST(CryptoTests, AES128){
    const auto aes = AES(128);

    const str plaintext = "This is a secret message.";

    const CharVec plaintext_vec = Helper::str_to_char_vec(plaintext);

    const CharVec ciphertext = aes.encrypt(plaintext_vec);

    EXPECT_EQ(aes.decrypt(ciphertext), plaintext_vec);
}

TEST(CryptoTests, AES256){
    const auto aes = AES();

    const str plaintext = "This is another secret message.";

    const CharVec plaintext_vec = Helper::str_to_char_vec(plaintext);

    const CharVec ciphertext = aes.encrypt(plaintext_vec);

    EXPECT_EQ(aes.decrypt(ciphertext), plaintext_vec);
}

TEST(CryptoTests, PRFEq){
    // Create a HMAC object and a testing string.
    const auto prf = PRF();
    const CharVec data_vec = Helper::str_to_char_vec("Something to hash.");

    // Two digests should be the same.
    EXPECT_EQ(prf.digest(data_vec), prf.digest(data_vec));
    // Double check digest length.
    EXPECT_EQ(prf.digest(data_vec).size(), 32);
}

TEST(CryptoTests, PRFNeq){
    // Create two keys.
    const CharVec key_1(32, '0');
    const CharVec key_2(32, '1');

    // Get two HMAC, they will be using different keys.
    const auto prf_1 = PRF(key_1);
    const auto prf_2 = PRF(key_2);
    const CharVec data_vec = Helper::str_to_char_vec("Something to hash.");

    // The digest should be not equal as keys are not equal.
    EXPECT_NE(prf_1.digest(data_vec), prf_2.digest(data_vec));
}

TEST(CryptoTests, HMACEq){
    // Create a HMAC object and a testing string.
    const auto hmac = HMAC();
    const CharVec data_vec = Helper::str_to_char_vec("Something to hash.");

    // Two digests should be the same.
    EXPECT_EQ(hmac.digest(data_vec), hmac.digest(data_vec));
    // Double check digest length.
    EXPECT_EQ(hmac.digest(data_vec).size(), 64);
}

TEST(CryptoTests, HMACNeq){
    // Create two keys.
    const CharVec key_1(64, '0');
    const CharVec key_2(64, '1');

    // Get two HMAC, they will be using different keys.
    const auto hmac_1 = HMAC(key_1);
    const auto hmac_2 = HMAC(key_2);
    const CharVec data_vec = Helper::str_to_char_vec("Something to hash.");

    // The digest should be not equal as keys are not equal.
    EXPECT_NE(hmac_1.digest(data_vec), hmac_2.digest(data_vec));
}

TEST(CryptoTests, HashIntVec){
    // Create a HMAC object and a testing vector.
    const auto hmac = HMAC();
    const IntVec x{0, 1, 2, 3, 4, 5};

    // Hash the vector without sel.
    const auto hx = hmac.digest_vec_to_fp(x);

    // Perform testing.
    EXPECT_TRUE(Field::cmp(hx[0], Helper::char_vec_to_fp(hmac.digest(Helper::str_to_char_vec("00")))));
    EXPECT_TRUE(Field::cmp(hx[5], Helper::char_vec_to_fp(hmac.digest(Helper::str_to_char_vec("55")))));

    // Random number test.
    EXPECT_FALSE(Field::cmp(hx[1], Helper::char_vec_to_fp(hmac.digest(Helper::str_to_char_vec("66")))));
}

TEST(CryptoTests, HashIntVecSel){
    // Create a HMAC object and a testing vector.
    const auto hmac = HMAC();
    const IntVec x{1, 2, 3, 4, 5};
    const IntVec sel{2, 4, 6, 8, 10};

    // Hash the vector without sel.
    const auto hx = hmac.digest_vec_to_fp(x, sel);

    // Perform testing.
    EXPECT_TRUE(Field::cmp(hx[0], Helper::char_vec_to_fp(hmac.digest(Helper::str_to_char_vec("12")))));
    EXPECT_TRUE(Field::cmp(hx[4], Helper::char_vec_to_fp(hmac.digest(Helper::str_to_char_vec("510")))));
}

TEST(CryptoTests, HashStrVec){
    // Create a HMAC object and a testing vector.
    const auto hmac = HMAC();
    const StrVec x{"0", "1", "2", "3", "4", "5"};

    // Hash the vector without sel.
    const auto hx = hmac.digest_vec_to_fp(x);

    // Perform testing.
    EXPECT_TRUE(Field::cmp(hx[0], Helper::char_vec_to_fp(hmac.digest(Helper::str_to_char_vec("00")))));
    EXPECT_TRUE(Field::cmp(hx[5], Helper::char_vec_to_fp(hmac.digest(Helper::str_to_char_vec("55")))));

    // Random number test.
    EXPECT_FALSE(Field::cmp(hx[1], Helper::char_vec_to_fp(hmac.digest(Helper::str_to_char_vec("Y")))));
}

TEST(CryptoTests, HashIntMat){
    // Create a HMAC object and a testing vector.
    const auto hmac = HMAC();
    const IntMat x{{0, 1, 2, 3, 4, 5}, {0, 1, 2, 3, 4, 5}, {0, 1, 2, 3, 4, 5}};

    // Hash the vector without sel.
    const auto hx = hmac.digest_mat_to_fp(x);

    // Perform testing.
    EXPECT_TRUE(Field::cmp(hx[0][5], Helper::char_vec_to_fp(hmac.digest(Helper::str_to_char_vec("50")))));
    EXPECT_TRUE(Field::cmp(hx[1][0], Helper::char_vec_to_fp(hmac.digest(Helper::str_to_char_vec("01")))));
    EXPECT_TRUE(Field::cmp(hx[1][1], Helper::char_vec_to_fp(hmac.digest(Helper::str_to_char_vec("11")))));
    EXPECT_TRUE(Field::cmp(hx[2][4], Helper::char_vec_to_fp(hmac.digest(Helper::str_to_char_vec("42")))));
    EXPECT_TRUE(Field::cmp(hx[2][5], Helper::char_vec_to_fp(hmac.digest(Helper::str_to_char_vec("52")))));
}
