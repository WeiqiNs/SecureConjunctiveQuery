#pragma once

#include <limits>
#include <string>
#include <random>
#include <variant>
#include <fstream>
#include <sstream>
#include "RBP/bp.hpp"

// Define some useful type alias that we use in this library.
using str = std::string;
using StrVec = std::vector<str>;
using StrMat = std::vector<StrVec>;
using CharVec = std::vector<unsigned char>;
using CharMat = std::vector<CharVec>;
using Vec = std::variant<IntVec, StrVec>;
using Mat = std::variant<IntMat, StrMat>;
using VecOrMat = std::variant<IntVec, StrVec, IntMat, StrMat>;

class Helper{
public:
    /**
     * Read number of rows from a table file, the data are stored as strings.
     * @param filename the name of the file to read.
     * @param row number of rows to read.
     * @return the parsed data as a string matrix.
     */
    static StrMat read_tbl(const std::string& filename, int row);

    /// Return a random integer from 1 to the largest.
    static int rand_int();

    /**
     * Convert an integer to a vector of unsigned characters.
     * @param x an integer.
     * @return a vector of unsigned characters.
     */
    static CharVec int_to_char_vec(const int& x);

    /**
     * Convert a vector of unsigned characters to an integer.
     * @param x a vector of unsigned characters.
     * @return an integer.
     */
    static int char_vec_to_int(const CharVec& x);

    /**
     * Convert a string to a vector of unsigned characters.
     * @param x a string.
     * @return a vector of unsigned characters.
     */
    static CharVec str_to_char_vec(const str& x);

    /**
     * Convert a vector of unsigned characters to a string.
     * @param x a vector of unsigned characters.
     * @return a string.
     */
    static str char_vec_to_str(const CharVec& x);

    /**
     * Convert a vector of unsigned characters to a field point.
     * @param x a vector of unsigned characters.
     * @return a field point.
     */
    static Fp char_vec_to_fp(const CharVec& x);

    /**
     * Perform xor between a vector of character vectors.
     * @param x a matrix of characters.
     * @return a vector of characters.
     */
    static CharVec xor_char_vec(const CharMat& x);

    /**
     * Return a vector of random integers with in a bound.
     * @param length the length of the desired vector.
     * @param min_v the minimum of the random value.
     * @param max_v the maximum of the random value.
     * @return a vector of random integers.
     */
    static IntVec rand_int_vec(const int& length, const int& min_v, const int& max_v);

    /**
     * Return a matrix of random integers with in a bound.
     * @param row the number of rows in the matrix.
     * @param col the number of columns in the matrix.
     * @param min_v the minimum of the random value.
     * @param max_v the maximum of the random value.
     * @return a matrix of random integers.
     */
    static IntMat rand_int_mat(const int& row, const int& col, const int& min_v, const int& max_v);

    /**
     * Compute the polynomial of powers for the input vector x.
     * Given (x1, x2, ..., xn), output (x1^d, x1^(d-1), ..., xn^1, n).
     * Note that the output length is n * d + 1, where n is the input length.
     * @param d the desired degree of the polynomial.
     * @param pairing_group the bilinear pairing group object.
     * @param x a vector of field point values.
     * @return a vector of powers of the input.
     */
    static FpVec power_poly(int d, const BP& pairing_group, const FpVec& x);

    /**
     * Given a matrix of roots, compute the polynomial coefficients and merge them into one vector.
     * @param d the desired degree of the polynomial.
     * @param pairing_group the bilinear pairing group object.
     * @param x a matrix of field point values. Note that each row may contain different number of elements though.
     * @return a vector of computed coefficients of the input matrix.
     */
    static FpVec coeff_poly(int d, const BP& pairing_group, const FpMat& x);

    /**
     * Given a vector x, find two vectors x1 + x2 = x, where no point in x1 and x2 are zeros.
     * @param pairing_group the bilinear pairing group object.
     * @param x a vector of field point values.
     * @return a vector of double the length of input.
     */
    static FpVec split_poly(const BP& pairing_group, const FpVec& x);

    /**
     * Given an integer or a string vector, convert it to Zp representations.
     * @param pairing_group the bilinear pairing group object.
     * @param x a vector of integers or strings.
     * @param sel an integer vector to indicate the index of input vector.
     * @return a vector of field point values.
     */
    static FpVec vec_to_fp(const BP& pairing_group, const Vec& x, const IntVec& sel = {});

    /**
     * Given an integer or a string matrix, convert it to Zp representations.
     * @param pairing_group the bilinear pairing group object.
     * @param x a matrix of integers or strings.
     * @param sel an integer vector to indicate the index of input vector.
     * @return a matrix of field point values.
     */
    static FpMat mat_to_fp(const BP& pairing_group, const Mat& x, const IntVec& sel = {});

    /**
     * This is specific to our functionality of selecting only a few columns from the entire ciphertext.
     * @param degree how many values we select for each index.
     * @param length total number of possible index.
     * @param sel a vector of integers indicating where we want to select.
     * @return a vector of selected indices.
     */
    static IntVec get_sel_index(int degree, int length, const IntVec& sel);
};
