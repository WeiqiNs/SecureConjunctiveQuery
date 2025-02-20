#include "exp.hpp"

void ipe_enc_time(const int round){
    // Open the output files.
    std::ofstream file("enc_time.txt", std::ios_base::app);
    file << "IPE Enc Timings" << std::endl;

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
            auto ct = IpeFilter::enc(pp, msk, x);
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

void our_enc_time(const int round){
    // Open the output files.
    std::ofstream file("enc_time.txt", std::ios_base::app);
    file << "Our Enc Timings" << std::endl;

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
            auto ct = Filter::enc(pp, msk, x);
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

void sse_enc_time(const int round){
    // Open the output files.
    std::ofstream file("enc_time.txt", std::ios_base::app);
    file << "SSE Enc Timings" << std::endl;

    // Setup and Encryption time.
    for (int length = 10; length <= 200; length += 10){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Create pp and msk.
        auto prf = PRF();

        // Perform ROUND number of setup.
        for (int i = 0; i < round; ++i){
            // Create a random vector of desired length.
            auto x = Helper::rand_int_vec(length, 1, std::numeric_limits<int>::max());

            // Setup timings.
            auto start = std::chrono::high_resolution_clock::now();
            for (const auto each_x : x) auto ct = prf.digest(Helper::int_to_char_vec(each_x));
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;
        }

        // Output the time.
        file << "(" << length << ", " << time.count() / round << ")" << std::endl;
    }
    // Create some blank spaces.
    file << std::endl << std::endl;
}

void bench_enc_time(const int round){
    ipe_enc_time(round);
    our_enc_time(round);
    sse_enc_time(round);
}
