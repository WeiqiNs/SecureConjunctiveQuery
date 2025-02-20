/// This file benchmarks when total length is 20 and selecting all columns, the cost of filtering n rows.
/// Note this is client query operation time, hence keygen is used.

#include "exp.hpp"

void ipe_mul_row_time(const int round){
    // Open the output files.
    std::ofstream time_file("multiple_row_time.txt", std::ios_base::app);
    time_file << "IPE Row Timings" << std::endl;

    std::ofstream storage_file("multiple_row_storage.txt", std::ios_base::app);
    storage_file << "IPE Row Storage" << std::endl;

    for (int num_row = 1; num_row <= 20; ++num_row){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Create pp and msk.
        auto pp = IpeFilter::pp_gen(1, 20);
        auto msk = IpeFilter::msk_gen(pp);
        G2Vec sk;

        // Perform round number of Enc.
        for (int i = 0; i < round; ++i){
            // Create a random vector of desired length.
            auto y = Helper::rand_int_mat(20, 1, 1, std::numeric_limits<int>::max());

            // Encryption timings.
            auto start = std::chrono::high_resolution_clock::now();
            sk = IpeFilter::keygen(pp, msk, y);
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;
        }

        // Compute the sk size.
        unsigned long sk_size = sk.size() * sizeof(uint8_t) * pp.pairing_group->Gp->g2_size;

        // Output the time.
        time_file << "(" << num_row << ", " << time.count() / round << ")" << std::endl;

        // Output the storage.
        storage_file << "(" << num_row << ", " << sk_size << ")" << std::endl;

        // Close the BP.
        BP::close();
    }
    // Create some blank spaces.
    time_file << std::endl << std::endl;
    storage_file << std::endl << std::endl;
}

void our_mul_row_time(const int round){
    // Open the output files.
    std::ofstream time_file("multiple_row_time.txt", std::ios_base::app);
    time_file << "Our Row Timings" << std::endl;

    std::ofstream storage_file("multiple_row_storage.txt", std::ios_base::app);
    storage_file << "Our Row Storage" << std::endl;

    for (int num_row = 1; num_row <= 20; ++num_row){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Create pp and msk.
        auto pp = Filter::pp_gen(1, 20);
        auto msk = Filter::msk_gen(pp);
        G2Vec sk;

        // Perform round number of Enc.
        for (int i = 0; i < round; ++i){
            // Create a random vector of desired length.
            auto y = Helper::rand_int_vec(20, 1, std::numeric_limits<int>::max());

            // Encryption timings.
            auto start = std::chrono::high_resolution_clock::now();
            sk = Filter::keygen(pp, msk, y);
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;
        }

        // Compute the secret key size.
        unsigned long sk_size = sk.size() * sizeof(uint8_t) * pp.pairing_group->Gp->g2_size;

        // Output the time.
        time_file << "(" << num_row << ", " << time.count() / round << ")" << std::endl;

        // Output the storage.
        storage_file << "(" << num_row << ", " << sk_size << ")" << std::endl;

        // Close the BP.
        BP::close();
    }
    // Create some blank spaces.
    time_file << std::endl << std::endl;
    storage_file << std::endl << std::endl;
}

void sse_mul_row_time(const int round){
    // Open the output files.
    std::ofstream time_file("multiple_row_time.txt", std::ios_base::app);
    time_file << "SSE Row Timings" << std::endl;

    std::ofstream storage_file("multiple_row_storage.txt", std::ios_base::app);
    storage_file << "SSE Row Storage" << std::endl;

    for (int num_row = 1; num_row <= 20; ++num_row){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Compute the number of rows used.
        int pow_row = static_cast<int>(std::pow(2, num_row));

        // Create pp and msk.
        auto prf = PRF();
        CharVec sk;

        // Perform ROUND number of setup.
        for (int i = 0; i < round; ++i){
            for (int j = 0; j < pow_row; ++j){
                // Create a random vector of desired length.
                auto y = Helper::rand_int_vec(20, 1, std::numeric_limits<int>::max());

                // Enc timings.
                auto start = std::chrono::high_resolution_clock::now();
                for (const auto each_y : y) sk = prf.digest(Helper::int_to_char_vec(each_y));
                auto end = std::chrono::high_resolution_clock::now();
                time += end - start;
            }
        }

        // Also create holder for size.
        unsigned long sk_size = sk.size() * sizeof(unsigned char) * 20 * pow_row;

        // Output the time.
        time_file << "(" << num_row << ", " << time.count() / round << ")" << std::endl;

        // Output the storage.
        storage_file << "(" << num_row << ", " << sk_size << ")" << std::endl;
    }
    // Create some blank spaces.
    time_file << std::endl << std::endl;
    storage_file << std::endl << std::endl;
}

void bench_mul_row_time(const int round){
    ipe_mul_row_time(round);
    our_mul_row_time(round);
    sse_mul_row_time(round);
}
