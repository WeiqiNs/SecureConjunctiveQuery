#include "group.hpp"

Group::Group(const bool& pre){
    // The constructor only needs to sample group generator.
    g1_get_gen(gen_1);
    g2_get_gen(gen_2);

    // If we use the precomputed table.
    if (pre){
        // Table for group 1.
        table_1 = static_cast<g1*>(malloc(sizeof(g1) * RLC_G1_TABLE));
        g1_mul_pre(table_1, gen_1);

        // Table for group 2.
        table_2 = static_cast<g2*>(malloc(sizeof(g2) * RLC_G2_TABLE));
        g2_mul_pre(table_2, gen_2);

        pre_table = true;
    }
    else{
        pre_table = false;
    }
}

bool Group::cmp_gt(const gt& x, const gt& y){
    return gt_cmp(x, y) == RLC_EQ;
}

void Group::pair(gt& r, const g1& x, const g2& y){
    pc_map(r, x, y);
}

void Group::pair(gt& r, const g1_vec& x, const g2_vec& y){
    // Check that inputs have to have equal length.
    if (x.size() != y.size()){
        throw std::runtime_error("The input vectors are of different lengths.");
    }

    pc_map_sim(r, x.data(), y.data(), x.size());
}

void Group::init(const bool& pre){
    // The constructor only needs to sample group generator.
    g1_get_gen(gen_1);
    g2_get_gen(gen_2);

    // If we use the precomputed table.
    if (pre){
        // Table for group 1.
        table_1 = static_cast<g1*>(malloc(sizeof(g1) * RLC_G1_TABLE));
        g1_mul_pre(table_1, gen_1);

        // Table for group 2.
        table_2 = static_cast<g2*>(malloc(sizeof(g2) * RLC_G2_TABLE));
        g2_mul_pre(table_2, gen_2);

        pre_table = true;
    }
    else{
        pre_table = false;
    }
}

void Group::g1_raise(g1& g, const zp& x) const{
    if (pre_table){
        g1_mul_fix(g, table_1, x);
    }
    else{
        g1_mul(g, gen_1, x);
    }
}

void Group::g2_raise(g2& g, const zp& x) const{
    if (pre_table){
        g2_mul_fix(g, table_2, x);
    }
    else{
        g2_mul(g, gen_2, x);
    }
}

g1_vec Group::g1_raise(const zp_vec& x) const{
    g1_vec r(x.size());

    for (auto i = 0; i < x.size(); i++) g1_mul(r[i], gen_1, x[i]);

    return r;
}

g2_vec Group::g2_raise(const zp_vec& x) const{
    g2_vec r(x.size());

    for (auto i = 0; i < x.size(); i++) g2_mul(r[i], gen_2, x[i]);

    return r;
}
