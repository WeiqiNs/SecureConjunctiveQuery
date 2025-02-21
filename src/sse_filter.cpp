#include "sse_filter.hpp"

SseFilterMsk SseFilter::msk_gen(const CharVec& key){
    // Create the msk instance.
    SseFilterMsk msk;

    msk.prf = std::make_unique<PRF>(key);

    return msk;
}

CharMat SseFilter::enc(const SseFilterMsk& msk, const Vec& x){
    // Create the object to hold ct.
    CharMat ct;

    // Make convert each input to CharVec and digest.
    std::visit([&ct, &msk](auto&& input_x){
        using T = std::decay_t<decltype(input_x)>;
        if constexpr (std::is_same_v<T, IntVec>){
            for (const auto each_x : input_x)
                ct.push_back(msk.prf->digest(Helper::int_to_char_vec(each_x)));
        }
        else if (std::is_same_v<T, StrVec>){
            for (const auto each_x : input_x)
                ct.push_back(msk.prf->digest(Helper::str_to_char_vec(each_x)));
        }
        else throw std::invalid_argument("The input type is not supported.");
    }, x);

    return ct;
}

CharVec SseFilter::keygen(const SseFilterMsk& msk, const Vec& y){
    // Create the object to hold sk.
    CharVec sk;

    // Make convert each input to CharVec and digest.
    std::visit([&sk, &msk](auto&& input_y){
        using T = std::decay_t<decltype(input_y)>;
        if constexpr (std::is_same_v<T, IntVec>){
            sk = msk.prf->digest(Helper::int_to_char_vec(input_y[0]));
            for (int i = 1; i < input_y.size(); ++i){
                CharVec temp = msk.prf->digest(Helper::int_to_char_vec(input_y[i]));
                std::transform(
                    sk.begin(), sk.end(), temp.begin(), sk.begin(),
                    [](const unsigned char a, const unsigned char b){ return a ^ b; }
                );
            }
        }
        else if (std::is_same_v<T, StrVec>){
            sk = msk.prf->digest(Helper::str_to_char_vec(input_y[0]));
            for (int i = 1; i < input_y.size(); ++i){
                CharVec temp = msk.prf->digest(Helper::str_to_char_vec(input_y[i]));
                std::transform(
                    sk.begin(), sk.end(), temp.begin(), sk.begin(),
                    [](const unsigned char a, const unsigned char b){ return a ^ b; }
                );
            }
        }
        else throw std::invalid_argument("The input type is not supported.");
    }, y);

    return sk;
}

bool SseFilter::dec(const CharMat& ct, const CharVec& sk){
    return Helper::xor_char_vec(ct) == sk;
}
