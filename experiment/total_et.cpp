#include "exp.hpp"

void ipe_total_et_time(const int round){
    // Open the output files.
    std::ofstream file("total_et_time.txt", std::ios_base::app);
    file << "IPE Timings" << std::endl;

    // Generate pp and msk.
    auto pp = IpeEt::pp_gen(20);
    auto msk = IpeEt::msk_gen(pp);

    for (int num_col : {2, 10, 20}){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Perform round number of Enc.
        for (int i = 0; i < round; ++i){
            // Create a random vector of desired length.
            auto y = Helper::rand_int_vec(20, 1, 5);
            // Set the unselected portion to zero.
            for (int j = num_col + 1; j < 20; ++j){ y[j] = 0; }

            // Total timings.
            auto start = std::chrono::high_resolution_clock::now();
            auto sk = IpeEt::keygen(pp, msk, y, 100);
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;

            for (int j = 0; j < pow(2, 20); ++j){
                // Compute ciphertext.
                auto ct = IpeEt::enc(pp, msk, Helper::rand_int_vec(20, 1, std::numeric_limits<int>::max()));

                // Decryption timings.
                start = std::chrono::high_resolution_clock::now();
                std::ignore = IpeEt::dec(ct, sk);
                end = std::chrono::high_resolution_clock::now();
                time += end - start;
            }
        }

        // Output the time.
        file << "(" << num_col << ", " << time.count() / round << ")" << std::endl;
    }

    // Close the BP.
    BP::close();

    // Create some blank spaces.
    file << std::endl << std::endl;
}

void our_total_et_time(const int round){
    // Open the output files.
    std::ofstream file("total_et_time.txt", std::ios_base::app);
    file << "Our Timings" << std::endl;

    // Generate pp and msk.
    auto pp = FlexDbEt::pp_gen(20);
    auto msk = FlexDbEt::msk_gen(pp);

    for (int num_col : {2, 10, 20}){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Perform round number of Enc.
        for (int i = 0; i < round; ++i){
            // Create a random vector of desired length.
            auto x = Helper::rand_int_vec(20, 1, std::numeric_limits<int>::max());
            // Create a random vector of desired length.
            auto y = Helper::rand_int_vec(num_col, 1, 5);

            // Set the unselected portion to zero.
            IntVec sel;
            for (int j = 0; j < num_col; ++j){ sel.push_back(j); }

            // Decryption timings.
            auto start = std::chrono::high_resolution_clock::now();
            auto sk = FlexDbEt::keygen(pp, msk, y, 100, sel);
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;

            for (int j = 0; j < pow(2, 20); ++j){
                // Compute ciphertext.
                auto ct = FlexDbEt::enc(pp, msk, x);

                // Add decryption time.
                start = std::chrono::high_resolution_clock::now();
                std::ignore = FlexDbEt::dec(ct, sk, sel);
                end = std::chrono::high_resolution_clock::now();
                time += end - start;
            }
        }

        // Output the time.
        file << "(" << num_col << ", " << time.count() / round << ")" << std::endl;
    }

    // Close the BP.
    BP::close();

    // Create some blank spaces.
    file << std::endl << std::endl;
}

void bench_total_et_time(const int round){
    ipe_total_et_time(round);
    our_total_et_time(round);
}
