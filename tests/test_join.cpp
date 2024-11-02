#include <gtest/gtest.h>
#include "join.hpp"

TEST(JoinTests, JoinDegOneTrue){
    const auto pp = Join::pp_gen(1, 10);
    const auto msk = Join::msk_gen(pp);

    const IntVec x1 = {1, 100, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec x2 = {6, 3, 100, 4, 5, 6, 7, 8, 9, 10};
    const IntVec y1 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y2 = {6, 3, 4, 5, 6, 7, 8, 9, 10};

    const auto ct1 = Join::enc(pp, msk, x1, 1);
    const auto ct2 = Join::enc(pp, msk, x2, 2);
    const auto sk1 = Join::keygen(pp, msk, y1);
    const auto sk2 = Join::keygen(pp, msk, y2);

    EXPECT_TRUE(Group::cmp_gt(Join::dec(ct1, sk1), Join::dec(ct2, sk2)));
}

TEST(JoinTests, JoinDegOneStr){
    const auto pp = Join::pp_gen(1, 7);
    const auto msk = Join::msk_gen(pp);

    const StrVec x1 = {"HERE", "a", "b", "c", "d", "e", "f"};
    const StrVec x2 = {"a", "b", "c", "d", "e", "f", "HERE"};
    const StrVec y1 = {"a", "b", "c", "d", "e", "f"};
    const StrVec y2 = {"a", "b", "c", "d", "e", "f"};

    const auto ct1 = Join::enc(pp, msk, x1, 0);
    const auto ct2 = Join::enc(pp, msk, x2, 6);
    const auto sk1 = Join::keygen(pp, msk, y1);
    const auto sk2 = Join::keygen(pp, msk, y2);

    EXPECT_TRUE(Group::cmp_gt(Join::dec(ct1, sk1), Join::dec(ct2, sk2)));
}

TEST(JoinTests, JoinDegOneFalseJoin){
    const auto pp = Join::pp_gen(1, 10);
    const auto msk = Join::msk_gen(pp);

    const IntVec x1 = {1, 100, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec x2 = {6, 3, 1, 4, 5, 6, 7, 8, 9, 10};
    const IntVec y1 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y2 = {6, 3, 4, 5, 6, 7, 8, 9, 10};

    const auto ct1 = Join::enc(pp, msk, x1, 1);
    const auto ct2 = Join::enc(pp, msk, x2, 2);
    const auto sk1 = Join::keygen(pp, msk, y1);
    const auto sk2 = Join::keygen(pp, msk, y2);

    EXPECT_FALSE(Group::cmp_gt(Join::dec(ct1, sk1), Join::dec(ct2, sk2)));
}

TEST(JoinTests, JoinDegOneFalseFilter){
    const auto pp = Join::pp_gen(1, 10);
    const auto msk = Join::msk_gen(pp);

    const IntVec x1 = {1, 100, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec x2 = {6, 3, 100, 4, 5, 6, 7, 8, 9, 10};
    const IntVec y1 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y2 = {6, 3, 4, 5, 6, 7, 8, 9, 1};

    const auto ct1 = Join::enc(pp, msk, x1, 1);
    const auto ct2 = Join::enc(pp, msk, x2, 2);
    const auto sk1 = Join::keygen(pp, msk, y1);
    const auto sk2 = Join::keygen(pp, msk, y2);

    EXPECT_FALSE(Group::cmp_gt(Join::dec(ct1, sk1), Join::dec(ct2, sk2)));
}

TEST(JoinTests, JoinDegMulTrue){
    const auto pp = Join::pp_gen(5, 10);
    const auto msk = Join::msk_gen(pp);

    const IntVec x1 = {1, 100, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec x2 = {6, 3, 100, 4, 5, 6, 7, 8, 9, 10};
    const IntMat y1 = {
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
    const IntMat y2 = {
            {0},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 5},
            {0, 1, 2, 3, 6},
            {0, 1, 2, 3, 7},
            {0, 1, 2, 3, 8},
            {0, 1, 2, 3, 9},
            {0, 1, 2, 3, 10}
        };

    const auto ct1 = Join::enc(pp, msk, x1, 1);
    const auto ct2 = Join::enc(pp, msk, x2, 2);
    const auto sk1 = Join::keygen(pp, msk, y1);
    const auto sk2 = Join::keygen(pp, msk, y2);

    EXPECT_TRUE(Group::cmp_gt(Join::dec(ct1, sk1), Join::dec(ct2, sk2)));
}

TEST(JoinTests, JoinDegMulFalseJoin){
    const auto pp = Join::pp_gen(5, 10);
    const auto msk = Join::msk_gen(pp);

    const IntVec x1 = {1, 100, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec x2 = {6, 3, 1, 4, 5, 6, 7, 8, 9, 10};
    const IntMat y1 = {
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
    const IntMat y2 = {
            {0, 1, 2, 3, 6},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 5},
            {0, 1, 2, 3, 6},
            {0, 1, 2, 3, 7},
            {0, 1, 2, 3, 8},
            {0, 1, 2, 3, 9},
            {0, 1, 2, 3, 10}
        };

    const auto ct1 = Join::enc(pp, msk, x1, 1);
    const auto ct2 = Join::enc(pp, msk, x2, 2);
    const auto sk1 = Join::keygen(pp, msk, y1);
    const auto sk2 = Join::keygen(pp, msk, y2);

    EXPECT_FALSE(Group::cmp_gt(Join::dec(ct1, sk1), Join::dec(ct2, sk2)));
}

TEST(JoinTests, JoinDegMulFalseFilter){
    const auto pp = Join::pp_gen(5, 10);
    const auto msk = Join::msk_gen(pp);

    const IntVec x1 = {1, 100, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec x2 = {6, 3, 100, 4, 5, 6, 7, 8, 9, 100};
    const IntMat y1 = {
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
    const IntMat y2 = {
            {0, 1, 2, 3, 6},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 5},
            {0, 1, 2, 3, 6},
            {0, 1, 2, 3, 7},
            {0, 1, 2, 3, 8},
            {0, 1, 2, 3, 9},
            {0, 1, 2, 3, 10}
        };

    const auto ct1 = Join::enc(pp, msk, x1, 1);
    const auto ct2 = Join::enc(pp, msk, x2, 2);
    const auto sk1 = Join::keygen(pp, msk, y1);
    const auto sk2 = Join::keygen(pp, msk, y2);

    EXPECT_FALSE(Group::cmp_gt(Join::dec(ct1, sk1), Join::dec(ct2, sk2)));
}

TEST(JoinTests, JoinDegOneSelTrue){
    const auto pp = Join::pp_gen(1, 10);
    const auto msk = Join::msk_gen(pp);

    const IntVec sel = {2, 4, 6};
    const IntVec x1 = {1, 100, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec x2 = {6, 3, 100, 4, 5, 6, 7, 8, 9, 10};
    const IntVec y1 = {3, 5, 7};
    const IntVec y2 = {4, 6, 8};

    const auto ct1 = Join::enc(pp, msk, x1, 1);
    const auto ct2 = Join::enc(pp, msk, x2, 2);
    const auto sk1 = Join::keygen(pp, msk, y1, sel);
    const auto sk2 = Join::keygen(pp, msk, y2, sel);

    EXPECT_TRUE(Group::cmp_gt(Join::dec(pp, ct1, sk1, sel), Join::dec(pp, ct2, sk2, sel)));
}

TEST(JoinTests, JoinDegOneSelFalse){
    const auto pp = Join::pp_gen(1, 10);
    const auto msk = Join::msk_gen(pp);

    const IntVec sel = {2, 4, 6};
    const IntVec x1 = {1, 100, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec x2 = {6, 3, 1, 4, 5, 6, 7, 8, 9, 10};
    const IntVec y1 = {3, 5, 7};
    const IntVec y2 = {4, 6, 8};

    const auto ct1 = Join::enc(pp, msk, x1, 1);
    const auto ct2 = Join::enc(pp, msk, x2, 2);
    const auto sk1 = Join::keygen(pp, msk, y1, sel);
    const auto sk2 = Join::keygen(pp, msk, y2, sel);

    EXPECT_FALSE(Group::cmp_gt(Join::dec(pp, ct1, sk1, sel), Join::dec(pp, ct2, sk2, sel)));
}

TEST(JoinTests, JoinDegMulSelTrue){
    const auto pp = Join::pp_gen(5, 10);
    const auto msk = Join::msk_gen(pp);

    const IntVec sel = {2, 4, 6};
    const IntVec x1 = {1, 100, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec x2 = {6, 3, 100, 4, 5, 6, 7, 8, 9, 10};
    const IntMat y1 = {{0, 1, 2, 3, 4}, {0}, {0, 1, 2, 3, 7}};
    const IntMat y2 = {{0}, {0, 1, 2, 3, 6}, {0, 1, 2, 3, 8}};

    const auto ct1 = Join::enc(pp, msk, x1, 1);
    const auto ct2 = Join::enc(pp, msk, x2, 2);
    const auto sk1 = Join::keygen(pp, msk, y1, sel);
    const auto sk2 = Join::keygen(pp, msk, y2, sel);

    EXPECT_TRUE(Group::cmp_gt(Join::dec(pp, ct1, sk1, sel), Join::dec(pp, ct2, sk2, sel)));
}

TEST(JoinTests, JoinDegMulSelFalse){
    const auto pp = Join::pp_gen(5, 10);
    const auto msk = Join::msk_gen(pp);

    const IntVec sel = {2, 4, 6};
    const IntVec x1 = {1, 100, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec x2 = {6, 3, 100, 4, 5, 6, 7, 8, 9, 10};
    const IntMat y1 = {{1}, {0, 1, 2, 3, 5}, {0, 1, 2, 3, 7}};
    const IntMat y2 = {{0}, {0, 1, 2, 3, 6}, {0, 1, 2, 3, 8}};

    const auto ct1 = Join::enc(pp, msk, x1, 1);
    const auto ct2 = Join::enc(pp, msk, x2, 2);
    const auto sk1 = Join::keygen(pp, msk, y1, sel);
    const auto sk2 = Join::keygen(pp, msk, y2, sel);

    EXPECT_FALSE(Group::cmp_gt(Join::dec(pp, ct1, sk1, sel), Join::dec(pp, ct2, sk2, sel)));
}
