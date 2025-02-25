#include <gtest/gtest.h>
#include "ipe_join.hpp"

TEST(IpeJoinTests, JoinTrue){
    const auto pp = IpeJoin::pp_gen(1, 10);
    const auto msk = IpeJoin::msk_gen(pp);

    const IntVec x1 = {1, 100, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec x2 = {6, 3, 100, 4, 5, 6, 7, 8, 9, 10};
    const IntMat y1 = {{1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}};
    const IntMat y2 = {{6}, {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10}};

    const auto ct1 = IpeJoin::enc(pp, msk, x1, 1);
    const auto ct2 = IpeJoin::enc(pp, msk, x2, 2);
    const auto sk1 = IpeJoin::keygen(pp, msk, y1);
    const auto sk2 = IpeJoin::keygen(pp, msk, y2);

    EXPECT_TRUE(Group::cmp_gt(IpeJoin::dec(ct1, sk1), IpeJoin::dec(ct2, sk2)));
}

TEST(IpeJoinTests, JoinFalse){
    const auto pp = IpeJoin::pp_gen(1, 10);
    const auto msk = IpeJoin::msk_gen(pp);

    const IntVec x1 = {1, 100, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec x2 = {6, 3, 1, 4, 5, 6, 7, 8, 9, 10};
    const IntMat y1 = {{1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}};
    const IntMat y2 = {{6}, {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10}};

    const auto ct1 = IpeJoin::enc(pp, msk, x1, 1);
    const auto ct2 = IpeJoin::enc(pp, msk, x2, 2);
    const auto sk1 = IpeJoin::keygen(pp, msk, y1);
    const auto sk2 = IpeJoin::keygen(pp, msk, y2);

    EXPECT_FALSE(Group::cmp_gt(IpeJoin::dec(ct1, sk1), IpeJoin::dec(ct2, sk2)));
}

TEST(IpeJoinTests, JoinHighDegTrue){
    const auto pp = IpeJoin::pp_gen(5, 10);
    const auto msk = IpeJoin::msk_gen(pp);

    const IntVec x1 = {1, 100, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec x2 = {6, 3, 100, 4, 5, 6, 7, 8, 9, 10};
    const IntMat y1 = {{1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}, {7, 8}, {8, 9}, {9, 10}};
    const IntMat y2 = {{6, 7}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}, {7, 8}, {8, 9}, {9, 10}};

    const auto ct1 = IpeJoin::enc(pp, msk, x1, 1);
    const auto ct2 = IpeJoin::enc(pp, msk, x2, 2);
    const auto sk1 = IpeJoin::keygen(pp, msk, y1);
    const auto sk2 = IpeJoin::keygen(pp, msk, y2);

    EXPECT_TRUE(Group::cmp_gt(IpeJoin::dec(ct1, sk1), IpeJoin::dec(ct2, sk2)));
}

TEST(IpeJoinTests, JoinHighDegFalse){
    const auto pp = IpeJoin::pp_gen(5, 10);
    const auto msk = IpeJoin::msk_gen(pp);

    const IntVec x1 = {1, 100, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec x2 = {6, 3, 1, 4, 5, 6, 7, 8, 9, 10};
    const IntMat y1 = {{1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}, {7, 8}, {8, 9}, {9, 10}};
    const IntMat y2 = {{6, 7}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}, {7, 8}, {8, 9}, {9, 10}};

    const auto ct1 = IpeJoin::enc(pp, msk, x1, 1);
    const auto ct2 = IpeJoin::enc(pp, msk, x2, 2);
    const auto sk1 = IpeJoin::keygen(pp, msk, y1);
    const auto sk2 = IpeJoin::keygen(pp, msk, y2);

    EXPECT_FALSE(Group::cmp_gt(IpeJoin::dec(ct1, sk1), IpeJoin::dec(ct2, sk2)));
}

TEST(IpeJoinTests, JoinHighDegFalseFilter){
    const auto pp = IpeJoin::pp_gen(5, 10);
    const auto msk = IpeJoin::msk_gen(pp);

    const IntVec x1 = {1, 100, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec x2 = {6, 3, 100, 4, 5, 6, 7, 8, 9, 10};
    const IntMat y1 = {{1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}, {7, 8}, {8, 9}, {9, 10}};
    const IntMat y2 = {{6, 7}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}, {7, 8}, {8, 9}, {9}};

    const auto ct1 = IpeJoin::enc(pp, msk, x1, 1);
    const auto ct2 = IpeJoin::enc(pp, msk, x2, 2);
    const auto sk1 = IpeJoin::keygen(pp, msk, y1);
    const auto sk2 = IpeJoin::keygen(pp, msk, y2);

    EXPECT_FALSE(Group::cmp_gt(IpeJoin::dec(ct1, sk1), IpeJoin::dec(ct2, sk2)));
}

TEST(IpeJoinTests, JoinDifferentLengthTrue){
    // Generate two set of pp and msk.
    const auto pp1 = IpeJoin::pp_gen(1, 10);
    const auto msk1 = IpeJoin::msk_gen(pp1);
    const auto pp2 = IpeJoin::pp_gen(1, 8);
    auto msk2 = IpeJoin::msk_gen(pp2, msk1.hmac->get_key());

    // We need to unify the k used. This k should be updated for each query.
    msk2.k = msk1.k;

    const IntVec x1 = {1, 100, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntMat y1 = {{1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}};
    const auto ct1 = IpeJoin::enc(pp1, msk1, x1, 1);
    const auto sk1 = IpeJoin::keygen(pp1, msk1, y1);

    const IntVec x2 = {6, 3, 100, 4, 5, 6, 7, 8};
    const IntMat y2 = {{6}, {3}, {4}, {5}, {6}, {7}, {8}};
    const auto ct2 = IpeJoin::enc(pp2, msk2, x2, 2);
    const auto sk2 = IpeJoin::keygen(pp2, msk2, y2);

    EXPECT_TRUE(Group::cmp_gt(IpeJoin::dec(ct1, sk1), IpeJoin::dec(ct2, sk2)));
}