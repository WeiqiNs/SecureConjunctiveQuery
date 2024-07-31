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

int test_zp_sub(){
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
    zp_field.sub(r, x, y);

    return Field::cmp(r, 6);
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

    return Field::cmp(r, 6);
}

int test_zp_exp(){
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
    zp_field.exp(r, x, y);

    return Field::cmp(r, 1);
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

int test_zp_vec_sub(){
    // Create a prime field.
    zp p;
    Field::init_zp(p);
    Field::set_int(p, 11);
    const Field zp_field(p);

    const int_vec int_x = {1, 2, 3, 4, 5};
    const int_vec int_y = {6, 7, 8, 9, 10};

    const zp_vec zp_x = zp_field.from_int_vec(int_x);
    const zp_vec zp_y = zp_field.from_int_vec(int_y);
    const zp_vec r = zp_field.vec_sub(zp_x, zp_y);

    return Field::cmp(r[3], 6);
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
    zp p;
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

int test_zp_vec_join(){
    // Create a prime field.
    zp p;
    Field::init_zp(p);
    Field::set_int(p, 11);
    const Field zp_field(p);

    const int_vec int_x = {1, 2, 3, 4, 5};
    const int_vec int_y = {6, 7, 8, 9, 10};

    const zp_vec zp_x = zp_field.from_int_vec(int_x);
    const zp_vec zp_y = zp_field.from_int_vec(int_y);
    const zp_vec zp_r = Field::vec_join(zp_x, zp_y);

    return Field::cmp(zp_r[8], 9);
}

int test_zp_mat_from(){
    // Create a prime field.
    zp p;
    Field::init_zp(p);
    Field::set_int(p, 11);
    const Field zp_field(p);

    const int_vec int_x = {1, 2, 3, 4, 5};
    const int_vec int_y = {6, 7, 8, 9, 10};
    const int_vec int_z = {11, 12, 13, 14, 15};
    const int_mat r = {int_x, int_y, int_z};

    const zp_mat zp_r = zp_field.from_int_mat(r);

    return Field::cmp(zp_r[2][2], 2);
}

int test_zp_mat_id(){
    const zp_mat id = Field::mat_id(10);
    return Field::mat_is_id(id);
}

int test_zp_mat_transpose(){
    const int_vec int_x = {1, 2, 3, 4, 5};
    const int_vec int_y = {6, 7, 8, 9, 10};
    const int_vec int_z = {11, 12, 13, 14, 15};
    const int_mat r = {int_x, int_y, int_z};

    const zp_mat zp_r = Field::set_int_mat(r);
    const zp_mat zp_r_t = Field::mat_transpose(zp_r);

    return Field::cmp(zp_r_t[4][2], 15);
}

int test_zp_mat_join(){
    const int_vec int_x = {1, 2, 3, 4, 5};
    const int_vec int_y = {6, 7, 8, 9, 10};
    const int_vec int_z = {11, 12, 13, 14, 15};
    const int_mat r = {int_x, int_y, int_z};
    const int_mat p = {int_z, int_y, int_x};

    const zp_mat zp_r = Field::set_int_mat(r);
    const zp_mat zp_p = Field::set_int_mat(p);
    const zp_mat zp_rp = Field::mat_join(zp_r, zp_p);

    return Field::cmp(zp_rp[2][5], 1);
}

int test_zp_mat_mul_mat(){
    // Create a prime field.
    zp p;
    Field::init_zp(p);
    Field::set_int(p, 23);
    const Field zp_field(p);

    const int_vec int_x = {1, 2, 3, 4, 5};
    const int_vec int_y = {6, 7, 8, 9, 10};
    const int_vec int_z = {11, 12, 13, 14, 15};
    const int_mat x = {int_x, int_y, int_z};

    const int_vec int_a = {1, 2, 3, 4};
    const int_mat y = {int_a, int_a, int_a, int_a, int_a};

    const zp_mat zp_x = zp_field.from_int_mat(x);
    const zp_mat zp_y = zp_field.from_int_mat(y);
    const zp_mat zp_r = zp_field.mat_mul(zp_x, zp_y);

    return Field::cmp(zp_r[2][3], 7);
}

int test_zp_mat_mul_zp(){
    // Create a prime field.
    zp p;
    Field::init_zp(p);
    Field::set_int(p, 11);
    const Field zp_field(p);

    zp c;
    Field::init_zp(c);
    zp_field.from_int(c, 5);

    const int_vec int_x = {1, 2, 3, 4, 5};
    const int_vec int_y = {6, 7, 8, 9, 10};
    const int_vec int_z = {11, 12, 13, 14, 15};
    const int_mat r = {int_x, int_y, int_z};

    const zp_mat zp_r = zp_field.from_int_mat(r);
    const zp_mat zp_cr = zp_field.mat_mul(zp_r, c);

    return Field::cmp(zp_cr[2][4], 9);
}

int test_zp_mat_inv(){
    // Create a prime field.
    zp p;
    Field::init_zp(p);
    Field::set_int(p, 101);
    const Field zp_field(p);

    zp det;
    Field::init_zp(det);

    const int_vec int_x = {3, 5, 8};
    const int_vec int_y = {2, 2, 2};
    const int_vec int_z = {9, 9, 3};
    const int_mat r = {int_x, int_y, int_z};

    const zp_mat zp_r = zp_field.from_int_mat(r);
    const zp_mat zp_ri = zp_field.mat_inv_with_det(zp_r, det);
    const zp_mat rri = zp_field.mat_mul(zp_r, zp_ri);

    return Field::mat_is_id(rri) && Field::cmp(det, 24);
}

int test_zp_find_coeff(){
    // Create a prime field.
    zp p;
    Field::init_zp(p);
    Field::set_int(p, 101);
    const Field zp_field(p);

    const int_vec int_x = {2, 4, 6, 8, 10};
    const zp_vec x = zp_field.from_int_vec(int_x);
    const zp_vec coeff = zp_field.find_coeff(10, x);

    return Field::cmp(coeff[0], 99) &&
        Field::cmp(coeff[3], 37) &&
        Field::cmp(coeff[5], 1) &&
        Field::cmp(coeff[9], 0);
}

int main(){
    // Get the prime number p, the order of the group.
    zp p;
    init_get_order(p);

    // Perform test.
    if (test_zp_one() != 1) return 1;
    if (test_zp_zero() != 1) return 1;
    if (test_zp_copy() != 1) return 1;
    if (test_zp_set() != 1) return 1;
    if (test_zp_from() != 1) return 1;
    if (test_zp_add() != 1) return 1;
    if (test_zp_sub() != 1) return 1;
    if (test_zp_neg() != 1) return 1;
    if (test_zp_mul() != 1) return 1;
    if (test_zp_exp() != 1) return 1;
    if (test_zp_inv() != 1) return 1;
    if (test_zp_vec_add() != 1) return 1;
    if (test_zp_vec_sub() != 1) return 1;
    if (test_zp_vec_mul() != 1) return 1;
    if (test_zp_vec_mul_zp() != 1) return 1;
    if (test_zp_vec_ip() != 1) return 1;
    if (test_zp_vec_join() != 1) return 1;
    if (test_zp_mat_from() != 1) return 1;
    if (test_zp_mat_id() != 1) return 1;
    if (test_zp_mat_transpose() != 1) return 1;
    if (test_zp_mat_join() != 1) return 1;
    if (test_zp_mat_mul_mat() != 1) return 1;
    if (test_zp_mat_mul_zp() != 1) return 1;
    if (test_zp_mat_inv() != 1) return 1;
    if (test_zp_find_coeff() != 1) return 1;

    return 0;
}
