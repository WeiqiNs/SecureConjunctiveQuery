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

zp_mat Field::set_int_mat(const int_mat& x){
    zp_mat r(x.size());

    for (int i = 0; i < x.size(); i++) r[i] = set_int_vec(x[i]);

    return r;
}

zp_mat Field::from_int_mat(const int_mat& x) const{
    zp_mat r(x.size());

    for (int i = 0; i < x.size(); i++) r[i] = from_int_vec(x[i]);

    return r;
}

zp_mat Field::mat_rand(const int& row, const int& col) const{
    zp_mat r(row);

    for (auto& i : r) i = vec_rand(col);

    return r;
}

zp_mat Field::mat_id(const int& size){
    zp_mat r(size);

    for (int i = 0; i < size; i++){
        int_vec temp;
        for (int j = 0; j < size; j++){
            if (i == j) temp.push_back(1);
            else temp.push_back(0);
        }
        r[i] = set_int_vec(temp);
    }

    return r;
}

bool Field::mat_is_id(const zp_mat& x){
    const auto size = x.size();

    for (int i = 0; i < size; i++){
        if (x[i].size() != size) return false;

        for (int j = 0; j < size; j++){
            if (i == j && !cmp(x[i][j], 1)) return false;
            if (i != j && !cmp(x[i][j], 0)) return false;
        }
    }

    return true;
}

zp_mat Field::mat_transpose(const zp_mat& x){
    const auto col = x.size();
    const auto row = x[0].size();

    zp_mat r(row);
    const int_vec temp(col);

    for (int i = 0; i < row; i++){
        r[i] = set_int_vec(temp);
        for (int j = 0; j < col; j++) copy(r[i][j], x[j][i]);
    }

    return r;
}


zp_mat Field::mat_join(const zp_mat& x, const zp_mat& y){
    // Check that inputs have to have equal length.
    if (x.size() != y.size()){
        throw std::runtime_error("The input matrices are of different lengths.");
    }

    zp_mat r(x.size());

    for (int i = 0; i < x.size(); i++) r[i] = vec_join(x[i], y[i]);

    return r;
}

zp_mat Field::mat_mul(const zp_mat& x, const zp_mat& y) const{
    // Check that inputs have to have equal length.
    if (x[0].size() != y.size()){
        throw std::runtime_error("Dimensions of the input matrices do not match.");
    }

    zp_mat r(x.size());
    const int_vec temp(y[0].size());

    zp prod;
    init_zp(prod);

    for (int i = 0; i < x.size(); ++i){
        r[i] = set_int_vec(temp);
        for (int j = 0; j < y[0].size(); ++j){
            for (int k = 0; k < y.size(); ++k){
                mul(prod, x[i][k], y[k][j]);
                add(r[i][j], r[i][j], prod);
            }
        }
    }

    return r;
}

zp_mat Field::mat_mul(const zp_mat& x, const zp& y) const{
    zp_mat r(x.size());

    for (int i = 0; i < x.size(); i++) r[i] = vec_mul(x[i], y);

    return r;
}

zp_mat Field::mat_inv_with_det(const zp_mat& x, zp& det) const{
    // Check that input matrix is a square matrix.
    if (x.size() != x[0].size()){
        throw std::runtime_error("The input matrix is not a square matrix.");
    }

    const int size = static_cast<int>(x.size());

    // Declare the row echelon matrix and generate it.
    const zp_mat id = mat_id(size);
    zp_mat row_echelon = mat_join(x, id);

    // Declare temp value.
    zp temp_mul, temp_neg;
    init_zp(temp_mul);
    init_zp(temp_neg);

    // Set determinant to 1.
    one(det);

    // Bottom left half to all zeros.
    for (int i = 0; i < size; i++){
        for (int j = i; j < size; j++){
            if (i == j && !cmp(row_echelon[i][j], 1)){
                // Compute determinant.
                mul(det, det, row_echelon[i][j]);
                inv(temp_mul, row_echelon[i][i]);
                for (int k = i; k < size * 2; k++){
                    mul(row_echelon[j][k], row_echelon[j][k], temp_mul);
                }
            }

            if (i == j && cmp(row_echelon[i][j], 0)) break;

            if (i != j){
                copy(temp_mul, row_echelon[j][i]);
                for (int k = i; k < size * 2; k++){
                    mul(temp_neg, temp_mul, row_echelon[i][k]);
                    neg(temp_neg, temp_neg);
                    add(row_echelon[j][k], row_echelon[j][k], temp_neg);
                }
            }
        }
    }

    // Top right half to all zeros.
    for (int i = size - 1; i > 0; i--){
        for (int j = i - 1; j >= 0; j--){
            copy(temp_mul, row_echelon[j][i]);
            for (int k = i; k < size * 2; k++){
                mul(temp_neg, temp_mul, row_echelon[i][k]);
                neg(temp_neg, temp_neg);
                add(row_echelon[j][k], row_echelon[j][k], temp_neg);
            }
        }
    }

    // Copy over the output.
    zp_mat xi(size);
    const int_vec temp(size);
    for (int i = 0; i < size; i++){
        xi[i] = set_int_vec(temp);
        for (int j = 0; j < size; j++){
            copy(xi[i][j], row_echelon[i][size + j]);
        }
    }
    return xi;
}
