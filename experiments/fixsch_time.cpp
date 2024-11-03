#include <chrono>
#include <fstream>
#include "ipe_filter.hpp"

static int ROUND = 100;

void setup_time(){
    // Open the output files.
    std::ofstream file("fixsch_time.txt", std::ios_base::app);
    file << "Fixsch Setup Timings" << std::endl << std::endl;

    // Setup and Encryption time.
    for (int length = 10; length <= 200; length += 10){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Create pp.
        auto pp = IpeFilter::pp_gen(5, length);

        // Perform ROUND number of setup.
        for (int i = 0; i < ROUND; ++i){
            // Setup timings.
            auto start = std::chrono::high_resolution_clock::now();
            auto msk = IpeFilter::msk_gen(pp);
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;
        }

        // Output the time.
        file << "(" << length << ", " << time.count() / ROUND << ")" << std::endl;
    }
    // Create some blank spaces.
    file << std::endl << std::endl;
}

void enc_time(){
    // Open the output files.
    std::ofstream file("fixsch_time.txt", std::ios_base::app);
    file << "Fixsch Encryption Timings" << std::endl << std::endl;

    // Setup and Encryption time.
    for (int length = 10; length <= 200; length += 10){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Create msk and pp.
        auto pp = IpeFilter::pp_gen(5, length);
        auto msk = IpeFilter::msk_gen(pp);

        // Perform ROUND number of setup and enc.
        for (int i = 0; i < ROUND; ++i){
            // Create a random vector of desired length.
            auto x = Helper::rand_int_vec(length, 1, 1000);

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


int main(){
    setup_time();
    enc_time();
}