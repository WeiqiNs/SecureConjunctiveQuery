#include "join.hpp"

int test_join_eq(){
    const auto pp = Join::pp_gen(5);
    const auto msk = Join::msk_gen(pp, 10);

    const int_vec x1 = {0, 0, 1, 2, 3, 4, 5, 6, 7, 8};
    const int_vec x2 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const int_mat y = {
        int_vec{0, 1}, int_vec{1, 2}, int_vec{2, 3}, int_vec{3, 4}, int_vec{4, 5},
        int_vec{5, 6}, int_vec{6, 7}, int_vec{7, 8}, int_vec{8, 9}
    };

    const auto ct1 = Join::enc(pp, msk, x1);
    const auto ct2 = Join::enc(pp, msk, x2);
    const auto sk = Join::keygen(pp, msk, y);

    gt r1, r2;
    Join::dec(r1, ct1, sk);
    Join::dec(r2, ct2, sk);

    return Group::cmp_gt(r1, r2);
}

int test_join_neq(){
    const auto pp = Join::pp_gen(5);
    const auto msk = Join::msk_gen(pp, 10);

    const int_vec x1 = {0, 0, 1, 2, 3, 4, 5, 6, 7, 8};
    const int_vec x2 = {1, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const int_vec x3 = {0, 0, 1, 2, 3, 4, 5, 6, 7, 0};
    const int_mat y = {
        int_vec{0, 1}, int_vec{1, 2}, int_vec{2, 3}, int_vec{3, 4}, int_vec{4, 5},
        int_vec{5, 6}, int_vec{6, 7}, int_vec{7, 8}, int_vec{8, 9}
    };

    const auto ct1 = Join::enc(pp, msk, x1);
    const auto ct2 = Join::enc(pp, msk, x2);
    const auto ct3 = Join::enc(pp, msk, x3);
    const auto sk = Join::keygen(pp, msk, y);

    gt r1, r2, r3;
    Join::dec(r1, ct1, sk);
    Join::dec(r2, ct2, sk);

    return !Group::cmp_gt(r1, r2) && !Group::cmp_gt(r1, r3);
}

int main(){
    // Perform test.
    if (test_join_eq() != 1) return 1;
    if (test_join_neq() != 1) return 1;

    return 0;
}
