#include <gtest/gtest.h>
#include "flexdb_sf.hpp"


TEST(FlexDbSfTests, IntTrue){
    const auto pp = FlexDbSf::pp_gen(10);
    const auto msk = FlexDbSf::msk_gen(pp);

    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    const auto ct = FlexDbSf::enc(pp, msk, x);
    const auto sk = FlexDbSf::keygen(pp, msk, y);

    EXPECT_TRUE(FlexDbSf::dec( ct, sk));
    BP::close();
}

TEST(FlexDbSfTests, IntFalse){
    const auto pp = FlexDbSf::pp_gen(10);
    const auto msk = FlexDbSf::msk_gen(pp);

    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y = {0, 1, 2, 3, 4, 5, 6, 7, 9, 8};

    const auto ct = FlexDbSf::enc(pp, msk, x);
    const auto sk = FlexDbSf::keygen(pp, msk, y);

    EXPECT_FALSE(FlexDbSf::dec( ct, sk));
    BP::close();
}

TEST(FlexDbSfTests, StrTrue){
    const auto pp = FlexDbSf::pp_gen(10);
    const auto msk = FlexDbSf::msk_gen(pp);

    const StrVec x = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    const StrVec y = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

    const auto ct = FlexDbSf::enc(pp, msk, x);
    const auto sk = FlexDbSf::keygen(pp, msk, y);

    EXPECT_TRUE(FlexDbSf::dec( ct, sk));
    BP::close();
}

TEST(FlexDbSfTests, StrFalse){
    const auto pp = FlexDbSf::pp_gen(10);
    const auto msk = FlexDbSf::msk_gen(pp);

    const StrVec x = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    const StrVec y = {"0", "1", "2", "3", "Wrong", "5", "6", "7", "8", "9"};

    const auto ct = FlexDbSf::enc(pp, msk, x);
    const auto sk = FlexDbSf::keygen(pp, msk, y);

    EXPECT_FALSE(FlexDbSf::dec( ct, sk));
    BP::close();
}

TEST(FlexDbSfTests, IntTrueWithSel){
    const auto pp = FlexDbSf::pp_gen(10);
    const auto msk = FlexDbSf::msk_gen(pp);

    const IntVec sel = {1, 3, 5};
    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y = {1, 3, 5};

    const auto ct = FlexDbSf::enc(pp, msk, x);
    const auto sk = FlexDbSf::keygen(pp, msk, y, sel);

    EXPECT_TRUE(FlexDbSf::dec(ct, sk, sel));
    BP::close();
}

TEST(FlexDbSfTests, IntFalseWithSel){
    const auto pp = FlexDbSf::pp_gen(10);
    const auto msk = FlexDbSf::msk_gen(pp);

    const IntVec sel = {1, 3, 5};
    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y = {10, 3, 5};

    const auto ct = FlexDbSf::enc(pp, msk, x);
    const auto sk = FlexDbSf::keygen(pp, msk, y, sel);

    EXPECT_FALSE(FlexDbSf::dec(ct, sk, sel));
    BP::close();
}

TEST(FlexDbSfTests, IntTrueWithCompress){
    const auto pp = FlexDbSf::pp_gen(10);
    const auto msk = FlexDbSf::msk_gen(pp, {}, true);

    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    const auto ct = FlexDbSf::enc(pp, msk, x);
    const auto sk = FlexDbSf::keygen(pp, msk, y);

    EXPECT_TRUE(FlexDbSf::dec( ct, sk));
    BP::close();
}
