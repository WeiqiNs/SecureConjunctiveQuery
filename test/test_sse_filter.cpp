#include <gtest/gtest.h>
#include "sse_filter.hpp"

TEST(SseFilterTests, FilterTrue){
    const auto msk = SseFilter::msk_gen();

    const IntVec x = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    const auto ct = SseFilter::enc(msk, x);
    const auto sk = SseFilter::keygen(msk, x);

    EXPECT_TRUE(SseFilter::dec(ct, sk));
}

TEST(SseFilterTests, FilterFalse){
    const auto msk = SseFilter::msk_gen();

    const IntVec x = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const IntVec y = {100, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    const auto ct = SseFilter::enc(msk, x);
    const auto sk = SseFilter::keygen(msk, y);

    EXPECT_FALSE(SseFilter::dec(ct, sk));
}

TEST(SseFilterTests, FilterStrTrue){
    const auto msk = SseFilter::msk_gen();

    const StrVec x = {"ab", "b", "c", "d", "e", "f", "g", "h", "i", "test"};

    const auto ct = SseFilter::enc(msk, x);
    const auto sk = SseFilter::keygen(msk, x);

    EXPECT_TRUE(SseFilter::dec(ct, sk));
}