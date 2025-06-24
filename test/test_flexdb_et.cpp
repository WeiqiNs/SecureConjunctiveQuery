#include <gtest/gtest.h>
#include "flexdb_et.hpp"


TEST(FlexDbEtTests, SumTrue){
    const auto pp = FlexDbEt::pp_gen(10);
    const auto msk = FlexDbEt::msk_gen(pp);

    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y = {0, 1, 1, 1, 1, 1, 1, 1, 2, -3};
    constexpr int p = 17;

    const auto ct = FlexDbEt::enc(pp, msk, x);
    const auto sk = FlexDbEt::keygen(pp, msk, y, p);

    EXPECT_TRUE(FlexDbEt::dec(ct, sk));
}

TEST(FlexDbEtTests, SumFalse){
    const auto pp = FlexDbEt::pp_gen(10);
    const auto msk = FlexDbEt::msk_gen(pp);

    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    constexpr int p = 1;

    const auto ct = FlexDbEt::enc(pp, msk, x);
    const auto sk = FlexDbEt::keygen(pp, msk, y, p);

    EXPECT_FALSE(FlexDbEt::dec(ct, sk));
}

TEST(FlexDbEtTests, SelSumTrue){
    const auto pp = FlexDbEt::pp_gen(10);
    const auto msk = FlexDbEt::msk_gen(pp);

    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y = {1, 1, 1};
    const IntVec sel = {2, 4, 6};
    constexpr int p = 12;

    const auto ct = FlexDbEt::enc(pp, msk, x);
    const auto sk = FlexDbEt::keygen(pp, msk, y, p, sel);

    EXPECT_TRUE(FlexDbEt::dec(ct, sk, sel));
}

TEST(FlexDbEtTests, SumTrueWithCompress){
    const auto pp = FlexDbEt::pp_gen(10);
    const auto msk = FlexDbEt::msk_gen(pp, {}, true);

    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y = {0, 1, 1, 1, 1, 1, 1, 1, 2, -3};
    constexpr int p = 17;

    const auto ct = FlexDbEt::enc(pp, msk, x);
    const auto sk = FlexDbEt::keygen(pp, msk, y, p);

    EXPECT_TRUE(FlexDbEt::dec(ct, sk));
}

TEST(FlexDbEtTests, SelSumTrueWithCompress){
    const auto pp = FlexDbEt::pp_gen(10);
    const auto msk = FlexDbEt::msk_gen(pp, {}, true);

    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y = {1, 1, 1};
    const IntVec sel = {2, 4, 6};
    constexpr int p = 12;

    const auto ct = FlexDbEt::enc(pp, msk, x);
    const auto sk = FlexDbEt::keygen(pp, msk, y, p, sel);

    EXPECT_TRUE(FlexDbEt::dec(ct, sk, sel));
}
