#include <chrono>
#include <random>
#include <fstream>
#include "filter.hpp"
#include "ipfe_filter.hpp"

static int ROUND = 50;
static int MINR = 1;
static int MAXR = 1000;

int_vec rand_vec(const int length, const int seed){
    std::vector<int> r;
    r.reserve(length);

    // Initialize random number generator with a seed based on current time
    std::mt19937 generator(seed);

    // Define the range for the random numbers
    std::uniform_int_distribution distribution(MINR, MAXR);

    for (int i = 0; i < length; ++i){
        r.push_back(distribution(generator));
    }

    return r;
}

int main(){
    // Open the output files.
    std::ofstream setup_file("client_setup_time.txt", std::ios_base::app);
    std::ofstream enc_file("client_enc_time.txt", std::ios_base::app);

    // Set the degree to be 5.
    const auto pp = Filter::pp_gen(5);
    filter_msk fmsk;
    ipfe_filter_msk imsk;

    // For m from 4 to 20, we perform the Filter setup.
    setup_file << "Filter Setup time (ms): " << std::endl;
    enc_file << "Filter Enc time (ms): " << std::endl;
    for (int i = 4; i <= 20; ++i){
        // Set the start and the end.
        auto start = std::chrono::high_resolution_clock::now();
        for (int j = 0; j < ROUND; ++j){
             fmsk = Filter::msk_gen(pp, i);
        }
        auto end = std::chrono::high_resolution_clock::now();
        // Count the time.
        std::chrono::duration<double, std::milli> elapsed = end - start;
        // Output the time to the file
        setup_file << "(" << i << ", " << elapsed.count() / ROUND << ")" << std::endl;

        // We now count the Enc time, first create a vector.
        int_vec r = rand_vec(i, i);
        // Count time.
        start = std::chrono::high_resolution_clock::now();
        for (int j = 0; j < ROUND; ++j){
            auto ct = Filter::enc(pp, fmsk, r);
        }
        end = std::chrono::high_resolution_clock::now();
        elapsed = end - start;
        // Output the time to the file
        enc_file << "(" << i << ", " << elapsed.count() / ROUND << ")" << std::endl;
    }
    setup_file << "-------------------------------------------------------------------------" << std::endl;
    enc_file << "-------------------------------------------------------------------------" << std::endl;

    // For m from 4 to 20, we perform the Filter setup.
    setup_file << "IPFE Filter Setup time (ms): " << std::endl;
    enc_file << "IPFE Filter Enc time (ms): " << std::endl;
    for (int i = 4; i <= 10; ++i){
        // Set the start and the end.
        auto start = std::chrono::high_resolution_clock::now();
        for (int j = 0; j < ROUND; ++j){
            imsk = IpfeFilter::msk_gen(pp, i);
        }
        auto end = std::chrono::high_resolution_clock::now();
        // Count the time.
        std::chrono::duration<double, std::milli> elapsed = end - start;
        // Output the time to the file
        setup_file << "(" << i << ", " << elapsed.count() / ROUND << ")" << std::endl;

        // We now count the Enc time, first create a vector.
        int_vec r = rand_vec(i, i);
        // Count time.
        start = std::chrono::high_resolution_clock::now();
        for (int j = 0; j < ROUND; ++j){
            auto ct = IpfeFilter::enc(pp, imsk, r);
        }
        end = std::chrono::high_resolution_clock::now();
        elapsed = end - start;
        // Output the time to the file
        enc_file << "(" << i << ", " << elapsed.count() / ROUND << ")" << std::endl;
    }
    setup_file << "-------------------------------------------------------------------------" << std::endl;
    setup_file << "-------------------------------------------------------------------------" << std::endl << std::endl;
    enc_file << "-------------------------------------------------------------------------" << std::endl;
    enc_file << "-------------------------------------------------------------------------" << std::endl << std::endl;
}
