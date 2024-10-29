#include "helper.hpp"

CharVec Helper::int_to_char_vec(const int& x){
    // Declare the return value.
    CharVec r;

    // Extract bytes in little-endian order.
    for (size_t i = 0; i < sizeof(int); ++i) r.push_back(static_cast<unsigned char>((x >> (i * 8)) & 0xFF));

    return r;
}

int Helper::char_vec_to_int(const CharVec& x){
    // Declare the return value.
    int r = 0;

    // Ensure the vector size does not exceed the size of int.
    const size_t num_bytes = std::min(x.size(), sizeof(int));

    // Shift and add each byte to result in little-endian order.
    for (size_t i = 0; i < num_bytes; ++i) r |= (x[i] << (i * 8));

    return r;
}

CharVec Helper::str_to_char_vec(const str& x){
    return {x.begin(), x.end()};
}

str Helper::char_vec_to_str(const CharVec& x){
    return {x.begin(), x.end()};
}

Fp Helper::char_vec_to_fp(const CharVec& x){
    Fp r;

    bn_set_dig(r.value, char_vec_to_int(x));

    return r;
}

IntVec Helper::rand_int_vec(const int& length, const int& min_v, const int& max_v){
    IntVec r(length);

    // Initialize random number generator with a seed based on current time
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution dis(min_v, max_v);

    for (int i = 0; i < length; ++i){
        r[i] = dis(gen);
    }

    return r;
}

IntMat Helper::rand_int_mat(const int& row, const int& col, const int& min_v, const int& max_v){
    IntMat r(row, IntVec(col));

    // Initialize random number generator with a seed based on current time
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution dis(min_v, max_v);

    // Fill the matrix with random integers
    for (int i = 0; i < row; ++i){
        for (int j = 0; j < col; ++j){
            r[i][j] = dis(gen);
        }
    }

    return r;
}
