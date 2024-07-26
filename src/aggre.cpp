#include "aggre.hpp"

pp Aggre::pp_gen(const bool& pre){ return Filter::pp_gen(pre); }

aggre_msk Aggre::msk_gen(const pp& pp, const int& input_len){
    aggre_msk msk;
    Field::init_zp(msk.d);
    Field::init_zp(msk.dp);

    pp.field_zp.rand(msk.d);
    pp.field_zp.inv(msk.dp, msk.d);
    msk.r = pp.field_zp.vec_rand(input_len);
    msk.b = pp.field_zp.vec_rand(input_len);
    msk.bp = pp.field_zp.vec_inv(msk.b);

    return msk;
}

g1_vec Aggre::enc(const pp& pp, const aggre_msk& msk, const int_vec& x){
    // Convert the input to zp.
    const zp_vec zp_x = pp.field_zp.from_int_vec(x);

    // Sample the random point alpha.
    zp alpha;
    Field::init_zp(alpha);
    pp.field_zp.rand(alpha);

    // Create the last point l.
    zp l;
    Field::init_zp(l);

    // Compute x + r.
    const zp_vec xr = pp.field_zp.vec_add(zp_x, msk.r);
    // Compute b * (x + r).
    const zp_vec bxr = pp.field_zp.vec_mul(xr, msk.b);
    // Compute alpha * b * (x + r).
    const zp_vec abxr = pp.field_zp.vec_mul(bxr, alpha);

    // Compute the last position alpha * delta.
    pp.field_zp.mul(l, alpha, msk.d);
    // Join the last position.
    const zp_vec raise = Field::vec_join(abxr, l);

    // Raise to g1 and return.
    return pp.group_bp.g1_raise(raise);
}

g2_vec Aggre::keygen(const pp& pp, const aggre_msk& msk, const int_vec& y, const int& p){
    // Convert the input to zp.
    const zp_vec zp_y = pp.field_zp.from_int_vec(y);
    // Convert the input point to zp as well.
    zp zp_p;
    Field::init_zp(zp_p);
    pp.field_zp.from_int(zp_p, p);

    // Sample the random point beta.
    zp beta;
    Field::init_zp(beta);
    pp.field_zp.rand(beta);

    // Create the last point l.
    zp l;
    Field::init_zp(l);

    // Compute b' * y.
    const zp_vec bpy = pp.field_zp.vec_mul(zp_y, msk.bp);
    // Compute beta * b' * y.
    const zp_vec bbpy = pp.field_zp.vec_mul(bpy, beta);

    // Compute the last position beta * delta' * (<y, r> + p).
    pp.field_zp.vec_ip(l, zp_y, msk.r);
    pp.field_zp.add(l, l, zp_p);
    pp.field_zp.mul(l, l, msk.dp);
    pp.field_zp.mul(l, l, beta);
    // Join the last position.
    const zp_vec raise = Field::vec_join(bbpy, l);

    // Raise to g2 and return.
    return pp.group_bp.g2_raise(raise);
}

bool Aggre::dec(g1_vec& ct, g2_vec& sk){
    return Filter::dec(ct, sk);
}
