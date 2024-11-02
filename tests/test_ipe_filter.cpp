#include <gtest/gtest.h>
#include "ipe_filter.hpp"

TEST(IpeFilterTests, FilterTrue){
    const auto pp = IpeFilter::pp_gen(5, 10);
    const auto msk = IpeFilter::msk_gen(pp);

    const IntVec x = {100, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntMat y = {
        // You could use 0 as a wild card to filter anything.
        {0}, {1, 2, 3, 4, 5}, {2, 3}, {3, 4}, {4, 5},
        {5, 6}, {6, 7}, {7, 8}, {8, 9}, {9, 10}
    };

    const auto ct = IpeFilter::enc(pp, msk, x);
    const auto sk = IpeFilter::keygen(pp, msk, y);

    EXPECT_TRUE(IpeFilter::dec(ct, sk));
}

TEST(IpeFilterTests, FilterFalse){
    const auto pp = IpeFilter::pp_gen(5, 10);
    const auto msk = IpeFilter::msk_gen(pp);

    const IntVec x = {100, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntMat y = {
        {1}, {1, 2, 3, 4, 5}, {2, 3}, {3, 4}, {4, 5},
        {5, 6}, {6, 7}, {7, 8}, {8, 9}, {9, 10}
    };

    const auto ct = IpeFilter::enc(pp, msk, x);
    const auto sk = IpeFilter::keygen(pp, msk, y);

    EXPECT_FALSE(IpeFilter::dec(ct, sk));
}
