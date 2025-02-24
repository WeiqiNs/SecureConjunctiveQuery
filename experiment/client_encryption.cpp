#include "exp.hpp"

void ipe_client_enc_time(const int round){
    // Open the output files.
    std::ofstream file("client_enc_time.txt", std::ios_base::app);
    file << "IPE Timings" << std::endl;

    for (int length = 10; length <= 200; length += 10){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Create pp and msk.
        auto pp = IpeFilter::pp_gen(1, length);
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

void sse_client_enc_time(const int round){
    // Open the output files.
    std::ofstream file("client_enc_time.txt", std::ios_base::app);
    file << "SSE Timings" << std::endl;

    // Setup and Encryption time.
    for (int length = 10; length <= 200; length += 10){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Create pp and msk.
        auto msk = SseFilter::msk_gen();

        // Perform ROUND number of setup.
        for (int i = 0; i < round; ++i){
            // Create a random vector of desired length.
            auto x = Helper::rand_int_vec(length, 1, std::numeric_limits<int>::max());

            // Encryption timings.
            auto start = std::chrono::high_resolution_clock::now();
            std::ignore = SseFilter::enc(msk, x);
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;
        }

        // Output the time.
        file << "(" << length << ", " << time.count() / round << ")" << std::endl;
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
        auto pp = Filter::pp_gen(1, length);
        auto msk = Filter::msk_gen(pp);

        // Perform ROUND number of Enc.
        for (int i = 0; i < round; ++i){
            // Create a random vector of desired length.
            auto x = Helper::rand_int_vec(length, 1, std::numeric_limits<int>::max());

            // Encryption timings.
            auto start = std::chrono::high_resolution_clock::now();
            std::ignore = Filter::enc(pp, msk, x);
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

void bench_enc_time(const int round){
    ipe_client_enc_time(round);
    sse_client_enc_time(round);
    our_client_enc_time(round);
}
