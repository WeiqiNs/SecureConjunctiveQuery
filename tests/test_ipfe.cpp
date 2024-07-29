#include "ipfe.hpp"

int test_ipfe_eq(){
    const auto pp = Ipfe::pp_gen(6);
    const auto msk = Ipfe::msk_gen(pp, 10);

    const int_vec x1 = {100, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const int_vec x2 = {100, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const int_mat y = {
        int_vec{1, 2},
        int_vec{2, 3},
        int_vec{3, 4},
        int_vec{4, 5},
        int_vec{5, 6},
        int_vec{6, 7},
        int_vec{7, 8},
        int_vec{8, 9},
        int_vec{9, 10},
    };

    const auto ct1 = Ipfe::enc(pp, msk, x1);
    const auto ct2 = Ipfe::enc(pp, msk, x2);
    const auto sk = Ipfe::keygen(pp, msk, y);

    return Ipfe::dec(ct1, sk, ct2, sk);
}

int test_ipfe_neq(){
    const auto pp = Ipfe::pp_gen(6);
    const auto msk = Ipfe::msk_gen(pp, 10);

    const int_vec x1 = {100, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const int_vec x2 = {10, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const int_vec x3 = {100, 2, 3, 4, 5, 6, 7, 8, 9, 11};
    const int_mat y = {
        int_vec{1, 2},
        int_vec{2, 3},
        int_vec{3, 4},
        int_vec{4, 5},
        int_vec{5, 6},
        int_vec{6, 7},
        int_vec{7, 8},
        int_vec{8, 9},
        int_vec{9, 10},
    };

    const auto ct1 = Ipfe::enc(pp, msk, x1);
    const auto ct2 = Ipfe::enc(pp, msk, x2);
    const auto ct3 = Ipfe::enc(pp, msk, x3);
    const auto sk = Ipfe::keygen(pp, msk, y);

    return Ipfe::dec(ct1, sk, ct2, sk) == false && Ipfe::dec(ct1, sk, ct3, sk) == false;
}


int main(){
    // Perform test.
    if (test_ipfe_eq() != 1) return 1;
    if (test_ipfe_neq() != 1) return 1;

    return 0;
}
