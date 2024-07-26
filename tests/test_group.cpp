#include "group.hpp"

int test_pair(){
    const Group group(false);

    constexpr zp x1{5}, y1{10}, x2{10}, y2{5};
    g1 a1, a2;
    g2 b1, b2;
    gt c1, c2;

    group.g1_raise(a1, x1);
    group.g2_raise(b1, y1);
    Group::pair(c1, a1, b1);

    group.g1_raise(a2, x2);
    group.g2_raise(b2, y2);
    Group::pair(c2, a2, b2);

    return Group::gt_eq(c1, c2);
}

int test_pair_with_pre(){
    const Group group(true);

    constexpr zp x1{5}, y1{10}, x2{10}, y2{5};
    g1 a1, a2;
    g2 b1, b2;
    gt c1, c2;

    group.g1_raise(a1, x1);
    group.g2_raise(b1, y1);
    Group::pair(c1, a1, b1);

    group.g1_raise(a2, x2);
    group.g2_raise(b2, y2);
    Group::pair(c2, a2, b2);

    return Group::gt_eq(c1, c2);
}

int test_pair_vec_with_pre(){
    const Group group(true);

    const int_vec int_x1 = {1, 1, 1, 1};
    const int_vec int_y1 = {2, 3, 4, 5};

    const int_vec int_x2 = {1, 2, 3, 4, 2, 2};
    const int_vec int_y2 = {1, 1, 1, 1, 1, 1};

    const zp_vec x1 = Field::set_int_vec(int_x1);
    const zp_vec y1 = Field::set_int_vec(int_y1);

    const zp_vec x2 = Field::set_int_vec(int_x2);
    const zp_vec y2 = Field::set_int_vec(int_y2);

    gt r1, r2;

    const g1_vec gx1 = group.g1_raise(x1);
    const g2_vec gy1 = group.g2_raise(y1);

    const g1_vec gx2 = group.g1_raise(x2);
    const g2_vec gy2 = group.g2_raise(y2);

    Group::pair(r1, gx1, gy1);
    Group::pair(r2, gx2, gy2);

    return Group::gt_eq(r1, r2);
}

int main() {
    // Get the prime number p, the order of the group.
    zp p;
    init_get_order(p);

    // Perform test.
    if (test_pair() != 1) return 1;
    if (test_pair_with_pre() != 1) return 1;
    if (test_pair_vec_with_pre() != 1) return 1;

    return 0;
}