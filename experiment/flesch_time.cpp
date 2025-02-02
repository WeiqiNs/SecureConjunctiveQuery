#include <chrono>
#include <fstream>
#include "filter.hpp"

static int ROUND = 100;

void filter_setup_time(){
    // Open the output files.
    std::ofstream file("flesch_time.txt", std::ios_base::app);
    file << "Flesch Setup Timings" << std::endl << std::endl;

    // Setup and Encryption time.
    for (int length = 10; length <= 200; length += 10){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Create pp.
        auto pp = Filter::pp_gen(5, length);

        // Perform ROUND number of setup.
        for (int i = 0; i < ROUND; ++i){
            // Setup timings.
            auto start = std::chrono::high_resolution_clock::now();
            auto msk = Filter::msk_gen(pp);
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;
        }

        // Output the time.
        file << "(" << length << ", " << time.count() / ROUND << ")" << std::endl;
    }
    // Create some blank spaces.
    file << std::endl << std::endl;
}

void filter_enc_time(){
    // Open the output files.
    std::ofstream file("flesch_time.txt", std::ios_base::app);
    file << "Flesch Encryption Timings" << std::endl << std::endl;

    // Setup and Encryption time.
    for (int length = 10; length <= 200; length += 10){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Create msk and pp.
        auto pp = Filter::pp_gen(5, length);
        auto msk = Filter::msk_gen(pp);

        // Perform ROUND number of setup and enc.
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

void filter_fixed_total_col_keygen_time(){
    // Open the output files.
    std::ofstream file("flesch_time.txt", std::ios_base::app);
    file << "Flesch Fixed Total 20 Columns Timings (degree = 1)" << std::endl << std::endl;

    // Setup and Encryption time.
    for (int filter_length = 1; filter_length <= 20; ++filter_length){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Create msk and pp.
        auto pp = Filter::pp_gen(1, 20);
        auto msk = Filter::msk_gen(pp);

        // Perform ROUND number of setup and enc.
        for (int i = 0; i < ROUND; ++i){
            // Create the filter indexes.
            IntVec sel;
            for (int j = 0; j < filter_length; ++j) sel.push_back(j);

            // Create the random y vector.
            auto y = Helper::rand_int_vec(filter_length, 1, 65536);

            // Encryption timings.
            auto start = std::chrono::high_resolution_clock::now();
            auto ct = Filter::keygen(pp, msk, y, sel);
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;
        }

        // Output the time.
        file << "(" << filter_length << ", " << time.count() / ROUND << ")" << std::endl;
    }
    // Create some blank spaces.
    file << std::endl << std::endl;

    // For degree 5.
    file << "Flesch Fixed Total 20 Columns Timings (degree = 5)" << std::endl << std::endl;

    // Setup and Encryption time.
    for (int filter_length = 1; filter_length <= 20; ++filter_length){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Create msk and pp.
        auto pp = Filter::pp_gen(5, 20);
        auto msk = Filter::msk_gen(pp);

        // Perform ROUND number of setup and enc.
        for (int i = 0; i < ROUND; ++i){
            // Create the filter indexes.
            IntVec sel;
            for (int j = 0; j < filter_length; ++j) sel.push_back(j);

            // Create a random matrix with desired number of columns to filter.
            IntMat y;
            for (int j = 0; j < filter_length; ++j) y.push_back(Helper::rand_int_vec(5, 1, 65536));

            // Encryption timings.
            auto start = std::chrono::high_resolution_clock::now();
            auto ct = Filter::keygen(pp, msk, y, sel);
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;
        }

        // Output the time.
        file << "(" << filter_length << ", " << time.count() / ROUND << ")" << std::endl;
    }
    // Create some blank spaces.
    file << std::endl << std::endl;
}

void filter_fixed_filter_col_keygen_time(){
    // Open the output files.
    std::ofstream file("flesch_time.txt", std::ios_base::app);
    file << "Flesch Fixed Filter Columns Timings (degree = 1)" << std::endl << std::endl;

    // Setup and Encryption time.
    for (int total_length = 5; total_length <= 20; ++total_length){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Create msk and pp.
        auto pp = Filter::pp_gen(1, total_length);
        auto msk = Filter::msk_gen(pp);

        // Perform ROUND number of setup and enc.
        for (int i = 0; i < ROUND; ++i){
            // Create the filter indexes.
            IntVec sel;
            for (int j = 0; j < 5; ++j) sel.push_back(j);
            // Create a random matrix with desired number of columns to filter.
            IntVec y = Helper::rand_int_vec(5, 1, 65536);

            // Encryption timings.
            auto start = std::chrono::high_resolution_clock::now();
            auto ct = Filter::keygen(pp, msk, y, sel);
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;
        }

        // Output the time.
        file << "(" << total_length << ", " << time.count() / ROUND << ")" << std::endl;
    }
    // Create some blank spaces.
    file << std::endl << std::endl;

    // For degree 5.
    file << "Flesch Fixed Filter Columns Timings (degree = 5)" << std::endl << std::endl;

    // Setup and Encryption time.
    for (int total_length = 5; total_length <= 20; ++total_length){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Create msk and pp.
        auto pp = Filter::pp_gen(5, total_length);
        auto msk = Filter::msk_gen(pp);

        // Perform ROUND number of setup and enc.
        for (int i = 0; i < ROUND; ++i){
            // Create the filter indexes.
            IntVec sel;
            for (int j = 0; j < 5; ++j) sel.push_back(j);
            // Create a random matrix with desired number of columns to filter.
            IntMat y = Helper::rand_int_mat(5, 5, 1, 65536);

            // Encryption timings.
            auto start = std::chrono::high_resolution_clock::now();
            auto ct = Filter::keygen(pp, msk, y, sel);
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;
        }

        // Output the time.
        file << "(" << total_length << ", " << time.count() / ROUND << ")" << std::endl;
    }
    // Create some blank spaces.
    file << std::endl << std::endl;
}

int main(){
    filter_setup_time();
    filter_enc_time();
    filter_fixed_total_col_keygen_time();
    filter_fixed_filter_col_keygen_time();
}
