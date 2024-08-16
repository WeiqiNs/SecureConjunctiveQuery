#pragma once

#include <algorithm>
#include <string>
#include <random>
#include <vector>
#include "field.hpp"

using str = std::string;
using str_vec = std::vector<str>;
using str_mat = std::vector<str_vec>;
using char_vec = std::vector<unsigned char>;

class Helper{
public:
    /**
     *
     * @param x
     * @return
     */
    static char_vec int_to_vec(const int& x);

    /**
     *
     * @param x
     * @return
     */
    static int vec_to_int(const char_vec& x);

    /**
     *
     * @param x
     * @return
     */
    static char_vec str_to_vec(const str& x);

    /**
     *
     * @param x
     * @return
     */
    static str vec_to_str(const char_vec& x);

    /**
     * 
     * @param r 
     * @param x 
     */
    static void vec_to_zp(zp& r, const char_vec& x);

    /**
     * 
     * @param length
     * @param min_v
     * @param max_v
     * @return 
     */
    static int_vec rand_vec(const int& length, const int& min_v, const int& max_v);

    /**
     *
     * @param row
     * @param col
     * @param min_v
     * @param max_v
     * @return
     */
    static int_mat rand_mat(const int& row, const int& col, const int& min_v, const int& max_v);
};
