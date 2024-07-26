#include "filter.hpp"

pp Filter::pp_gen(const bool& pre){
    zp p;
    init_get_order(p);

    pp pp;
    pp.field_zp.init(p);
    pp.group_bp.init(pre);

    return pp;
}

filter_msk Filter::msk_gen(const pp& pp, const int& input_len){
    filter_msk msk;
    msk.r = pp.field_zp.vec_rand(input_len);
    return msk;
}

g1_vec Filter::enc(const pp& pp, const filter_msk& msk, const int_vec& x){
    return enc(pp, msk.r, msk.hash.digest_int_vec(x));
}

g1_vec Filter::enc(const pp& pp, const filter_msk& msk, const str_vec& x){
    return enc(pp, msk.r, msk.hash.digest_str_vec(x));
}


g2_vec Filter::keygen(const pp& pp, const filter_msk& msk, const int_vec& y){
    return keygen(pp, msk.r, msk.hash.digest_int_vec(y));
}

g2_vec Filter::keygen(const pp& pp, const filter_msk& msk, const str_vec& y){
    return keygen(pp, msk.r, msk.hash.digest_str_vec(y));
}

bool Filter::dec(g1_vec& ct, g2_vec& sk){
    // Decalre two variables in Gt.
    gt x, y;

    // First compute a x byyr.
    Group::pair(x, ct.back(), sk.back());

    // Remove them from the vector.
    ct.pop_back();
    sk.pop_back();

    // Compute ip pairing.
    Group::pair(y, ct, sk);

    // Check for equality.
    return Group::cmp_gt(x, y);
}

g1_vec Filter::enc(const pp& pp, const zp_vec& r, const zp_vec& x){
    // Sample the random point alpha.
    zp alpha;
    Field::init_zp(alpha);
    pp.field_zp.rand(alpha);

    // Compute x + r.
    const zp_vec xr = pp.field_zp.vec_add(x, r);
    // Compute alpha * (x + r).
    const zp_vec axr = pp.field_zp.vec_mul(xr, alpha);
    // Join the last position.
    const zp_vec raise = Field::vec_join(axr, alpha);

    // Raise to g1 and return.
    return pp.group_bp.g1_raise(raise);
}

g2_vec Filter::keygen(const pp& pp, const zp_vec& r, const zp_vec& y){
    // Sample the random point beta.
    zp beta;
    Field::init_zp(beta);
    pp.field_zp.rand(beta);

    // Create the last point l.
    zp l;
    Field::init_zp(l);

    // Compute beta * y.
    const zp_vec by = pp.field_zp.vec_mul(y, beta);

    // Compute the last position beta * <y, y + r>.
    pp.field_zp.vec_ip(l, y, pp.field_zp.vec_add(y, r));
    pp.field_zp.mul(l, beta, l);
    // Join the last position.
    const zp_vec raise = Field::vec_join(by, l);

    // Raise to g2 and return.
    return pp.group_bp.g2_raise(raise);
}
