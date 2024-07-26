#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include "field.hpp"

using str = std::string;
using str_vec = std::vector<str>;
using vec = std::vector<unsigned char>;

class Helper{
public:
    /**
     *
     * @param x
     * @return
     */
    static vec int_to_vec(const int& x);

    /**
     *
     * @param x
     * @return
     */
    static int vec_to_int(const vec& x);

    /**
     *
     * @param x
     * @return
     */
    static vec str_to_vec(const str& x);

    /**
     *
     * @param x
     * @return
     */
    static str vec_to_str(const vec& x);

    /**
     * 
     * @param r 
     * @param x 
     */
    static void vec_to_zp(zp& r, const vec& x);
};
