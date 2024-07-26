#include "filter.hpp"

int test_filter_int_eq(){
    const auto pp = Filter::pp_gen();
    const auto msk = Filter::msk_gen(pp, 10);

    const int_vec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const int_vec y = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    auto ct = Filter::enc(pp, msk, x);
    auto sk = Filter::keygen(pp, msk, y);

    return Filter::dec(ct, sk);
}

int test_filter_int_neq(){
    const auto pp = Filter::pp_gen();
    const auto msk = Filter::msk_gen(pp, 10);

    const int_vec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const int_vec y = {0, 1, 2, 3, 4, 5, 6, 7, 8, 0};

    auto ct = Filter::enc(pp, msk, x);
    auto sk = Filter::keygen(pp, msk, y);

    return Filter::dec(ct, sk);
}

int test_filter_str_eq(){
    const auto pp = Filter::pp_gen();
    const auto msk = Filter::msk_gen(pp, 5);

    const str_vec x = {"apple", "banana", "cherry", "date", "elderberry"};
    const str_vec y = {"apple", "banana", "cherry", "date", "elderberry"};

    auto ct = Filter::enc(pp, msk, x);
    auto sk = Filter::keygen(pp, msk, y);

    return Filter::dec(ct, sk);
}

int test_filter_str_neq(){
    const auto pp = Filter::pp_gen();
    const auto msk = Filter::msk_gen(pp, 5);

    const str_vec x = {"apple", "banana", "cherry", "date", "elderberry"};
    const str_vec y = {"Apple", "banana", "cherry", "date", "elderberry"};

    auto ct = Filter::enc(pp, msk, x);
    auto sk = Filter::keygen(pp, msk, y);

    return Filter::dec(ct, sk);
}

int main(){
    // Perform test.
    if (test_filter_int_eq() != 1) return 1;
    if (test_filter_int_neq() == 1) return 1;
    if (test_filter_str_eq() != 1) return 1;
    if (test_filter_str_neq() == 1) return 1;

    return 0;
}
