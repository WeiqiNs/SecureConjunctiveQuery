#include "crypto.hpp"

int test_aes_128() {
    const auto aes = Aes(128);

    const str plaintext = "This is a secret message.";

    const char_vec plaintext_vec = Helper::str_to_vec(plaintext);

    const char_vec ciphertext = aes.encrypt(plaintext_vec);

    return aes.decrypt(ciphertext) == plaintext_vec;
}

int test_aes_256() {
    const auto aes = Aes(256);

    const str plaintext = "This is another secret message.";

    const char_vec plaintext_vec = Helper::str_to_vec(plaintext);

    const char_vec ciphertext = aes.encrypt(plaintext_vec);

    return aes.decrypt(ciphertext) == plaintext_vec;
}

int test_hash(){
    const auto hash = Hash();

    const str data = "Something to hash.";

    const char_vec data_vec = Helper::str_to_vec(data);

    const char_vec hash_one = hash.digest(data_vec);

    const char_vec hash_two = hash.digest(data_vec);

    return hash_one == hash_two;
}


int main() {
    // Perform test.
    if (test_aes_128() != 1) return 1;
    if (test_aes_256() != 1) return 1;
    if (test_hash() != 1) return 1;

    return 0;
}
