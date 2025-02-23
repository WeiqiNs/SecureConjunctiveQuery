#include <gtest/gtest.h>
#include "kim_aggre.hpp"

TEST(KimAggreTests, AggreTrue){
    const auto pp = KimAggre::pp_gen(10);
    const auto msk = KimAggre::msk_gen(pp);

    const IntVec x = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const IntVec y = {1, 1, 1, 1, 1, 0, 0, 0, 0, 3};

    const auto ct = KimAggre::enc(pp, msk, x);
    const auto sk = KimAggre::keygen(pp, msk, y, 45);

    EXPECT_TRUE(KimAggre::dec(ct, sk));
}

TEST(KimAggreTests, AggreFalse){
    const auto pp = KimAggre::pp_gen(10);
    const auto msk = KimAggre::msk_gen(pp);

    const IntVec x = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const IntVec y = {1, 1, 1, 1, 1, 0, 0, 0, 0, 0};

    const auto ct = KimAggre::enc(pp, msk, x);
    const auto sk = KimAggre::keygen(pp, msk, y, 100);

    EXPECT_FALSE(KimAggre::dec(ct, sk));
}
