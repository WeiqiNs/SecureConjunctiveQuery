#include <gtest/gtest.h>
#include "filter.hpp"

TEST(FilterTests, DegOneIntTrue){
    auto pp = Filter::pp_gen(1);
    const auto msk = Filter::msk_gen(pp, 10);

    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    const auto ct = Filter::enc(pp, msk, x);
    const auto sk = Filter::keygen(pp, msk, y);

    EXPECT_TRUE(Filter::dec(ct, sk));
    BP::close();
}

TEST(FilterTests, DegOneIntFalse){
    auto pp = Filter::pp_gen(1);
    const auto msk = Filter::msk_gen(pp, 10);

    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y = {0, 1, 2, 3, 4, 5, 6, 7, 8, 100};

    const auto ct = Filter::enc(pp, msk, x);
    const auto sk = Filter::keygen(pp, msk, y);

    EXPECT_FALSE(Filter::dec(ct, sk));
    BP::close();
}

TEST(FilterTests, DegOneStrTrue){
    auto pp = Filter::pp_gen(1);
    const auto msk = Filter::msk_gen(pp, 10);

    const StrVec x = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    const StrVec y = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

    const auto ct = Filter::enc(pp, msk, x);
    const auto sk = Filter::keygen(pp, msk, y);

    EXPECT_TRUE(Filter::dec(ct, sk));
    BP::close();
}

TEST(FilterTests, DegOneStrFalse){
    auto pp = Filter::pp_gen(1);
    const auto msk = Filter::msk_gen(pp, 10);

    const StrVec x = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    const StrVec y = {"0", "1", "2", "3", "Wrong", "5", "6", "7", "8", "9"};

    const auto ct = Filter::enc(pp, msk, x);
    const auto sk = Filter::keygen(pp, msk, y);

    EXPECT_FALSE(Filter::dec(ct, sk));
    BP::close();
}

TEST(FilterTests, DegMulIntTrue){
    auto pp = Filter::pp_gen(5);
    const auto msk = Filter::msk_gen(pp, 10);

    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntMat y = {
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 5},
            {0, 1, 2, 3, 6},
            {0, 1, 2, 3, 7},
            {0, 1, 2, 3, 8},
            {0, 1, 2, 3, 9}
        };

    const auto ct = Filter::enc(pp, msk, x);
    const auto sk = Filter::keygen(pp, msk, y);

    EXPECT_TRUE(Filter::dec(ct, sk));
    BP::close();
}

TEST(FilterTests, DegMulIntFalse){
    auto pp = Filter::pp_gen(5);
    const auto msk = Filter::msk_gen(pp, 10);

    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntMat y = {
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 5},
            {0, 1, 2, 3, 6},
            {0, 1, 2, 3, 7},
            {0, 1, 2, 3, 8},
            {0, 1, 2, 3}
        };

    const auto ct = Filter::enc(pp, msk, x);
    const auto sk = Filter::keygen(pp, msk, y);

    EXPECT_FALSE(Filter::dec(ct, sk));
    BP::close();
}

TEST(FilterTests, DegMulStrTrue){
    // int main() {
    auto pp = Filter::pp_gen(5);
    const auto msk = Filter::msk_gen(pp, 10);

    const StrVec x = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    const StrMat y = {
            {"0", "1", "2", "3", "4"},
            {"0", "1", "2", "3", "4"},
            {"0", "1", "2", "3", "4"},
            {"0", "1", "2", "3", "4"},
            {"0", "1", "2", "3", "4"},
            {"0", "1", "2", "3", "5"},
            {"0", "1", "2", "3", "6"},
            {"0", "1", "2", "3", "7"},
            {"0", "1", "2", "3", "8"},
            {"0", "1", "2", "3", "9"}
        };

    const auto ct = Filter::enc(pp, msk, x);
    const auto sk = Filter::keygen(pp, msk, y);

    EXPECT_TRUE(Filter::dec(ct, sk));
    BP::close();
}

TEST(FilterTests, DegMulStrFalse){
    // int main() {
    auto pp = Filter::pp_gen(5);
    const auto msk = Filter::msk_gen(pp, 10);

    const StrVec x = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    const StrMat y = {
            {"0", "1", "2", "3", "4"},
            {"0"},
            {"0", "1", "2", "3", "4"},
            {"0", "1", "2", "3", "4"},
            {"0", "1", "2", "3", "4"},
            {"0", "1", "2", "3", "5"},
            {"0", "1", "2", "3", "6"},
            {"0", "1", "2", "3", "7"},
            {"0", "1", "2", "3", "8"},
            {"0", "1", "2", "3", "9"}
        };

    const auto ct = Filter::enc(pp, msk, x);
    const auto sk = Filter::keygen(pp, msk, y);

    EXPECT_FALSE(Filter::dec(ct, sk));
    BP::close();
}

TEST(FilterTests, DegOneSelIntTrue){
    auto pp = Filter::pp_gen(1);
    const auto msk = Filter::msk_gen(pp, 10);

    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y = {0, 1, 2, 3, 4};
    const IntVec sel = {0, 1, 2, 3, 4};

    const auto ct = Filter::enc(pp, msk, x);
    const auto sk = Filter::keygen(pp, msk, y, sel);

    EXPECT_TRUE(Filter::dec(pp, ct, sk, sel));
    BP::close();
}

TEST(FilterTests, DegOneSelIntFalse){
    auto pp = Filter::pp_gen(1);
    const auto msk = Filter::msk_gen(pp, 10);

    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y = {0, 100};
    const IntVec sel = {0, 9};

    const auto ct = Filter::enc(pp, msk, x);
    const auto sk = Filter::keygen(pp, msk, y, sel);

    EXPECT_FALSE(Filter::dec(pp, ct, sk, sel));
    BP::close();
}

TEST(FilterTests, DegMulSelIntTrue){
    auto pp = Filter::pp_gen(5);
    const auto msk = Filter::msk_gen(pp, 10);

    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntMat y = {
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4}
        };
    const IntVec sel = {0, 1, 2, 3, 4};

    const auto ct = Filter::enc(pp, msk, x);
    const auto sk = Filter::keygen(pp, msk, y, sel);

    EXPECT_TRUE(Filter::dec(pp, ct, sk, sel));
    BP::close();
}

TEST(FilterTests, DegMulSelIntFalse){
    auto pp = Filter::pp_gen(5);
    const auto msk = Filter::msk_gen(pp, 10);

    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntMat y = {
        {0, 1, 2, 3, 4},
        {0, 1, 2, 3, 4}
    };
    const IntVec sel = {0, 8};

    const auto ct = Filter::enc(pp, msk, x);
    const auto sk = Filter::keygen(pp, msk, y, sel);

    EXPECT_FALSE(Filter::dec(pp, ct, sk, sel));
    BP::close();
}
