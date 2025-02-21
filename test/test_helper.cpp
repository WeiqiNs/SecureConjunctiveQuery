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

    EXPECT_TRUE(Field::cmp(x, 4660));
}

TEST(HelperTests, XorCharVec){
    const CharMat x_mat = {{0x12, 0x34}};
    EXPECT_EQ(x_mat[0], Helper::xor_char_vec(x_mat));

    const CharMat y_mat = {{0x12, 0x34}, {0x23, 0x45}, {0x34, 0x56}, {0x00, 0x00}};
    const CharVec y_xor = {0x05, 0x27};
    EXPECT_EQ(y_xor, Helper::xor_char_vec(y_mat));
}

TEST(HelperTests, PowerPoly){
    const IntVec x_vec = {1, 2, 3};

    const auto pairing_group = BP();

    const auto r = Helper::power_poly(3, pairing_group, pairing_group.Zp->from_int(x_vec));

    // Check the size.
    EXPECT_EQ(r.size(), 10);
    // Check correctness.
    EXPECT_TRUE(Field::cmp(r[0], 1) && Field::cmp(r[8], 27) && Field::cmp(r[9], 1));
}

TEST(HelperTests, CoeffPoly){
    const IntMat x_mat = {{1, 2, 3}, {4, 5}, {6}};

    const auto pairing_group = BP();

    const auto r = Helper::coeff_poly(3, pairing_group, pairing_group.Zp->from_int(x_mat));

    // Check the size.
    EXPECT_EQ(r.size(), 10);
    // Check correctness.
    EXPECT_TRUE(
        Field::cmp(r[0], 11) &&
        Field::cmp(r[1], pairing_group.Zp->from_int(-6)) &&
        Field::cmp(r[2], 1) &&
        Field::cmp(r[3], pairing_group.Zp->from_int(-9)) &&
        Field::cmp(r[4], 1) &&
        Field::cmp(r[5], 0) &&
        Field::cmp(r[9], 8)
    );
}

TEST(HelperTests, SplitPoly){
    const IntVec x = {1, 2, 3, 4, 5};

    const auto pairing_group = BP();

    const auto r = Helper::split_poly(pairing_group, pairing_group.Zp->from_int(x));

    EXPECT_TRUE(
        Field::cmp(pairing_group.Zp->add(r[0], r[5]), 1) &&
        Field::cmp(pairing_group.Zp->add(r[2], r[7]), 3)
    );
}

TEST(HelperTests, FilterTrue){
    const IntVec x = {1, 2, 3};
    const IntMat y = {{1, 2}, {2, 3}, {1, 3}};

    const auto pairing_group = BP();

    const auto poly_x = Helper::power_poly(3, pairing_group, pairing_group.Zp->from_int(x));
    const auto poly_y = Helper::coeff_poly(3, pairing_group, pairing_group.Zp->from_int(y));

    const auto r = pairing_group.Zp->vec_ip(poly_x, poly_y);

    EXPECT_TRUE(Field::cmp(r, 0));
}

TEST(HelperTests, FilterFalse){
    const IntVec x = {1, 2, 3};
    const IntMat y = {{2, 3}, {2, 3}, {1, 3}};

    const auto pairing_group = BP();

    const auto poly_x = Helper::power_poly(3, pairing_group, pairing_group.Zp->from_int(x));
    const auto poly_y = Helper::coeff_poly(3, pairing_group, pairing_group.Zp->from_int(y));

    const auto r = pairing_group.Zp->vec_ip(poly_x, poly_y);

    EXPECT_FALSE(Field::cmp(r, 0));
}

TEST(HelperTests, FilterSplitTrue){
    const IntVec x = {1, 2, 3};
    const IntMat y = {{1, 2}, {2, 3}, {1, 3}};

    const auto pairing_group = BP();

    const auto poly_x = Helper::power_poly(3, pairing_group, pairing_group.Zp->from_int(x));
    const auto poly_y = Helper::coeff_poly(3, pairing_group, pairing_group.Zp->from_int(y));

    const auto r = pairing_group.Zp->vec_ip(
        Field::vec_join(poly_x, poly_x), Helper::split_poly(pairing_group, poly_y)
    );

    EXPECT_TRUE(Field::cmp(r, 0));
}

TEST(HelperTests, FilterSplitFalse){
    const IntVec x = {1, 2, 3};
    const IntMat y = {{2, 3}, {2, 3}, {1, 3}};

    const auto pairing_group = BP();

    const auto poly_x = Helper::power_poly(3, pairing_group, pairing_group.Zp->from_int(x));
    const auto poly_y = Helper::coeff_poly(3, pairing_group, pairing_group.Zp->from_int(y));

    const auto r = pairing_group.Zp->vec_ip(
        Field::vec_join(poly_x, poly_x), Helper::split_poly(pairing_group, poly_y)
    );

    EXPECT_FALSE(Field::cmp(r, 0));
}

TEST(HelperTests, SelectIndex){
    const IntVec sel = {1, 2, 5};

    const auto r = Helper::get_sel_index(3, 10, sel);

    EXPECT_EQ(r[0], 3);
    EXPECT_EQ(r.back(), 30);
}
