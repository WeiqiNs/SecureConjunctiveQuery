#include "helper.hpp"

CharVec Helper::int_to_char_vec(const int& x){
    // Declare the return object, a vector of unsigned characters.
    CharVec r;

    // Extract bytes in big-endian order.
    for (size_t i = sizeof(int); i > 0; --i) r.emplace_back(static_cast<unsigned char>(x >> (i - 1) * 8) & 0xFF);

    return r;
}

int Helper::char_vec_to_int(const CharVec& x){
    // Declare the return object, an integer.
    int r = 0;

    // Ensure the vector size does not exceed the size of int (typically 4 bytes)
    const size_t num_bytes = std::min(x.size(), sizeof(int));

    // Combine bytes into an integer (big-endian order).
    for (size_t i = 0; i < num_bytes; ++i) r |= x[i] << (num_bytes - 1 - i) * 8;

    return r;
}

CharVec Helper::str_to_char_vec(const str& x){
    return {x.begin(), x.end()};
}

str Helper::char_vec_to_str(const CharVec& x){
    return {x.begin(), x.end()};
}

Fp Helper::char_vec_to_fp(const CharVec& x){
    return Fp(x);
}

IntVec Helper::rand_int_vec(const int& length, const int& min_v, const int& max_v){
    IntVec r(length);

    // Initialize random number generator with a seed based on current time
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution dis(min_v, max_v);

    for (int i = 0; i < length; ++i) r[i] = dis(gen);

    return r;
}

IntMat Helper::rand_int_mat(const int& row, const int& col, const int& min_v, const int& max_v){
    IntMat r(row, IntVec(col));

    // Initialize random number generator with a seed based on current time
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution dis(min_v, max_v);

    // Fill the matrix with random integers
    for (int i = 0; i < row; ++i) for (int j = 0; j < col; ++j) r[i][j] = dis(gen);

    return r;
}

FpVec Helper::power_poly(const int d, const BP& pairing_group, const FpVec& x){
    FpVec r, power;

    // Fill the power Fp vector from 1 to degree.
    for (int i = 0; i <= d; ++i) power.emplace_back(i);

    // Compute x_i^0, x_i^1, x_i^2..., x_n^d.
    for (const auto& i : x)
        for (const auto& j : power)
            r.emplace_back(pairing_group.Zp->exp(i, j));

    // Return the output vector.
    return r;
}

FpVec Helper::coeff_poly(const int d, const BP& pairing_group, const FpMat& x){
    // Create holder for the returned vector.
    FpVec r;

    for (const auto& i : x)
        // Interpolate the polynomial on this row.
        r = Field::vec_join(r, pairing_group.Zp->poly_interpolate(d, i));

    return r;
}

FpVec Helper::split_poly(const BP& pairing_group, const FpVec& x){
    // Create holder for the returned vector.
    FpVec r;

    // Sample the x1.
    for (const auto& i : x){
        // Sample a random point.
        Fp temp = pairing_group.Zp->rand();
        // If the random point is the same as i, sample another one.
        while (Field::cmp(i, temp)) temp = pairing_group.Zp->rand();
        // Attach this value to r.
        r.push_back(temp);
    }

    // Find x2 such that x1 + x2 = x.
    for (auto i = 0; i < x.size(); ++i) r.push_back(pairing_group.Zp->sub(x[i], r[i]));

    return r;
}

IntVec Helper::get_sel_index(const int degree, const IntVec& sel){
    // Create holder for the returned vector.
    IntVec r;

    // Add the selected index.
    for (const auto i: sel) for (int j = 0; j <= degree; ++j) r.emplace_back(i * (degree + 1) + j);

    return r;
}
