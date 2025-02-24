#include <gtest/gtest.h>
#include "sse_filter.hpp"

TEST(SseFilterTests, FilterTrue){
    auto msk = SseFilter::msk_gen();

    const IntVec x = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    const auto ct = SseFilter::enc(msk, x);
    const auto sk = SseFilter::keygen(msk, x, 1);

    EXPECT_TRUE(SseFilter::dec(ct, sk[0]));
}

TEST(SseFilterTests, FilterFalse){
    auto msk = SseFilter::msk_gen();

    const IntVec x = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const IntVec y = {100, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    const auto ct = SseFilter::enc(msk, x);
    const auto sk = SseFilter::keygen(msk, y, 1);

    EXPECT_FALSE(SseFilter::dec(ct, sk[0]));
}

TEST(SseFilterTests, FilterStrTrue){
    auto msk = SseFilter::msk_gen();

    const StrVec x = {"ab", "b", "c", "d", "e", "f", "g", "h", "i", "test"};

    const auto ct = SseFilter::enc(msk, x);
    const auto sk = SseFilter::keygen(msk, x, 1);

    EXPECT_TRUE(SseFilter::dec(ct, sk[0]));
}

TEST(SseFilterTests, FilterMultiEncTrue){
    auto msk = SseFilter::msk_gen();

    const IntVec x = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const IntVec y = {2, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const IntVec z = {3, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    const auto ctx = SseFilter::enc(msk, x);
    const auto cty = SseFilter::enc(msk, y);
    const auto ctz = SseFilter::enc(msk, z);
    const auto sk = SseFilter::keygen(msk, z, 3);

    EXPECT_TRUE(SseFilter::dec(ctz, sk[2]));
}