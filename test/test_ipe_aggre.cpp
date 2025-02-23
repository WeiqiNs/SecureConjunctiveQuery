#include <gtest/gtest.h>
#include "ipe_aggre.hpp"

TEST(IpeAggreTests, AggreTrue){
    const auto pp = IpeAggre::pp_gen(10);
    const auto msk = IpeAggre::msk_gen(pp);

    const IntVec x = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const IntVec y = {1, 1, 1, 1, 1, 0, 0, 0, 0, 2};

    const auto ct = IpeAggre::enc(pp, msk, x);
    const auto sk = IpeAggre::keygen(pp, msk, y, 35);

    EXPECT_TRUE(IpeAggre::dec(ct, sk));
}

TEST(IpeAggreTests, AggreFalse){
    const auto pp = IpeAggre::pp_gen(10);
    const auto msk = IpeAggre::msk_gen(pp);

    const IntVec x = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const IntVec y = {1, 1, 1, 1, 1, 0, 0, 0, 0, 0};

    const auto ct = IpeAggre::enc(pp, msk, x);
    const auto sk = IpeAggre::keygen(pp, msk, y, 100);

    EXPECT_FALSE(IpeAggre::dec(ct, sk));
}
