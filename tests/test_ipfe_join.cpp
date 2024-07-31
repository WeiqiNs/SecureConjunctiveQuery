#include "ipfe_join.hpp"

int test_ipfe_join_eq(){
    const auto pp = IpfeJoin::pp_gen(6);
    const auto msk = IpfeJoin::msk_gen(pp, 10);

    const int_vec x1 = {100, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const int_vec x2 = {100, 6, 3, 4, 5, 6, 7, 8, 9, 10};
    const int_mat y1 = {
        int_vec{1, 2}, int_vec{2, 3}, int_vec{3, 4},
        int_vec{4, 5}, int_vec{5, 6}, int_vec{6, 7},
        int_vec{7, 8}, int_vec{8, 9}, int_vec{9, 10}
    };
    const int_mat y2 = {
        int_vec{6, 7}, int_vec{2, 3}, int_vec{3, 4},
        int_vec{4, 5}, int_vec{5, 6}, int_vec{6, 7},
        int_vec{7, 8}, int_vec{8, 9}, int_vec{9, 10}
    };

    const auto ct1 = IpfeJoin::enc(pp, msk, x1);
    const auto ct2 = IpfeJoin::enc(pp, msk, x2);
    const auto sk1 = IpfeJoin::keygen(pp, msk, y1);
    const auto sk2 = IpfeJoin::keygen(pp, msk, y2);

    // Compute the decryption.
    gt r1, r2;
    IpfeJoin::dec(r1, ct1, sk1);
    IpfeJoin::dec(r2, ct2, sk2);

    return Group::cmp_gt(r1, r2);
}

int test_ipfe_join_neq(){
    const auto pp = IpfeJoin::pp_gen(6);
    const auto msk = IpfeJoin::msk_gen(pp, 10);

    const int_vec x1 = {100, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const int_vec x2 = {1, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const int_vec x3 = {100, 100, 3, 4, 5, 6, 7, 8, 9, 10};
    const int_mat y = {
        int_vec{1, 2}, int_vec{2, 3}, int_vec{3, 4},
        int_vec{4, 5}, int_vec{5, 6}, int_vec{6, 7},
        int_vec{7, 8}, int_vec{8, 9}, int_vec{9, 10}
    };

    const auto ct1 = IpfeJoin::enc(pp, msk, x1);
    const auto ct2 = IpfeJoin::enc(pp, msk, x2);
    const auto ct3 = IpfeJoin::enc(pp, msk, x3);
    const auto sk = IpfeJoin::keygen(pp, msk, y);

    // Compute the decryption.
    gt r1, r2, r3;
    IpfeJoin::dec(r1, ct1, sk);
    IpfeJoin::dec(r2, ct2, sk);
    IpfeJoin::dec(r3, ct3, sk);

    return !Group::cmp_gt(r1, r2) && !Group::cmp_gt(r1, r3);
}

int main(){
    // Perform test.
    if (test_ipfe_join_eq() != 1) return 1;
    if (test_ipfe_join_neq() != 1) return 1;

    return 0;
}
