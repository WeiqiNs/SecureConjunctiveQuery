#include "ipe_join.hpp"

IpeJoinPP IpeJoin::pp_gen(const int degree, const int length, const bool pre){
    // Create the pp instance.
    IpeJoinPP pp;
    // Update the degree according to input.
    pp.d = degree;
    // Update the input length according to input.
    pp.l = length;
    // Save the created pairing group.
    pp.pairing_group = std::make_unique<BP>(pre);

    return pp;
}

IpeJoinMsk IpeJoin::msk_gen(const IpeJoinPP& pp, const CharVec& key){
    // Create the msk instance.
    IpeJoinMsk msk;
    // Get the unique point for HMAC.
    msk.hmac = std::make_unique<HMAC>(key);
    // Sample a random point k.
    msk.k = pp.pairing_group->Zp->rand();

    // Compute the matrix size.
    const int mat_size = (pp.l - 1) * pp.d + 4;
    // Generate the random matrix of desired size.
    msk.b = pp.pairing_group->Zp->rand_mat(mat_size, mat_size);
    // Find the inverse and the determinant.
    Fp det;
    msk.bi = pp.pairing_group->Zp->mat_inv_with_det(msk.b, det);
    // Find its transpose multiplied with determinant.
    msk.bi = Field::mat_transpose(msk.bi);

    return msk;
}

G1Vec IpeJoin::enc(const IpeJoinPP& pp, const IpeJoinMsk& msk, const Vec& x, const int join_index){
    // Declare variable to hold the point to join on.
    Fp join_on;

    // Make a copy of the input x.
    Vec x_copy = x;

    // Compute the hash of value to join and remove the join value from x copy.
    std::visit([&pp, &msk, &join_on, join_index](auto&& input_x){
        using T = std::decay_t<decltype(input_x)>;
        if constexpr (std::is_same_v<T, IntVec>){
            join_on = Helper::char_vec_to_fp(msk.hmac->digest(Helper::int_to_char_vec(input_x[join_index])));
            pp.pairing_group->Zp->mod(join_on);
            input_x.erase(input_x.begin() + join_index);
        }
        else if constexpr (std::is_same_v<T, StrVec>){
            join_on = Helper::char_vec_to_fp(msk.hmac->digest(Helper::str_to_char_vec(input_x[join_index])));
            pp.pairing_group->Zp->mod(join_on);
            input_x.erase(input_x.begin() + join_index);
        }
        else throw std::invalid_argument("The input type is not supported.");
    }, x_copy);

    // Generate hash of the input x vector.
    const auto x_digest = msk.hmac->digest_vec_to_fp_mod(*pp.pairing_group, x_copy);

    // We compute polynomial of x.
    auto poly_x = Helper::power_poly(pp.d, *pp.pairing_group, x_digest);

    // Attach (r, 0).
    poly_x.push_back(pp.pairing_group->Zp->rand());
    poly_x.emplace_back(0);

    // Insert the join on.
    poly_x.push_back(join_on);

    // Multiply with the matrix B.
    const auto xb = pp.pairing_group->Zp->mat_mul(poly_x, msk.b);

    // Raise to g1 and return.
    return pp.pairing_group->Gp->g1_raise(xb);
}

G2Vec IpeJoin::keygen(const IpeJoinPP& pp, const IpeJoinMsk& msk, const Mat& y){
    // Compute the hash of filtered values.
    const auto y_digest = msk.hmac->digest_mat_to_fp_mod(*pp.pairing_group, y);

    // We compute the polynomial of y.
    auto poly_y = Helper::coeff_poly(pp.d, *pp.pairing_group, y_digest);

    // Attach (0, r).
    poly_y.emplace_back(0);
    poly_y.push_back(pp.pairing_group->Zp->rand());

    // Push the randomness in the key to the back.
    poly_y.push_back(msk.k);

    // Multiply with the matrix B.
    const auto ybi = pp.pairing_group->Zp->mat_mul(poly_y, msk.bi);

    // Raise to g2 and return.
    return pp.pairing_group->Gp->g2_raise(ybi);
}

Gt IpeJoin::dec(const G1Vec& ct, const G2Vec& sk){ return Group::pair(ct, sk); }
