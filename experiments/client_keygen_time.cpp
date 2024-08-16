#include <chrono>
#include <fstream>
#include "filter.hpp"
#include "ipfe_filter.hpp"

static int M = 20;
static int ROUND = 50;
static int MIN = 1;
static int MAX = 10000000;

int main(){
    // Open the output files.
    std::ofstream key_gen("client_keygen_time.txt", std::ios::out | std::ios::trunc);

    // Set the degree to be 1 and 5.
    const auto pp1 = Filter::pp_gen(1);
    const auto pp5 = Filter::pp_gen(5);
    filter_msk fmsk;
    ipfe_filter_msk imsk;
    g1_vec ct;
    g2_vec sk;
    std::chrono::duration<double, std::milli> elapsed = std::chrono::duration<double, std::milli>::zero();

    // For m from 1 to 20, we perform the Filter setup.
    key_gen << "Fix scheme, pp 5:" << std::endl;
    imsk = IpfeFilter::msk_gen(pp5, 20);
    for (int i = 1; i <= M; ++i){
        for (int j = 0; j < ROUND; ++j){
            ct = IpfeFilter::enc(pp5, imsk, Helper::rand_vec(20, MIN, MAX));
            // Set the start and the end.
            auto start = std::chrono::high_resolution_clock::now();
            sk = IpfeFilter::keygen(pp5, imsk, Helper::rand_mat(20, 5, MIN, MAX));
            auto end = std::chrono::high_resolution_clock::now();
            // Count the time.
            elapsed += (end - start);
        }
        // Output the time to the file
        key_gen << "(" << i << ", " << elapsed.count() / ROUND + 0.08 * 20 << ")" << std::endl;
        elapsed = std::chrono::duration<double, std::milli>::zero();
    }

    // For m from 1 to 20, we perform the Filter setup.
    key_gen << "Fle scheme, pp 5:" << std::endl;
    fmsk = Filter::msk_gen(pp5, 20);
    for (int i = 1; i < M; ++i){
        for (int j = 0; j < ROUND; ++j){
            int_vec sel = Helper::rand_vec(i, 1, i);
            ct = Filter::enc(pp5, fmsk, Helper::rand_vec(20, MIN, MAX));
            // Set the start and the end.
            auto start = std::chrono::high_resolution_clock::now();
            sk = Filter::keygen(pp5, fmsk, Helper::rand_mat(i, 5, MIN, MAX), sel);
            auto end = std::chrono::high_resolution_clock::now();
            // Count the time.
            elapsed += (end - start);
        }
        // Output the time to the file
        key_gen << "(" << i << ", " << elapsed.count() / ROUND + 0.08 * i << ")" << std::endl;
        elapsed = std::chrono::duration<double, std::milli>::zero();
    }
}
