#include "exp.hpp"

void ipe_client_enc_time(const int round){
    // Open the output files.
    std::ofstream file("client_enc_time.txt", std::ios_base::app);
    file << "IPE Timings" << std::endl;

    for (int length = 10; length <= 200; length += 10){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Create pp and msk.
        auto pp = IpeFilter::pp_gen(5, length);
        auto msk = IpeFilter::msk_gen(pp);

        // Perform ROUND number of Enc.
        for (int i = 0; i < round; ++i){
            // Create a random vector of desired length.
            auto x = Helper::rand_int_vec(length, 1, std::numeric_limits<int>::max());

            // Encryption timings.
            auto start = std::chrono::high_resolution_clock::now();
            std::ignore = IpeFilter::enc(pp, msk, x);
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;
        }

        // Output the time.
        file << "(" << length << ", " << time.count() / round << ")" << std::endl;

        // Close the BP.
        BP::close();
    }
    // Create some blank spaces.
    file << std::endl << std::endl;
}


void our_client_enc_time(const int round){
    // Open the output files.
    std::ofstream file("client_enc_time.txt", std::ios_base::app);
    file << "Our Timings" << std::endl;

    for (int length = 10; length <= 200; length += 10){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Create pp and msk.
        auto pp = FlexDbMf::pp_gen(5, length);
        auto msk = FlexDbMf::msk_gen(pp);

        // Perform ROUND number of Enc.
        for (int i = 0; i < round; ++i){
            // Create a random vector of desired length.
            auto x = Helper::rand_int_vec(length, 1, std::numeric_limits<int>::max());

            // Encryption timings.
            auto start = std::chrono::high_resolution_clock::now();
            std::ignore = FlexDbMf::enc(pp, msk, x);
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;
        }

        // Output the time.
        file << "(" << length << ", " << time.count() / round << ")" << std::endl;

        // Close the BP.
        BP::close();
    }
    // Create some blank spaces.
    file << std::endl << std::endl;
}

void bench_client_enc_time(const int round){
    ipe_client_enc_time(round);
    our_client_enc_time(round);
}
