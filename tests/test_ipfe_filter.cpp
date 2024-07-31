#include "ipfe_filter.hpp"

int test_ipfe_filter_eq(){
    const auto pp = IpfeFilter::pp_gen(5);
    const auto msk = IpfeFilter::msk_gen(pp, 10);

    const int_vec x1 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const int_vec x2 = {101, 1, 2, 4, 5, 6, 7, 8, 9, 9};
    const int_mat y = {
        int_vec{0, 101}, int_vec{1, 2}, int_vec{2, 3}, int_vec{3, 4}, int_vec{4, 5},
        int_vec{5, 6}, int_vec{6, 7}, int_vec{7, 8}, int_vec{8, 9}, int_vec{9, 10}
    };

    const auto ct1 = IpfeFilter::enc(pp, msk, x1);
    const auto ct2 = IpfeFilter::enc(pp, msk, x2);
    const auto sk = IpfeFilter::keygen(pp, msk, y);

    return IpfeFilter::dec(ct1, sk) && IpfeFilter::dec(ct2, sk);
}

int test_ipfe_filter_neq(){
    const auto pp = IpfeFilter::pp_gen(5);
    const auto msk = IpfeFilter::msk_gen(pp, 10);

    const int_vec x1 = {10, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const int_vec x2 = {101, 1, 2, 4, 5, 6, 7, 8, 9, 101};
    const int_mat y = {
        int_vec{0, 101}, int_vec{1, 2}, int_vec{2, 3}, int_vec{3, 4}, int_vec{4, 5},
        int_vec{5, 6}, int_vec{6, 7}, int_vec{7, 8}, int_vec{8, 9}, int_vec{9, 10}
    };

    const auto ct1 = IpfeFilter::enc(pp, msk, x1);
    const auto ct2 = IpfeFilter::enc(pp, msk, x2);
    const auto sk = IpfeFilter::keygen(pp, msk, y);

    return !IpfeFilter::dec(ct1, sk) && !IpfeFilter::dec(ct2, sk);
}

int main(){
    // Perform test.
    if (test_ipfe_filter_eq() != 1) return 1;
    if (test_ipfe_filter_neq() != 1) return 1;

    return 0;
}
