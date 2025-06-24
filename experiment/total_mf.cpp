#include "exp.hpp"

void ipe_total_mf_time(const int round){
    // Open the output files.
    std::ofstream file("total_mf_time.txt", std::ios_base::app);
    file << "IPE Timings" << std::endl;

    // Generate pp and msk.
    auto pp = IpeFilter::pp_gen(5, 20);
    auto msk = IpeFilter::msk_gen(pp);

    for (int num_col = 1; num_col <= 20; ++num_col){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Perform round number of Enc.
        for (int i = 0; i < round; ++i){
            // Create a random vector of desired length.
            auto x = Helper::rand_int_vec(20, 1, std::numeric_limits<int>::max());
            auto y = Helper::rand_int_mat(20, 5, 1, std::numeric_limits<int>::max());
            // Set the unselected portion to zero.
            for (int j = num_col + 1; j < 20; ++j) for (int k = 0; k < 5; ++k) y[j][k] = 0;

            // Total timings.
            auto start = std::chrono::high_resolution_clock::now();
            auto sk = IpeFilter::keygen(pp, msk, y);
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;

            for (int j = 0; j < 100; ++j){
                // Compute ciphertext.
                auto ct = IpeFilter::enc(pp, msk, x);

                // Add decryption time.
                start = std::chrono::high_resolution_clock::now();
                std::ignore = IpeFilter::dec(ct, sk);
                end = std::chrono::high_resolution_clock::now();
                time += (end - start) * 100;
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

void our_total_mf_time(const int round){
    // Open the output files.
    std::ofstream file("total_mf_time.txt", std::ios_base::app);
    file << "Our Timings" << std::endl;

    // Generate pp and msk.
    auto pp = FlexDbMf::pp_gen(5, 20);
    auto msk = FlexDbMf::msk_gen(pp);

    for (int num_col = 1; num_col <= 20; ++num_col){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Perform round number of Enc.
        for (int i = 0; i < round; ++i){
            // Create a random vector of desired length.
            auto x = Helper::rand_int_vec(20, 1, std::numeric_limits<int>::max());
            // Create a random vector of desired length.
            auto y = Helper::rand_int_mat(num_col, 5, 1, std::numeric_limits<int>::max());

            // Set the unselected portion to zero.
            IntVec sel;
            for (int j = 0; j < num_col; ++j){ sel.push_back(j); }

            // Total timings.
            auto start = std::chrono::high_resolution_clock::now();
            auto sk = FlexDbMf::keygen(pp, msk, y, sel);
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;

            for (int j = 0; j < 100; ++j){
                // Compute ciphertext.
                auto ct = FlexDbMf::enc(pp, msk, x);

                // Add decryption time.
                start = std::chrono::high_resolution_clock::now();
                std::ignore = FlexDbMf::dec(pp, ct, sk, sel);
                end = std::chrono::high_resolution_clock::now();
                time += (end - start) * 100;
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

void bench_total_mf_time(const int round){
    ipe_total_mf_time(round);
    our_total_mf_time(round);
}
