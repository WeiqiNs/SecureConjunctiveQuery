#include "ipfe.hpp"

int test_aggre_eq(){
    const auto pp = Ipfe::pp_gen(6);
    const auto msk = Ipfe::msk_gen(pp, 10);

    const int_vec x1 = {100, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const int_vec x2 = {100, 1, 32, 3, 4, 5, 6, 7, 64, 81};
    const int_vec y = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    auto ct1 = Ipfe::enc(pp, msk, x1);
    auto sk1 = Ipfe::keygen(pp, msk, y);
    auto ct2 = Ipfe::enc(pp, msk, x2);
    auto sk2 = Ipfe::keygen(pp, msk, y);

    return Ipfe::dec(ct1, sk1, ct2, sk2);
}


int main(){
    // Perform test.
    if (test_aggre_eq() != 1) return 1;

    return 0;
}
