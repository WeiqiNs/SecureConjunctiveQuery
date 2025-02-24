#include "sse_filter.hpp"

SseFilterMsk SseFilter::msk_gen(const CharVec& key){
    // Create the msk instance.
    SseFilterMsk msk;

    msk.counter = 0;
    msk.prf = std::make_unique<PRF>(key);

    return msk;
}

CharMat SseFilter::enc(SseFilterMsk& msk, const Vec& x){
    // Create the object to hold ct.
    CharMat ct;

    // Make convert each input to CharVec and digest.
    std::visit([&ct, &msk](auto&& input_x){
        using T = std::decay_t<decltype(input_x)>;
        if constexpr (std::is_same_v<T, IntVec>){
            for (const auto& each_x : input_x)
                ct.push_back(msk.prf->digest(
                    Helper::str_to_char_vec(std::to_string(each_x) + std::to_string(msk.counter))
                ));
        }
        else if (std::is_same_v<T, StrVec>){
            for (const auto& each_x : input_x)
                ct.push_back(msk.prf->digest(
                    Helper::str_to_char_vec(each_x + std::to_string(msk.counter))
                ));
        }
        else throw std::invalid_argument("The input type is not supported.");
    }, x);

    // Increment the counter.
    ++msk.counter;

    return ct;
}

CharMat SseFilter::keygen(const SseFilterMsk& msk, const Vec& y, const int row){
    // Create the object to hold sk.
    CharMat sk;

    // Make convert each input to CharVec and digest.
    std::visit([&sk, &msk, row](auto&& input_y){
        using T = std::decay_t<decltype(input_y)>;
        if constexpr (std::is_same_v<T, IntVec>){
            for (int i = 0; i < row; ++i){
                sk.push_back(msk.prf->digest(
                    Helper::str_to_char_vec(std::to_string(input_y[0]) + std::to_string(i))
                ));

                for (int j = 1; j < input_y.size(); ++j){
                    CharVec temp = msk.prf->digest(
                        Helper::str_to_char_vec(std::to_string(input_y[j]) + std::to_string(i))
                    );
                    std::transform(
                        sk.back().begin(), sk.back().end(), temp.begin(), sk.back().begin(),
                        [](const unsigned char a, const unsigned char b){ return a ^ b; }
                    );
                }
            }
        }
        else if (std::is_same_v<T, StrVec>){
            for (int i = 0; i < row; ++i){
                sk.push_back(msk.prf->digest(
                    Helper::str_to_char_vec(input_y[0] + std::to_string(i))
                ));

                for (int j = 1; j < input_y.size(); ++j){
                    CharVec temp = msk.prf->digest(
                        Helper::str_to_char_vec(input_y[j] + std::to_string(i))
                    );
                    std::transform(
                        sk.back().begin(), sk.back().end(), temp.begin(), sk.back().begin(),
                        [](const unsigned char a, const unsigned char b){ return a ^ b; }
                    );
                }
            }
        }
        else throw std::invalid_argument("The input type is not supported.");
    }, y);

    return sk;
}

bool SseFilter::dec(const CharMat& ct, const CharVec& sk){
    return Helper::xor_char_vec(ct) == sk;
}
