#include <gtest/gtest.h>
#include "helper.hpp"

TEST(HelperTests, CharIntConversion){
    constexpr int x = 10;

    const CharVec x_vec = Helper::int_to_char_vec(x);

    EXPECT_EQ(Helper::char_vec_to_int(x_vec), x);
}

TEST(HelperTests, CharNegIntConversion){
    constexpr int x = -10;

    const CharVec x_vec = Helper::int_to_char_vec(x);

    EXPECT_EQ(Helper::char_vec_to_int(x_vec), x);
}

TEST(HelperTests, CharStrConversion){
    const str x = "This is a test.";

    const CharVec x_vec = Helper::str_to_char_vec(x);

    EXPECT_EQ(Helper::char_vec_to_str(x_vec), x);
}

TEST(HelperTests, CharFpConversion){
    const CharVec x_vec = {0x12, 0x34};

    const Fp x = Helper::char_vec_to_fp(x_vec);

    EXPECT_TRUE(Field::cmp(x, 13330));
}
