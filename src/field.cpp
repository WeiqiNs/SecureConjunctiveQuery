#include "field.hpp"

Field::Field(const zp& order){
    init_zp(P);
    bn_copy(P, order);
}

void Field::init(const zp& order){
    init_zp(P);
    bn_copy(P, order);
}

void Field::init_zp(zp& x){
    bn_null(x);
    bn_new(x);
}

void Field::one(zp& x){
    bn_set_dig(x, 1);
}

void Field::zero(zp& x){
    bn_set_dig(x, 0);
}

void Field::copy(zp& r, const zp& x){
    bn_copy(r, x);
}

bool Field::cmp(const zp& x, const zp& y){
    return bn_cmp(x, y) == RLC_EQ;
}

bool Field::cmp(const zp& x, const int& y){
    return bn_cmp_dig(x, y) == RLC_EQ;
}

void Field::set_int(zp& r, const int& x){
    if (x >= 0){
        bn_set_dig(r, x);
    }
    else{
        bn_set_dig(r, abs(x));
        bn_neg(r, r);
    }
}

void Field::from_int(zp& r, const int& x) const{
    if (x >= 0){
        bn_set_dig(r, x);
        mod(r);
    }
    else{
        bn_set_dig(r, abs(x));
        neg(r, r);
    }
}

void Field::mod(zp& x) const{
    bn_mod(x, x, P);
}

void Field::rand(zp& x) const{
    bn_rand_mod(x, P);
}

void Field::add(zp& r, const zp& x, const zp& y) const{
    bn_add(r, x, y);
    mod(r);
}

void Field::mul(zp& r, const zp& x, const zp& y) const{
    bn_mul(r, x, y);
    mod(r);
}

void Field::neg(zp& r, const zp& x) const{
    bn_neg(r, x);
    mod(r);
}

void Field::inv(zp& r, const zp& x) const{
    bn_mod_inv(r, x, P);
}

zp_vec Field::set_int_vec(const int_vec& x){
    zp_vec r(x.size());

    for (int i = 0; i < x.size(); i++){
        init_zp(r[i]);
        set_int(r[i], x[i]);
    }

    return r;
}

zp_vec Field::from_int_vec(const int_vec& x) const{
    zp_vec r(x.size());

    for (int i = 0; i < x.size(); i++){
        init_zp(r[i]);
        from_int(r[i], x[i]);
    }

    return r;
}

zp_vec Field::vec_rand(const int& size) const{
    zp_vec r(size);

    for (int i = 0; i < size; i++){
        init_zp(r[i]);
        rand(r[i]);
    }

    return r;
}

zp_vec Field::vec_add(const zp_vec& x, const zp_vec& y) const{
    // Check that inputs have to have equal length.
    if (x.size() != y.size()){
        throw std::runtime_error("The input vectors are of different lengths.");
    }

    zp_vec r(x.size());

    for (int i = 0; i < x.size(); i++){
        init_zp(r[i]);
        add(r[i], x[i], y[i]);
    }

    return r;
}

zp_vec Field::vec_mul(const zp_vec& x, const zp_vec& y) const{
    // Check that inputs have to have equal length.
    if (x.size() != y.size()){
        throw std::runtime_error("The input vectors are of different lengths.");
    }

    zp_vec r(x.size());

    for (int i = 0; i < x.size(); i++){
        init_zp(r[i]);
        mul(r[i], x[i], y[i]);
    }

    return r;
}

zp_vec Field::vec_mul(const zp_vec& x, const zp& y) const{
    zp_vec r(x.size());

    for (int i = 0; i < x.size(); i++){
        init_zp(r[i]);
        mul(r[i], x[i], y);
    }

    return r;
}

zp_vec Field::vec_inv(const zp_vec& x) const{
    zp_vec r(x.size());

    for (int i = 0; i < x.size(); i++){
        init_zp(r[i]);
        inv(r[i], x[i]);
    }

    return r;
}

void Field::vec_ip(zp& ip, const zp_vec& x, const zp_vec& y) const{
    // Check that inputs have to have equal length.
    if (x.size() != y.size()){
        throw std::runtime_error("The input vectors are of different lengths.");
    }

    zp_vec r = vec_mul(x, y);

    if (x.size() == 1){
        copy(ip, r[0]);
    }
    else{
        zero(ip);
        for (const auto& i : r) add(ip, ip, i);
    }
}

zp_vec Field::vec_join(const zp_vec& x, const zp& y){
    zp_vec r(x.size() + 1);

    for (int i = 0; i < x.size(); i++){
        init_zp(r[i]);
        copy(r[i], x[i]);
    }

    init_zp(r[x.size()]);
    copy(r[x.size()], y);

    return r;
}

zp_vec Field::vec_join(const zp_vec& x, const zp_vec& y){
    zp_vec r(x.size() + y.size());

    for (int i = 0; i < x.size(); i++){
        init_zp(r[i]);
        copy(r[i], x[i]);
    }

    for (int i = 0; i < y.size(); i++){
        init_zp(r[i + x.size()]);
        copy(r[i + x.size()], y[i]);
    }
    return r;
}
