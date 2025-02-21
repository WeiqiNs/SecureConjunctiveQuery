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

CharVec Helper::xor_char_vec(const CharMat& x){
    // Create a copy of x.
    CharVec r = x[0];

    // Perform XOR.
    for (size_t i = 1; i < x.size(); ++i){
        std::transform(
            r.begin(), r.end(), x[i].begin(), r.begin(),
            [](const unsigned char a, const unsigned char b){ return a ^ b; }
        );
    }

    return r;
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
    for (int i = 1; i <= d; ++i) power.emplace_back(i);

    // Compute x_i^1, x_i^2..., x_n^d.
    for (const auto& i : x)
        for (const auto& j : power) r.emplace_back(pairing_group.Zp->exp(i, j));

    // Finally attach a one to the end.
    r.emplace_back(1);

    // Return the output vector.
    return r;
}

FpVec Helper::coeff_poly(const int d, const BP& pairing_group, const FpMat& x){
    // Create holder for the returned vector.
    FpVec r;

    // Create holder for the aggregated constants.
    Fp constant;

    for (const auto& i : x){
        // Interpolate the polynomial on this row.
        FpVec temp = pairing_group.Zp->poly_interpolate(d, i);
        // Add the constant together.
        constant = pairing_group.Zp->add(constant, temp[0]);
        // Pop the constant; the first value.
        temp.erase(temp.begin());
        // Join the vector to the result.
        r = Field::vec_join(r, temp);
    }

    // Add the aggregated constant to the returned vector.
    r.push_back(constant);

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

IntVec Helper::get_sel_index(const int degree, const int length, const IntVec& sel){
    // Create holder for the returned vector.
    IntVec r;

    // Add the selected index.
    for (const auto i : sel) for (int j = 0; j < degree; ++j) r.emplace_back(i * degree + j);

    // Add the last index.
    r.push_back(degree * length);
    return r;
}
