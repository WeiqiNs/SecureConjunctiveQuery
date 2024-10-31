#include <gtest/gtest.h>
#include "filter.hpp"

TEST(FilterTests, DegOneIntTrue){
    const auto pp = Filter::pp_gen(1, 10);
    const auto msk = Filter::msk_gen(pp);

    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    const auto ct = Filter::enc(pp, msk, x);
    const auto sk = Filter::keygen(pp, msk, y);

    EXPECT_TRUE(Filter::dec(ct, sk));
    BP::close();
}

TEST(FilterTests, DegOneIntFalse){
    const auto pp = Filter::pp_gen(1, 10);
    const auto msk = Filter::msk_gen(pp);

    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y = {0, 1, 2, 3, 4, 5, 6, 7, 8, 100};

    const auto ct = Filter::enc(pp, msk, x);
    const auto sk = Filter::keygen(pp, msk, y);

    EXPECT_FALSE(Filter::dec(ct, sk));
    BP::close();
}

TEST(FilterTests, DegOneStrTrue){
    const auto pp = Filter::pp_gen(1, 10);
    const auto msk = Filter::msk_gen(pp);

    const StrVec x = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    const StrVec y = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

    const auto ct = Filter::enc(pp, msk, x);
    const auto sk = Filter::keygen(pp, msk, y);

    EXPECT_TRUE(Filter::dec(ct, sk));
    BP::close();
}

TEST(FilterTests, DegOneStrFalse){
    const auto pp = Filter::pp_gen(1, 10);
    const auto msk = Filter::msk_gen(pp);

    const StrVec x = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    const StrVec y = {"0", "1", "2", "3", "Wrong", "5", "6", "7", "8", "9"};

    const auto ct = Filter::enc(pp, msk, x);
    const auto sk = Filter::keygen(pp, msk, y);

    EXPECT_FALSE(Filter::dec(ct, sk));
    BP::close();
}

TEST(FilterTests, DegMulIntTrue){
    const auto pp = Filter::pp_gen(5, 10);
    const auto msk = Filter::msk_gen(pp);

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
    const auto pp = Filter::pp_gen(5, 10);
    const auto msk = Filter::msk_gen(pp);

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
    const auto pp = Filter::pp_gen(5, 10);
    const auto msk = Filter::msk_gen(pp);

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
    const auto pp = Filter::pp_gen(5, 10);
    const auto msk = Filter::msk_gen(pp);

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
    const auto pp = Filter::pp_gen(1, 10);
    const auto msk = Filter::msk_gen(pp);

    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y = {0, 1, 2, 3, 4};
    const IntVec sel = {0, 1, 2, 3, 4};

    const auto ct = Filter::enc(pp, msk, x);
    const auto sk = Filter::keygen(pp, msk, y, sel);

    EXPECT_TRUE(Filter::dec(pp, ct, sk, sel));
    BP::close();
}

TEST(FilterTests, DegOneSelIntFalse){
    const auto pp = Filter::pp_gen(1, 10);
    const auto msk = Filter::msk_gen(pp);

    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y = {0, 100};
    const IntVec sel = {0, 9};

    const auto ct = Filter::enc(pp, msk, x);
    const auto sk = Filter::keygen(pp, msk, y, sel);

    EXPECT_FALSE(Filter::dec(pp, ct, sk, sel));
    BP::close();
}

TEST(FilterTests, DegMulSelIntTrue){
    const auto pp = Filter::pp_gen(5, 10);
    const auto msk = Filter::msk_gen(pp);

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
    const auto pp = Filter::pp_gen(5, 10);
    const auto msk = Filter::msk_gen(pp);

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
