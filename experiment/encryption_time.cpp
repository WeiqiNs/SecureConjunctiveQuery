#include <chrono>
#include <fstream>
#include "filter.hpp"
#include "ipe_filter.hpp"

static int ROUND = 10;

void ipe_enc_time(){
    // Open the output files.
    std::ofstream file("ipe_enc_time.txt", std::ios_base::app);
    file << "IPE Enc Timings" << std::endl << std::endl;

    for (int length = 10; length <= 200; length += 10){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Create pp and msk.
        auto pp = IpeFilter::pp_gen(1, length);
        auto msk = IpeFilter::msk_gen(pp);

        // Perform ROUND number of Enc.
        for (int i = 0; i < ROUND; ++i){
            // Create a random vector of desired length.
            auto x = Helper::rand_int_vec(length, 1, 65536);

            // Encryption timings.
            auto start = std::chrono::high_resolution_clock::now();
            auto ct = IpeFilter::enc(pp, msk, x);
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;
        }

        // Output the time.
        file << "(" << length << ", " << time.count() / ROUND << ")" << std::endl;
    }
    // Create some blank spaces.
    file << std::endl << std::endl;
}

void our_enc_time(){
    // Open the output files.
    std::ofstream file("our_enc_time.txt", std::ios_base::app);
    file << "Our Enc Timings" << std::endl << std::endl;

    for (int length = 10; length <= 200; length += 10){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Create pp and msk.
        auto pp = Filter::pp_gen(1, length);
        auto msk = Filter::msk_gen(pp);

        // Perform ROUND number of Enc.
        for (int i = 0; i < ROUND; ++i){
            // Create a random vector of desired length.
            auto x = Helper::rand_int_vec(length, 1, 65536);

            // Encryption timings.
            auto start = std::chrono::high_resolution_clock::now();
            auto ct = Filter::enc(pp, msk, x);
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;
        }

        // Output the time.
        file << "(" << length << ", " << time.count() / ROUND << ")" << std::endl;
    }
    // Create some blank spaces.
    file << std::endl << std::endl;
}

void sse_enc_time(){
    // Open the output files.
    std::ofstream file("sse_enc_time.txt", std::ios_base::app);
    file << "SSE Enc Timings" << std::endl << std::endl;

    // Setup and Encryption time.
    for (int length = 10; length <= 200; length += 10){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Create pp and msk.
        auto prf = PRF();

        // Perform ROUND number of setup.
        for (int i = 0; i < ROUND; ++i){
            // Create a random vector of desired length.
            auto x = Helper::rand_int_vec(length, 1, 65536);

            // Setup timings.
            auto start = std::chrono::high_resolution_clock::now();
            for (const auto each_x : x) auto ct = prf.digest(Helper::int_to_char_vec(each_x));
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;
        }

        // Output the time.
        file << "(" << length << ", " << time.count() / ROUND << ")" << std::endl;
    }
    // Create some blank spaces.
    file << std::endl << std::endl;
}


int main(){
    ipe_enc_time();
    our_enc_time();
    sse_enc_time();
}
