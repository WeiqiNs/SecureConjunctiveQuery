#pragma once

#include "field.hpp"

using g1 = g1_t;
using g2 = g2_t;
using gt = gt_t;
using g1_vec = std::vector<g1>;
using g2_vec = std::vector<g2>;

class Group{
public:
    Group() = default;

    explicit Group(const bool& pre);

    static bool cmp_gt(const gt& x, const gt& y);

    static void pair(gt& r, const g1& x, const g2& y);

    static void pair(gt& r, const g1_vec& x, const g2_vec& y);

    void init(const bool& pre);

    void g1_raise(g1& g, const zp& x) const;

    void g2_raise(g2& g, const zp& x) const;

    [[nodiscard]] g1_vec g1_raise(const zp_vec& x) const;

    [[nodiscard]] g2_vec g2_raise(const zp_vec& x) const;

private:
    bool pre_table{};
    g1 gen_1{};
    g2 gen_2{};
    g1* table_1{};
    g2* table_2{};
};
