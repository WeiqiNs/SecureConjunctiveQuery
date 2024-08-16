#include <chrono>
#include <iostream>

#include "filter.hpp"

int test_filter_int_eq(){
    const auto pp = Filter::pp_gen(5);
    const auto msk = Filter::msk_gen(pp, 10);

    const int_vec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const int_mat y = {
        int_vec{0, 1}, int_vec{1, 2}, int_vec{2, 3}, int_vec{3, 4}, int_vec{4, 5},
        int_vec{5, 6}, int_vec{6, 7}, int_vec{7, 8}, int_vec{8, 9}, int_vec{9, 10}
    };

    auto ct = Filter::enc(pp, msk, x);
    auto sk = Filter::keygen(pp, msk, y);

    return Filter::dec(ct, sk);
}

int test_filter_int_neq(){
    const auto pp = Filter::pp_gen(5);
    const auto msk = Filter::msk_gen(pp, 10);

    const int_vec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const int_mat y = {
        int_vec{0, 1}, int_vec{1, 2}, int_vec{2, 3}, int_vec{3, 4}, int_vec{4, 5},
        int_vec{5, 6}, int_vec{6, 7}, int_vec{7, 8}, int_vec{8, 9}, int_vec{0, 1}
    };

    auto ct = Filter::enc(pp, msk, x);
    auto sk = Filter::keygen(pp, msk, y);

    return !Filter::dec(ct, sk);
}

int test_filter_str_eq(){
    const auto pp = Filter::pp_gen(5);
    const auto msk = Filter::msk_gen(pp, 5);

    const str_vec x = {"apple", "banana", "cherry", "date", "elderberry"};
    const str_mat y = {
        str_vec{"A", "apple"},
        str_vec{"B", "banana"},
        str_vec{"C", "cherry"},
        str_vec{"D", "date"},
        str_vec{"E", "elderberry"},
    } ;

    auto ct = Filter::enc(pp, msk, x);
    auto sk = Filter::keygen(pp, msk, y);

    return Filter::dec(ct, sk);
}

int test_filter_sel_int_eq(){
    const auto pp = Filter::pp_gen(5);
    const auto msk = Filter::msk_gen(pp, 10);

    const int_vec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const int_mat y = {int_vec{0, 1}, int_vec{2, 3}, int_vec{4, 5}};

    auto ct = Filter::enc(pp, msk, x);
    auto sk = Filter::keygen(pp, msk, y, int_vec{0, 2, 4});

    return Filter::dec(pp, ct, sk, int_vec{0, 2, 4});
}

int main(){
    // Perform test.
    if (test_filter_int_eq() != 1) return 1;
    if (test_filter_int_eq() != 1) return 1;
    if (test_filter_str_eq() != 1) return 1;
    if (test_filter_sel_int_eq() != 1) return 1;

    return 0;
}
