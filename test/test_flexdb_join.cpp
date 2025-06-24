#include <gtest/gtest.h>
#include "flexdb_join.hpp"


TEST(FlexDbEjTests, JoinDegOneTrue){
    const auto pp = FlexDbEj::pp_gen(1, 10);
    const auto msk = FlexDbEj::msk_gen(pp);

    const IntVec x1 = {1, 100, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec x2 = {1, 3, 100, 4, 5, 6, 7, 8, 9, 10};
    const IntVec y1 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y2 = {1, 3, 4, 5, 6, 7, 8, 9, 10};

    const auto ct1 = FlexDbEj::enc(pp, msk, x1, 1);
    const auto ct2 = FlexDbEj::enc(pp, msk, x2, 2);
    const auto sk1 = FlexDbEj::keygen(pp, msk, y1);
    const auto sk2 = FlexDbEj::keygen(pp, msk, y2);

    EXPECT_TRUE(Group::cmp_gt(FlexDbEj::dec(pp, ct1, sk1), FlexDbEj::dec(pp, ct2, sk2)));
}

TEST(FlexDbEjTests, JoinDegOneTrueWithCompress){
    const auto pp = FlexDbEj::pp_gen(1, 10);
    const auto msk = FlexDbEj::msk_gen(pp, {}, true);

    const IntVec x1 = {1, 100, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec x2 = {1, 3, 100, 4, 5, 6, 7, 8, 9, 10};
    const IntVec y1 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y2 = {1, 3, 4, 5, 6, 7, 8, 9, 10};

    const auto ct1 = FlexDbEj::enc(pp, msk, x1, 1);
    const auto ct2 = FlexDbEj::enc(pp, msk, x2, 2);
    const auto sk1 = FlexDbEj::keygen(pp, msk, y1);
    const auto sk2 = FlexDbEj::keygen(pp, msk, y2);

    EXPECT_TRUE(Group::cmp_gt(FlexDbEj::dec(pp, ct1, sk1), FlexDbEj::dec(pp, ct2, sk2)));
}

TEST(FlexDbEjTests, JoinDegOneStr){
    const auto pp = FlexDbEj::pp_gen(1, 7);
    const auto msk = FlexDbEj::msk_gen(pp);

    const StrVec x1 = {"HERE", "a", "b", "c", "d", "e", "f"};
    const StrVec x2 = {"a", "b", "c", "d", "e", "f", "HERE"};
    const StrVec y = {"a", "b", "c", "d", "e", "f"};

    const auto ct1 = FlexDbEj::enc(pp, msk, x1, 0);
    const auto ct2 = FlexDbEj::enc(pp, msk, x2, 6);
    const auto sk = FlexDbEj::keygen(pp, msk, y);

    EXPECT_TRUE(Group::cmp_gt(FlexDbEj::dec(pp, ct1, sk), FlexDbEj::dec(pp, ct2, sk)));
}

TEST(FlexDbEjTests, JoinDegOneFalseJoin){
    const auto pp = FlexDbEj::pp_gen(1, 10);
    const auto msk = FlexDbEj::msk_gen(pp);

    const IntVec x1 = {1, 100, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec x2 = {6, 3, 1, 4, 5, 6, 7, 8, 9, 10};
    const IntVec y1 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y2 = {6, 3, 4, 5, 6, 7, 8, 9, 10};

    const auto ct1 = FlexDbEj::enc(pp, msk, x1, 1);
    const auto ct2 = FlexDbEj::enc(pp, msk, x2, 2);
    const auto sk1 = FlexDbEj::keygen(pp, msk, y1);
    const auto sk2 = FlexDbEj::keygen(pp, msk, y2);

    EXPECT_FALSE(Group::cmp_gt(FlexDbEj::dec(pp, ct1, sk1), FlexDbEj::dec(pp, ct2, sk2)));
}

TEST(FlexDbEjTests, JoinDegOneFalseFilter){
    const auto pp = FlexDbEj::pp_gen(1, 10);
    const auto msk = FlexDbEj::msk_gen(pp);

    const IntVec x1 = {1, 100, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec x2 = {6, 3, 100, 4, 5, 6, 7, 8, 9, 10};
    const IntVec y1 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y2 = {6, 3, 4, 5, 6, 7, 8, 9, 1};

    const auto ct1 = FlexDbEj::enc(pp, msk, x1, 1);
    const auto ct2 = FlexDbEj::enc(pp, msk, x2, 2);
    const auto sk1 = FlexDbEj::keygen(pp, msk, y1);
    const auto sk2 = FlexDbEj::keygen(pp, msk, y2);

    EXPECT_FALSE(Group::cmp_gt(FlexDbEj::dec(pp, ct1, sk1), FlexDbEj::dec(pp, ct2, sk2)));
}

TEST(FlexDbEjTests, JoinDegMulTrue){
    const auto pp = FlexDbEj::pp_gen(5, 10);
    const auto msk = FlexDbEj::msk_gen(pp);

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
            {6},
            {3, 4},
            {2, 3, 4},
            {1, 2, 3, 5},
            {0, 1, 2, 3, 6},
            {0, 1, 2, 3, 7},
            {0, 1, 2, 3, 8},
            {0, 1, 2, 3, 9},
            {0, 1, 2, 3, 10}
        };

    const auto ct1 = FlexDbEj::enc(pp, msk, x1, 1);
    const auto ct2 = FlexDbEj::enc(pp, msk, x2, 2);
    const auto sk1 = FlexDbEj::keygen(pp, msk, y1);
    const auto sk2 = FlexDbEj::keygen(pp, msk, y2);

    EXPECT_TRUE(Group::cmp_gt(FlexDbEj::dec(pp, ct1, sk1), FlexDbEj::dec(pp, ct2, sk2)));
}

TEST(FlexDbEjTests, JoinDegMulFalseJoin){
    const auto pp = FlexDbEj::pp_gen(5, 10);
    const auto msk = FlexDbEj::msk_gen(pp);

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
            {6},
            {3, 4},
            {2, 3, 4},
            {1, 2, 3, 5},
            {0, 1, 2, 3, 6},
            {0, 1, 2, 3, 7},
            {0, 1, 2, 3, 8},
            {0, 1, 2, 3, 9},
            {0, 1, 2, 3, 10}
        };

    const auto ct1 = FlexDbEj::enc(pp, msk, x1, 1);
    const auto ct2 = FlexDbEj::enc(pp, msk, x2, 2);
    const auto sk1 = FlexDbEj::keygen(pp, msk, y1);
    const auto sk2 = FlexDbEj::keygen(pp, msk, y2);

    EXPECT_FALSE(Group::cmp_gt(FlexDbEj::dec(pp, ct1, sk1), FlexDbEj::dec(pp, ct2, sk2)));
}

TEST(FlexDbEjTests, JoinDegMulFalseFilter){
    const auto pp = FlexDbEj::pp_gen(5, 10);
    const auto msk = FlexDbEj::msk_gen(pp);

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
            {6},
            {3, 4},
            {2, 3, 4},
            {1, 2, 3, 5},
            {0, 1, 2, 3, 6},
            {0, 1, 2, 3, 7},
            {0, 1, 2, 3, 8},
            {0, 1, 2, 3, 9},
            {0, 1, 2, 3, 10}
        };

    const auto ct1 = FlexDbEj::enc(pp, msk, x1, 1);
    const auto ct2 = FlexDbEj::enc(pp, msk, x2, 2);
    const auto sk1 = FlexDbEj::keygen(pp, msk, y1);
    const auto sk2 = FlexDbEj::keygen(pp, msk, y2);

    EXPECT_FALSE(Group::cmp_gt(FlexDbEj::dec(pp, ct1, sk1), FlexDbEj::dec(pp, ct2, sk2)));
}

TEST(FlexDbEjTests, JoinDegOneSelTrue){
    const auto pp = FlexDbEj::pp_gen(1, 10);
    const auto msk = FlexDbEj::msk_gen(pp);

    const IntVec sel = {2, 4, 6};
    const IntVec x1 = {1, 100, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec x2 = {6, 3, 100, 4, 5, 6, 7, 8, 9, 10};
    const IntVec y1 = {3, 5, 7};
    const IntVec y2 = {4, 6, 8};

    const auto ct1 = FlexDbEj::enc(pp, msk, x1, 1);
    const auto ct2 = FlexDbEj::enc(pp, msk, x2, 2);
    const auto sk1 = FlexDbEj::keygen(pp, msk, y1, sel);
    const auto sk2 = FlexDbEj::keygen(pp, msk, y2, sel);

    EXPECT_TRUE(Group::cmp_gt(FlexDbEj::dec(pp, ct1, sk1, sel), FlexDbEj::dec(pp, ct2, sk2, sel)));
}

TEST(FlexDbEjTests, JoinDegOneSelFalse){
    const auto pp = FlexDbEj::pp_gen(1, 10);
    const auto msk = FlexDbEj::msk_gen(pp);

    const IntVec sel = {2, 4, 6};
    const IntVec x1 = {1, 100, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec x2 = {6, 3, 1, 4, 5, 6, 7, 8, 9, 10};
    const IntVec y1 = {3, 5, 7};
    const IntVec y2 = {4, 6, 8};

    const auto ct1 = FlexDbEj::enc(pp, msk, x1, 1);
    const auto ct2 = FlexDbEj::enc(pp, msk, x2, 2);
    const auto sk1 = FlexDbEj::keygen(pp, msk, y1, sel);
    const auto sk2 = FlexDbEj::keygen(pp, msk, y2, sel);

    EXPECT_FALSE(Group::cmp_gt(FlexDbEj::dec(pp, ct1, sk1, sel), FlexDbEj::dec(pp, ct2, sk2, sel)));
}

TEST(FlexDbEjTests, JoinDegMulSelTrue){
    const auto pp = FlexDbEj::pp_gen(5, 10);
    const auto msk = FlexDbEj::msk_gen(pp);

    const IntVec sel = {2, 4, 6};
    const IntVec x1 = {1, 100, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec x2 = {6, 3, 100, 4, 5, 6, 7, 8, 9, 10};
    const IntMat y1 = {{1, 2, 3}, {3, 4, 5, 6}, {5, 6, 7, 8, 9}};
    const IntMat y2 = {{2, 3, 4}, {6}, {8}};

    const auto ct1 = FlexDbEj::enc(pp, msk, x1, 1);
    const auto ct2 = FlexDbEj::enc(pp, msk, x2, 2);
    const auto sk1 = FlexDbEj::keygen(pp, msk, y1, sel);
    const auto sk2 = FlexDbEj::keygen(pp, msk, y2, sel);

    EXPECT_TRUE(Group::cmp_gt(FlexDbEj::dec(pp, ct1, sk1, sel), FlexDbEj::dec(pp, ct2, sk2, sel)));
}

TEST(FlexDbEjTests, JoinDegMulSelTrueWithCompress){
    const auto pp = FlexDbEj::pp_gen(5, 10);
    const auto msk = FlexDbEj::msk_gen(pp, {}, true);

    const IntVec sel = {2, 4, 6};
    const IntVec x1 = {1, 100, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec x2 = {6, 3, 100, 4, 5, 6, 7, 8, 9, 10};
    const IntMat y1 = {{1, 2, 3}, {3, 4, 5, 6}, {5, 6, 7, 8, 9}};
    const IntMat y2 = {{2, 3, 4}, {6}, {8}};

    const auto ct1 = FlexDbEj::enc(pp, msk, x1, 1);
    const auto ct2 = FlexDbEj::enc(pp, msk, x2, 2);
    const auto sk1 = FlexDbEj::keygen(pp, msk, y1, sel);
    const auto sk2 = FlexDbEj::keygen(pp, msk, y2, sel);

    EXPECT_TRUE(Group::cmp_gt(FlexDbEj::dec(pp, ct1, sk1, sel), FlexDbEj::dec(pp, ct2, sk2, sel)));
}

TEST(FlexDbEjTests, JoinDegMulSelFalse){
    const auto pp = FlexDbEj::pp_gen(5, 10);
    const auto msk = FlexDbEj::msk_gen(pp);

    const IntVec sel = {2, 4, 6};
    const IntVec x1 = {1, 100, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec x2 = {6, 3, 100, 4, 5, 6, 7, 8, 9, 10};
    const IntMat y1 = {{1, 2, 3}, {3, 4, 5, 6}, {5, 6, 7, 8, 9}};
    const IntMat y2 = {{2, 3, 4}, {6}, {9}};

    const auto ct1 = FlexDbEj::enc(pp, msk, x1, 1);
    const auto ct2 = FlexDbEj::enc(pp, msk, x2, 2);
    const auto sk1 = FlexDbEj::keygen(pp, msk, y1, sel);
    const auto sk2 = FlexDbEj::keygen(pp, msk, y2, sel);

    EXPECT_FALSE(Group::cmp_gt(FlexDbEj::dec(pp, ct1, sk1, sel), FlexDbEj::dec(pp, ct2, sk2, sel)));
}

TEST(FlexDbEjTests, JoinDifferentLengthTrue){
    // Generate two set of pp and msk.
    const auto pp1 = FlexDbEj::pp_gen(1, 10);
    const auto msk1 = FlexDbEj::msk_gen(pp1);
    const auto pp2 = FlexDbEj::pp_gen(1, 8);
    auto msk2 = FlexDbEj::msk_gen(pp2, msk1.hmac->get_key());

    // We need to unify the k used. This k should be updated for each query.
    msk2.k = msk1.k;

    const IntVec x1 = {1, 100, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y1 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    const auto ct1 = FlexDbEj::enc(pp1, msk1, x1, 1);
    const auto sk1 = FlexDbEj::keygen(pp1, msk1, y1);

    const IntVec x2 = {6, 3, 100, 4, 5, 6, 7, 8};
    const IntVec y2 = {6, 3, 4, 5, 6, 7, 8};
    const auto ct2 = FlexDbEj::enc(pp2, msk2, x2, 2);
    const auto sk2 = FlexDbEj::keygen(pp2, msk2, y2);

    EXPECT_TRUE(Group::cmp_gt(FlexDbEj::dec(pp1, ct1, sk1), FlexDbEj::dec(pp2, ct2, sk2)));
}

TEST(FlexDbEjTests, JoinDifferentLengthTrueWithCompress){
    // Generate two set of pp and msk.
    const auto pp1 = FlexDbEj::pp_gen(1, 10);
    const auto msk1 = FlexDbEj::msk_gen(pp1, {}, true);
    const auto pp2 = FlexDbEj::pp_gen(1, 8);
    auto msk2 = FlexDbEj::msk_gen(pp2, msk1.hmac->get_key(), true);

    // We need to unify the k used. This k should be updated for each query.
    msk2.k_int = msk1.k_int;

    const IntVec x1 = {1, 100, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y1 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    const auto ct1 = FlexDbEj::enc(pp1, msk1, x1, 1);
    const auto sk1 = FlexDbEj::keygen(pp1, msk1, y1);

    const IntVec x2 = {6, 3, 100, 4, 5, 6, 7, 8};
    const IntVec y2 = {6, 3, 4, 5, 6, 7, 8};
    const auto ct2 = FlexDbEj::enc(pp2, msk2, x2, 2);
    const auto sk2 = FlexDbEj::keygen(pp2, msk2, y2);

    EXPECT_TRUE(Group::cmp_gt(FlexDbEj::dec(pp1, ct1, sk1), FlexDbEj::dec(pp2, ct2, sk2)));
}