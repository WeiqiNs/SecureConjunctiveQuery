#include "helper.hpp"

int test_int_conversion() {
    constexpr int x = 10;

    const char_vec x_vec = Helper::int_to_vec(x);

    return Helper::vec_to_int(x_vec) == x;
}

int test_neg_int_conversion() {
    constexpr int x = -100;

    const char_vec x_vec = Helper::int_to_vec(x);

    return Helper::vec_to_int(x_vec) == x;
}

int test_str_conversion(){
    const str x = "This is a test.";

    const char_vec x_vec = Helper::str_to_vec(x);

    return Helper::vec_to_str(x_vec) == x;
}

int test_zp_conversion(){
    const char_vec x_vec = {0x12, 0x34};

    zp x;
    Field::init_zp(x);

    Helper::vec_to_zp(x, x_vec);

    return Field::cmp(x, 4660);

}


int main() {
    // Perform test.
    if (test_int_conversion() != 1) return 1;
    if (test_neg_int_conversion() != 1) return 1;
    if (test_str_conversion() != 1) return 1;
    if (test_zp_conversion() != 1) return 1;

    return 0;
}