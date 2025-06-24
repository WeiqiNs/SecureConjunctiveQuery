#include <gtest/gtest.h>
#include "ipe_et.hpp"


TEST(IpeEtTests, AggreTrue){
    const auto pp = IpeEt::pp_gen(10);
    const auto msk = IpeEt::msk_gen(pp);

    const IntVec x = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const IntVec y = {1, 1, 1, 1, 1, 0, 0, 0, 0, 2};

    const auto ct = IpeEt::enc(pp, msk, x);
    const auto sk = IpeEt::keygen(pp, msk, y, 35);

    EXPECT_TRUE(IpeEt::dec(ct, sk));
}

TEST(IpeEtTests, AggreFalse){
    const auto pp = IpeEt::pp_gen(10);
    const auto msk = IpeEt::msk_gen(pp);

    const IntVec x = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const IntVec y = {1, 1, 1, 1, 1, 0, 0, 0, 0, 0};

    const auto ct = IpeEt::enc(pp, msk, x);
    const auto sk = IpeEt::keygen(pp, msk, y, 100);

    EXPECT_FALSE(IpeEt::dec(ct, sk));
}
