#include "aggre.hpp"

int test_aggre_eq(){
    const auto pp = Aggre::pp_gen();
    const auto msk = Aggre::msk_gen(pp, 10);

    const int_vec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const int_vec y = {0, 1, 1, 1, 1, 1, 1, 1, 2, -3};
    constexpr int p = 17;

    auto ct = Aggre::enc(pp, msk, x);
    auto sk = Aggre::keygen(pp, msk, y, p);

    return Aggre::dec(ct, sk);
}

int test_aggre_neq(){
    const auto pp = Aggre::pp_gen();
    const auto msk = Aggre::msk_gen(pp, 10);

    const int_vec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const int_vec y = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    constexpr int p = 0;

    auto ct = Aggre::enc(pp, msk, x);
    auto sk = Aggre::keygen(pp, msk, y, p);

    return Aggre::dec(ct, sk);
}

int main(){
    // Perform test.
    if (test_aggre_eq() != 1) return 1;
    if (test_aggre_neq() == 1) return 1;

    return 0;
}
