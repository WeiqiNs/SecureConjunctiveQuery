#include "field.hpp"

int test_zp_one(){
    zp x;
    Field::init_zp(x);
    Field::one(x);
    return Field::cmp(x, 1);
}

int test_zp_zero(){
    zp x;
    Field::init_zp(x);
    Field::zero(x);
    return Field::cmp(x, 0);
}

int test_zp_copy(){
    // Create a prime field.
    zp p;
    Field::init_zp(p);
    Field::set_int(p, 11);
    const Field zp_field(p);

    // Perform testing.
    zp x, y;
    Field::init_zp(x);
    Field::init_zp(y);
    zp_field.from_int(x, 20);
    Field::copy(y, x);

    return Field::cmp(y, 9);
}

int test_zp_set(){
    zp x;
    Field::init_zp(x);
    Field::set_int(x, 10);
    return Field::cmp(x, 10);
}

int test_zp_from(){
    // Create a prime field.
    zp p;
    Field::init_zp(p);
    Field::set_int(p, 11);
    const Field zp_field(p);

    // Perform testing.
    zp x;
    Field::init_zp(x);
    zp_field.from_int(x, -10);

    return Field::cmp(x, 1);
}

int test_zp_add(){
    // Create a prime field.
    zp p;
    Field::init_zp(p);
    Field::set_int(p, 11);
    const Field zp_field(p);

    // Perform testing.
    zp x, y, r;
    Field::init_zp(x);
    Field::init_zp(y);
    Field::init_zp(r);
    zp_field.from_int(x, 5);
    zp_field.from_int(y, 10);
    zp_field.add(r, x, y);

    return Field::cmp(r, 4);
}

int test_zp_neg(){
    // Create a prime field.
    zp p;
    Field::init_zp(p);
    Field::set_int(p, 11);
    const Field zp_field(p);

    // Perform testing.
    zp x, y, r;
    Field::init_zp(x);
    Field::init_zp(y);
    Field::init_zp(r);
    zp_field.from_int(x, 5);
    zp_field.neg(y, x);
    zp_field.add(r, x, y);

    return Field::cmp(r, 0);
}

int test_zp_mul(){
    // Create a prime field.
    zp p;
    Field::init_zp(p);
    Field::set_int(p, 11);
    const Field zp_field(p);

    // Perform testing.
    zp x, y, r;
    Field::init_zp(x);
    Field::init_zp(y);
    Field::init_zp(r);
    zp_field.from_int(x, 5);
    zp_field.from_int(y, 10);
    zp_field.mul(r, x, y);

    return Field::cmp( r, 6);
}

int test_zp_inv(){
    // Create a prime field.
    zp p;
    Field::init_zp(p);
    Field::set_int(p, 11);
    const Field zp_field(p);

    // Perform testing.
    zp x, y, r;
    Field::init_zp(x);
    Field::init_zp(y);
    Field::init_zp(r);
    zp_field.from_int(x, 5);
    zp_field.inv(y, x);
    zp_field.mul(r, x, y);

    return Field::cmp(r, 1);
}

int test_zp_vec_add(){
    // Create a prime field.
    zp p;
    Field::init_zp(p);
    Field::set_int(p, 11);
    const Field zp_field(p);

    const int_vec int_x = {1, 2, 3, 4, 5};
    const int_vec int_y = {6, 7, 8, 9, 10};

    const zp_vec zp_x = zp_field.from_int_vec(int_x);
    const zp_vec zp_y = zp_field.from_int_vec(int_y);
    const zp_vec r = zp_field.vec_add(zp_x, zp_y);

    return Field::cmp(r[3], 2);
}

int test_zp_vec_mul(){
    // Create a prime field.
    zp p;
    Field::init_zp(p);
    Field::set_int(p, 11);
    const Field zp_field(p);

    const int_vec int_x = {1, 2, 3, 4, 5};
    const int_vec int_y = {6, 7, 8, 9, 10};

    const zp_vec zp_x = zp_field.from_int_vec(int_x);
    const zp_vec zp_y = zp_field.from_int_vec(int_y);
    const zp_vec r = zp_field.vec_mul(zp_x, zp_y);

    return Field::cmp(r[3], 3);
}

int test_zp_vec_mul_zp(){
    // Create a prime field.
    zp p;
    Field::init_zp(p);
    Field::set_int(p, 11);
    const Field zp_field(p);

    zp r;
    Field::init_zp(r);
    zp_field.from_int(r, 5);

    const int_vec int_x = {1, 2, 3, 4, 5};
    const zp_vec zp_x = zp_field.from_int_vec(int_x);
    const zp_vec zp_r = zp_field.vec_mul(zp_x, r);

    return Field::cmp(zp_r[3], 9);
}

int test_zp_vec_ip(){
    // Create a prime field.
    zp p{};
    Field::init_zp(p);
    Field::set_int(p, 11);
    const Field zp_field(p);

    zp ip;
    Field::init_zp(ip);

    const int_vec int_x = {1, 2, 3, 4, 5};
    const int_vec int_y = {6, 7, 8, 9, 10};

    const zp_vec zp_x = zp_field.from_int_vec(int_x);
    const zp_vec zp_y = zp_field.from_int_vec(int_y);
    zp_field.vec_ip(ip, zp_x, zp_y);

    return Field::cmp(ip, 9);
}

int main(){
    // Get the prime number p, the order of the group.
    zp p;
    init_get_order(p);

    // Perform test.
    if (test_zp_zero() != 1) return 1;
    if (test_zp_one() != 1) return 1;
    if (test_zp_copy() != 1) return 1;
    if (test_zp_set() != 1) return 1;
    if (test_zp_from() != 1) return 1;
    if (test_zp_add() != 1) return 1;
    if (test_zp_neg() != 1) return 1;
    if (test_zp_mul() != 1) return 1;
    if (test_zp_inv() != 1) return 1;
    if (test_zp_vec_add() != 1) return 1;
    if (test_zp_vec_mul() != 1) return 1;
    if (test_zp_vec_mul_zp() != 1) return 1;
    if (test_zp_vec_ip() != 1) return 1;

    return 0;
}
