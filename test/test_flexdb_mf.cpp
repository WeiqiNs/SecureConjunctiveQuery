#include <gtest/gtest.h>
#include "flexdb_mf.hpp"


TEST(FlexDbMfTests, IntTrue){
    const auto pp = FlexDbMf::pp_gen(5, 10);
    const auto msk = FlexDbMf::msk_gen(pp);

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

    const auto ct = FlexDbMf::enc(pp, msk, x);
    const auto sk = FlexDbMf::keygen(pp, msk, y);

    EXPECT_TRUE(FlexDbMf::dec(pp, ct, sk));
    BP::close();
}

TEST(FlexDbMfTests, IntFalse){
    const auto pp = FlexDbMf::pp_gen(5, 10);
    const auto msk = FlexDbMf::msk_gen(pp);

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

    const auto ct = FlexDbMf::enc(pp, msk, x);
    const auto sk = FlexDbMf::keygen(pp, msk, y);

    EXPECT_FALSE(FlexDbMf::dec(pp, ct, sk));
    BP::close();
}

TEST(FlexDbMfTests, StrTrue){
    const auto pp = FlexDbMf::pp_gen(5, 10);
    const auto msk = FlexDbMf::msk_gen(pp);

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

    const auto ct = FlexDbMf::enc(pp, msk, x);
    const auto sk = FlexDbMf::keygen(pp, msk, y);

    EXPECT_TRUE(FlexDbMf::dec(pp, ct, sk));
    BP::close();
}

TEST(FlexDbMfTests, StrFalse){
    const auto pp = FlexDbMf::pp_gen(5, 10);
    const auto msk = FlexDbMf::msk_gen(pp);

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

    const auto ct = FlexDbMf::enc(pp, msk, x);
    const auto sk = FlexDbMf::keygen(pp, msk, y);

    EXPECT_FALSE(FlexDbMf::dec(pp, ct, sk));
    BP::close();
}

TEST(FlexDbMfTests, SelIntTrue){
    const auto pp = FlexDbMf::pp_gen(5, 10);
    const auto msk = FlexDbMf::msk_gen(pp);

    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntMat y = {
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4}
        };
    const IntVec sel = {0, 1, 2, 3, 4};

    const auto ct = FlexDbMf::enc(pp, msk, x);
    const auto sk = FlexDbMf::keygen(pp, msk, y, sel);

    EXPECT_TRUE(FlexDbMf::dec(pp, ct, sk, sel));
    BP::close();
}

TEST(FlexDbMfTests, SelIntFalse){
    const auto pp = FlexDbMf::pp_gen(5, 10);
    const auto msk = FlexDbMf::msk_gen(pp);

    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntMat y = {
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4}
        };
    const IntVec sel = {0, 8};

    const auto ct = FlexDbMf::enc(pp, msk, x);
    const auto sk = FlexDbMf::keygen(pp, msk, y, sel);

    EXPECT_FALSE(FlexDbMf::dec(pp, ct, sk, sel));
    BP::close();
}

TEST(FlexDbMfTests, IntTrueWithCompress){
    const auto pp = FlexDbMf::pp_gen(5, 10);
    const auto msk = FlexDbMf::msk_gen(pp, {}, true);

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

    const auto ct = FlexDbMf::enc(pp, msk, x);
    const auto sk = FlexDbMf::keygen(pp, msk, y);

    EXPECT_TRUE(FlexDbMf::dec(pp, ct, sk));
    BP::close();
}

TEST(FlexDbMfTests, SelIntTrueWithCompress){
    const auto pp = FlexDbMf::pp_gen(5, 10);
    const auto msk = FlexDbMf::msk_gen(pp, {}, true);

    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntMat y = {
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4}
        };
    const IntVec sel = {0, 1, 2, 3, 4};

    const auto ct = FlexDbMf::enc(pp, msk, x);
    const auto sk = FlexDbMf::keygen(pp, msk, y, sel);

    EXPECT_TRUE(FlexDbMf::dec(pp, ct, sk, sel));
    BP::close();
}
