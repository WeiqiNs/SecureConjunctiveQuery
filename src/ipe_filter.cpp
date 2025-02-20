#include "ipe_filter.hpp"

IpeFilterPP IpeFilter::pp_gen(const int degree, const int length, const bool pre){
    // Create the pp instance.
    IpeFilterPP pp;
    // Update the degree according to input.
    pp.d = degree;
    // Update the input length according to input.
    pp.l = length;
    // Save the created pairing group.
    pp.pairing_group = std::make_unique<BP>(pre);

    return pp;
}

IpeFilterMsk IpeFilter::msk_gen(const IpeFilterPP& pp){
    // Create the msk instance.
    IpeFilterMsk msk;

    // Compute the matrix size.
    const int mat_size = pp.l * pp.d + 3;
    // Generate the random matrix of desired size.
    msk.b = pp.pairing_group->Zp->rand_mat(mat_size, mat_size);
    // Find the inverse and the determinant.
    Fp det;
    msk.bi = pp.pairing_group->Zp->mat_inv_with_det(msk.b, det);
    // Find its transpose multiplied with determinant.
    msk.bi = Field::mat_transpose(pp.pairing_group->Zp->mat_mul(msk.bi, det));

    return msk;
}

G1Vec IpeFilter::enc(const IpeFilterPP& pp, const IpeFilterMsk& msk, const Vec& x){
    // Create the x vector in Fp.
    FpVec x_vec;

    // Convert the input x vector to Fp.
    std::visit([&pp, &x_vec](auto&& input_x){
        using T = std::decay_t<decltype(input_x)>;
        if constexpr (std::is_same_v<T, IntVec> || std::is_same_v<T, StrVec>){
            for (auto& i : input_x){
                Fp temp(i);
                pp.pairing_group->Zp->mod(temp);
                x_vec.push_back(temp);
            }
        }
        else throw std::invalid_argument("The input type is not supported.");
    }, x);

    // Sample the random point alpha.
    const auto r = pp.pairing_group->Zp->rand();

    // We compute polynomial of x.
    const auto poly_x = Helper::power_poly(pp.d, *pp.pairing_group, x_vec);
    // We compute a * x.
    auto ax = pp.pairing_group->Zp->vec_mul(poly_x, r);
    // Attach (r, 0).
    ax.push_back(pp.pairing_group->Zp->rand());
    ax.emplace_back(0);

    // Multiply with the matrix Bi.
    const auto axb = pp.pairing_group->Zp->mat_mul(ax, msk.b);

    // Raise to g1 and return.
    return pp.pairing_group->Gp->g1_raise(axb);
}

G2Vec IpeFilter::keygen(const IpeFilterPP& pp, const IpeFilterMsk& msk, const Mat& y){
    // Create the y matrix in Fp.
    FpMat y_mat;

    // Convert the input y matrix to Fp.
    std::visit([&pp, &y_mat](auto&& input_y){
        using T = std::decay_t<decltype(input_y)>;
        if constexpr (std::is_same_v<T, IntMat> || std::is_same_v<T, StrMat>){
            for (auto& row : input_y){
                FpVec temp_vec;
                for (auto& i : row){
                    Fp temp(i);
                    pp.pairing_group->Zp->mod(temp);
                    temp_vec.push_back(temp);
                }
                y_mat.push_back(temp_vec);
            }
        }
        else throw std::invalid_argument("The input type is not supported.");
    }, y);

    // We compute the polynomial of y.
    auto poly_y = Helper::coeff_poly(pp.d, *pp.pairing_group, y_mat);
    // Attach (0, r).
    poly_y.emplace_back(0);
    poly_y.push_back(pp.pairing_group->Zp->rand());
    // Multiply with the matrix B.
    const auto yb = pp.pairing_group->Zp->mat_mul(poly_y, msk.bi);

    // Raise to g2 and return.
    return pp.pairing_group->Gp->g2_raise(yb);
}

bool IpeFilter::dec(const G1Vec& ct, const G2Vec& sk){ return Group::check_gt_unity(Group::pair(ct, sk)); }
