#include <gtest/gtest.h>
#include "aggre.hpp"

TEST(AggreTests, SumTrue){
    const auto pp = Aggre::pp_gen(10);
    const auto msk = Aggre::msk_gen(pp);

    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y = {0, 1, 1, 1, 1, 1, 1, 1, 2, -3};
    constexpr int p = 17;

    const auto ct = Aggre::enc(pp, msk, x);
    const auto sk = Aggre::keygen(pp, msk, y, p);

    EXPECT_TRUE(Aggre::dec(ct, sk));
}

TEST(AggreTests, SumFalse){
    const auto pp = Aggre::pp_gen(10);
    const auto msk = Aggre::msk_gen(pp);

    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    constexpr int p = 1;

    const auto ct = Aggre::enc(pp, msk, x);
    const auto sk = Aggre::keygen(pp, msk, y, p);

    EXPECT_FALSE(Aggre::dec(ct, sk));
}
