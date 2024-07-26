#pragma once

#include <vector>
#include <stdexcept>
#include "init.hpp"

using int_vec = std::vector<int>;
using int_mat = std::vector<int_vec>;
using zp_vec = std::vector<zp>;
using zp_mat = std::vector<zp_vec>;

/**
 *
 */
class Field{
public:
    /**
     *
     */
    Field() = default;

    /**
     *
     * @param order
     */
    explicit Field(const zp& order);

    /**
     *
     * @param order
     */
    void init(const zp& order);

    /**
     *
     * @param x
     */
    static void init_zp(zp& x);

    /**
     *
     * @param x
     */
    static void one(zp& x);

    /**
     *
     * @param x
     */
    static void zero(zp& x);

    /**
     *
     * @param r
     * @param x
     */
    static void copy(zp& r, const zp& x);

    /**
     *
     * @param x
     * @param y
     * @return
     */
    static bool cmp(const zp& x, const zp& y);

    /**
     *
     * @param x
     * @param y
     * @return
     */
    static bool cmp(const zp& x, const int& y);

    /**
    *
    * @param r
    * @param x
    */
    static void set_int(zp& r, const int& x);

    /**
        *
        * @param r
        * @param x
        */
    void from_int(zp& r, const int& x) const;

    /**
     *
     * @param x
     */
    void mod(zp& x) const;

    /**
     *
     * @param x
     */
    void rand(zp& x) const;

    /**
     *
     * @param r
     * @param x
     * @param y
     */
    void add(zp& r, const zp& x, const zp& y) const;

    /**
     *
     * @param r
     * @param x
     * @param y
     */
    void mul(zp& r, const zp& x, const zp& y) const;

    /**
     *
     * @param r
     * @param x
     */
    void neg(zp& r, const zp& x) const;

    /**
     *
     * @param r
     * @param x
     */
    void inv(zp& r, const zp& x) const;

    /**
        *
        * @param x
        * @return
        */
    static zp_vec set_int_vec(const int_vec& x);

    /**
        *
        * @param x
        * @return
        */
    [[nodiscard]] zp_vec from_int_vec(const int_vec& x) const;

    /**
    *
    * @param size
    * @return
    */
    [[nodiscard]] zp_vec vec_rand(const int& size) const;

    /**
     *
     * @param x
     * @param y
     * @return
     */
    [[nodiscard]] zp_vec vec_add(const zp_vec& x, const zp_vec& y) const;

    /**
        *
        * @param x
        * @param y
        * @return
        */
    [[nodiscard]] zp_vec vec_mul(const zp_vec& x, const zp_vec& y) const;

    /**
     *
     * @param x
     * @param y
     * @return
     */
    [[nodiscard]] zp_vec vec_mul(const zp_vec& x, const zp& y) const;

    /**
        *
        * @param x
        * @return
        */
    [[nodiscard]] zp_vec vec_inv(const zp_vec& x) const;

    /**
     *
     * @param ip
     * @param x
     * @param y
     */
    void vec_ip(zp& ip, const zp_vec& x, const zp_vec& y) const;

    /**
     *
     * @param x
     * @param y
     * @return
     */
    static zp_vec vec_join(const zp_vec& x, const zp& y);

    /**
     *
     * @param x
     * @param y
     * @return
     */
    static zp_vec vec_join(const zp_vec& x, const zp_vec& y);

    /**
     * 
     * @param x 
     * @return 
     */
    static zp_mat set_int_mat(const int_mat& x);

    /**
     * 
     * @param x 
     * @return 
     */
    zp_mat from_int_mat(const int_mat& x);

    /**
     * 
     * @param row 
     * @param col 
     * @return 
     */
    zp_mat mat_rand(const int& row, const int& col);

    /**
     * 
     * @param size 
     * @return 
     */
    zp_mat mat_id(const int& size);

    /**
     * 
     * @param x 
     * @return 
     */
    bool mat_is_id(zp_mat& x);

    /**
     * 
     * @param x 
     * @return 
     */
    zp_mat mat_transpose(zp_mat& x);

    /**
     * 
     * @param x 
     * @param y 
     * @return 
     */
    zp_mat mat_join(zp_mat& x, zp_mat& y);

    /**
     * 
     * @param x 
     * @param y 
     * @return 
     */
    zp_mat mat_mul(zp_mat& x, zp_mat& y);

    /**
     * 
     * @param x 
     * @param y 
     * @return 
     */
    zp_mat mat_mul(zp_mat& x, zp& y);

    /**
     * 
     * @param x 
     * @param det 
     * @return 
     */
    zp_mat mat_inv_with_det(zp_mat& x, zp& det);

private:
    zp P{};
};
