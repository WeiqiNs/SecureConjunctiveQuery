#include "crypto.hpp"

AES::AES(const int& key_length){
    // Compute the byte length.
    const int byte_length = key_length / 8;
    // Get the cipher object.
    cipher_ = get_cipher(byte_length);
    // Sample a random key.
    key_.resize(byte_length);
    RAND_bytes(key_.data(), byte_length);
    // Get the cipher ctx.
    ctx_ = EVP_CIPHER_CTX_new();
}

AES::AES(const CharVec& key){
    // Get the cipher object.
    cipher_ = get_cipher(key.size());
    // Set the key.
    key_ = key;
    // Get the cipher ctx.
    ctx_ = EVP_CIPHER_CTX_new();
}

AES::~AES(){
    EVP_CIPHER_CTX_free(ctx_);
}

CharVec AES::get_key(){ return key_; }

void AES::set_key(const CharVec& key){ key_ = key; }

CharVec AES::encrypt(const CharVec& plaintext) const{
    // Sample the random IV.
    CharVec iv(EVP_MAX_IV_LENGTH);
    RAND_bytes(iv.data(), EVP_MAX_IV_LENGTH);

    // Declare length variable and get space for ciphertext.
    int len;
    CharVec ciphertext(plaintext.size() + EVP_CIPHER_block_size(cipher_));

    // Init the encryption scheme and update with ciphertext.
    EVP_EncryptInit(ctx_, cipher_, key_.data(), iv.data());
    EVP_EncryptUpdate(ctx_, ciphertext.data(), &len, plaintext.data(), plaintext.size());

    // Record the ciphertext length and finalize.
    int ciphertext_len = len;
    EVP_EncryptFinal_ex(ctx_, ciphertext.data() + len, &len);

    // Insert the IV to the beginning.
    ciphertext_len += len;
    ciphertext.resize(ciphertext_len);
    ciphertext.insert(ciphertext.begin(), iv.begin(), iv.end());

    return ciphertext;
}

CharVec AES::decrypt(const CharVec& ciphertext) const{
    // Split IV and the actual ciphertext.
    const CharVec iv(ciphertext.begin(), ciphertext.begin() + EVP_MAX_IV_LENGTH);
    const CharVec actual_ciphertext(ciphertext.begin() + EVP_MAX_IV_LENGTH, ciphertext.end());

    // Declare length variable and get space for plaintext.
    int len;
    CharVec plaintext(actual_ciphertext.size() + EVP_CIPHER_block_size(cipher_));

    // Init the decryption scheme and update with ciphertext.
    EVP_DecryptInit(ctx_, cipher_, key_.data(), iv.data());
    EVP_DecryptUpdate(ctx_, plaintext.data(), &len, actual_ciphertext.data(), actual_ciphertext.size());

    // Record the plaintext length and finalize.
    int plaintext_len = len;
    EVP_DecryptFinal_ex(ctx_, plaintext.data() + len, &len);

    // Resize the plaintext and output.
    plaintext_len += len;
    plaintext.resize(plaintext_len);

    return plaintext;
}

const EVP_CIPHER* AES::get_cipher(const int byte_length){
    switch (byte_length){
    case 16:
        return EVP_aes_128_cbc();
    case 24:
        return EVP_aes_192_cbc();
    case 32:
        return EVP_aes_256_cbc();
    default:
        throw std::invalid_argument("Key length must be 128, 192, or 256 bits.");
    }
}

HMAC::HMAC(const CharVec& key){
    // If key is not provided, sample a random key, we use BLAKE2B and manually set the key size to 64.
    if (key.empty()){
        key_.resize(64);
        RAND_bytes(key_.data(), 64);
    }
    else{
        if (key.size() != 64) throw std::invalid_argument("Key length must be 64 bytes.");
        key_ = key;
    }

    // Fetch the desired algorithm to use.
    mac_ = EVP_MAC_fetch(nullptr, "HMAC", nullptr);

    // Get the ctx object.
    ctx_ = EVP_MAC_CTX_new(mac_);
}

HMAC::~HMAC(){
    EVP_MAC_CTX_free(ctx_);
    EVP_MAC_free(mac_);
}

CharVec HMAC::get_key(){ return key_; }

void HMAC::set_key(const CharVec& key){ key_ = key; }

CharVec HMAC::digest(const CharVec& data) const{
    // Set the parameters.
    const OSSL_PARAM params[] = {
            OSSL_PARAM_construct_utf8_string(
                OSSL_MAC_PARAM_DIGEST,
                const_cast<char*>(digest_.data()),
                digest_.size()
            ),
            OSSL_PARAM_construct_end()
        };

    // Initialize the HMAC.
    EVP_MAC_init(ctx_, key_.data(), key_.size(), params);
    // Update HMAC with input data.
    EVP_MAC_update(ctx_, data.data(), data.size());
    // Create the holder for output.
    CharVec out(64);
    // Finalize the HMAC.
    EVP_MAC_final(ctx_, out.data(), nullptr, 64);

    return out;
}

FpVec HMAC::digest_vec_to_fp(const Vec& x, const IntVec& sel) const{
    // Create holder for the hash result.
    FpVec r;

    // Depending on the input type, hash the input x vector.
    std::visit([&r, sel, this](auto&& vec_x){
        // Get the type of the input.
        using T = std::decay_t<decltype(vec_x)>;

        // For integer vectors.
        if constexpr (std::is_same_v<T, IntVec>){
            // If the selective is emtpy, we hash x_i + i.
            if (sel.empty())
                for (int i = 0; i < vec_x.size(); ++i){
                    auto more_temp = Helper::int_to_char_vec(vec_x[i] + i);
                    auto temp = digest(Helper::int_to_char_vec(vec_x[i] + i));
                    r.push_back(Helper::char_vec_to_fp(
                        digest(Helper::int_to_char_vec(vec_x[i] + i))
                    ));
                }
            else
                // Otherwise we hash x_i + sel[i].
                for (int i = 0; i < vec_x.size(); ++i)
                    r.push_back(Helper::char_vec_to_fp(
                        digest(Helper::int_to_char_vec(vec_x[i] + sel[i]))
                    ));
        }

        // For string vectors.
        else if constexpr (std::is_same_v<T, StrVec>){
            if (sel.empty())
                // If the selective is emtpy, we hash x_i || i.
                for (int i = 0; i < vec_x.size(); ++i)
                    r.push_back(Helper::char_vec_to_fp(
                        digest(Helper::str_to_char_vec(vec_x[i] + std::to_string(i)))
                    ));
            else
                // Otherwise we hash x_i || sel[i].
                for (int i = 0; i < vec_x.size(); ++i)
                    r.push_back(Helper::char_vec_to_fp(
                        digest(Helper::str_to_char_vec(vec_x[i] + std::to_string(sel[i])))
                    ));
        }

        // Otherwise the type is not supported.
        else{
            throw std::runtime_error("Unsupported hash type");
        }
    }, x);

    return r;
}

FpMat HMAC::digest_mat_to_fp(const Mat& x, const IntVec& sel) const{
    // Create holder for the hash result.
    FpMat r;

    // Depending on the input type, hash the input x matrix.
    std::visit([&r, sel, this](auto&& mat_x){
        // Get the type of the input.
        using T = std::decay_t<decltype(mat_x)>;

        // For integer matrix.
        if constexpr (std::is_same_v<T, IntMat>){
            // If we do not need to select values.
            if (sel.empty()){
                for (int i = 0; i < mat_x.size(); ++i){
                    // Create holder for hash result of this row.
                    FpVec row_hash;

                    // This row is hashed with row + i.
                    for (const auto& each_x : mat_x[i])
                        row_hash.push_back(Helper::char_vec_to_fp(
                            digest(Helper::int_to_char_vec(each_x + i))
                        ));

                    // Add the hashed value back to r.
                    r.push_back(row_hash);
                }
            }
            else{
                for (int i = 0; i < mat_x.size(); ++i){
                    // Create holder for hash result of this row.
                    FpVec row_hash;

                    // This row is hashed with row + sel[i].
                    for (const auto& each_x : mat_x[i])
                        row_hash.push_back(Helper::char_vec_to_fp(
                            digest(Helper::int_to_char_vec(each_x + sel[i]))
                        ));

                    // Add the hashed value back to r.
                    r.push_back(row_hash);
                }
            }
        }
        else if constexpr (std::is_same_v<T, StrMat>){
            // If we do not need to select values.
            if (sel.empty()){
                for (int i = 0; i < mat_x.size(); ++i){
                    // Create holder for hash result of this row.
                    FpVec row_hash;

                    // This row is hashed with row + i.
                    for (const auto& each_x : mat_x[i])
                        row_hash.push_back(Helper::char_vec_to_fp(
                            digest(Helper::str_to_char_vec(each_x + std::to_string(i)))
                        ));

                    // Add the hashed value back to r.
                    r.push_back(row_hash);
                }
            }
            else{
                for (int i = 0; i < mat_x.size(); ++i){
                    // Create holder for hash result of this row.
                    FpVec row_hash;

                    // This row is hashed with row + sel[i].
                    for (const auto& each_x : mat_x[i])
                        row_hash.push_back(Helper::char_vec_to_fp(
                            digest(Helper::str_to_char_vec(each_x + std::to_string(sel[i])))
                        ));

                    // Add the hashed value back to r.
                    r.push_back(row_hash);
                }
            }
        }
        else{
            throw std::runtime_error("Unsupported hash type");
        }
    }, x);

    return r;
}

FpVec HMAC::digest_vec_to_fp_mod(const BP& pairing_group, const Vec& x, const IntVec& sel) const{
    // Get the hash result and perform modulo.
    FpVec r = digest_vec_to_fp(x, sel);
    pairing_group.Zp->mod(r);
    return r;
}

FpMat HMAC::digest_mat_to_fp_mod(const BP& pairing_group, const Mat& x, const IntVec& sel) const{
    // Get the hash result and perform modulo.
    FpMat r = digest_mat_to_fp(x, sel);
    pairing_group.Zp->mod(r);
    return r;
}
