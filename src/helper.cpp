#include "helper.hpp"

char_vec Helper::int_to_vec(const int& x){
    int temp = x;
    char_vec r(sizeof(int));

    // Use reinterpret_cast to treat the integer as a sequence of bytes
    std::copy_n(reinterpret_cast<unsigned char*>(&temp), sizeof(int), r.begin());

    return r;
}

int Helper::vec_to_int(const char_vec& x){
    int r = 0;

    // Check if the size of the vector is correct
    if (x.size() == sizeof(int)){
        // Use reinterpret_cast to treat the vector's data as an integer and copy_n
        std::copy_n(x.begin(), sizeof(int), reinterpret_cast<unsigned char*>(&r));
    }
    else{
        throw std::runtime_error("The size of the vector does not match the size of an integer.");
    }

    return r;
}

char_vec Helper::str_to_vec(const str& x){
    return {x.begin(), x.end()};
}

str Helper::vec_to_str(const char_vec& x){
    return {x.begin(), x.end()};
}

void Helper::vec_to_zp(zp& r, const char_vec& x){
    // Make sure we start with zero.
    Field::zero(r);

    for (const unsigned char i : x){
        bn_mul_dig(r, r, 256);
        bn_add_dig(r, r, i);
    }
}

int_vec Helper::rand_vec(const int& length, const int& min_v, const int& max_v){
    std::vector<int> r;
    r.reserve(length);

    // Initialize random number generator with a seed based on current time
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution dis(min_v, max_v);

    for (int i = 0; i < length; ++i){
        r.push_back(dis(gen));
    }

    return r;
}

int_mat Helper::rand_mat(const int& row, const int& col, const int& min_v, const int& max_v){
    int_mat r(row, int_vec(col));

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
