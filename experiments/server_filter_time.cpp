#include <chrono>
#include <fstream>
#include "filter.hpp"
#include "ipfe_filter.hpp"

static int M = 20;
static int ROUND = 1;
static int MIN = 1;
static int MAX = 10000000;

int main(){
    // Open the output files.
    std::ofstream fix_col_file("fixed_column_time.txt", std::ios::out | std::ios::trunc);
    std::ofstream fix_fil_file("fixed_filter_time.txt", std::ios::out | std::ios::trunc);

    // Set the degree to be 1 and 5.
    const auto pp1 = Filter::pp_gen(1);
    const auto pp5 = Filter::pp_gen(5);
    filter_msk fmsk;
    ipfe_filter_msk imsk;
    g1_vec ct;
    g2_vec sk;
    std::chrono::duration<double, std::milli> elapsed = std::chrono::duration<double, std::milli>::zero();

    // For m from 1 to 20, we perform the Filter setup.
    fix_col_file << "Fix scheme, pp 1:" << std::endl;
    imsk = IpfeFilter::msk_gen(pp1, 20);
    for (int i = 1; i <= M; ++i){
        for (int j = 0; j < ROUND; ++j){
            ct = IpfeFilter::enc(pp1, imsk, Helper::rand_vec(20, MIN, MAX));
            sk = IpfeFilter::keygen(pp1, imsk, Helper::rand_mat(20, 1, MIN, MAX));
            // Set the start and the end.
            auto start = std::chrono::high_resolution_clock::now();
            bool test = IpfeFilter::dec(ct, sk);
            auto end = std::chrono::high_resolution_clock::now();
            // Count the time.
            elapsed += (end - start);
        }
        // Output the time to the file
        fix_col_file << "(" << i << ", " << elapsed.count() / ROUND << ")" << std::endl;
        elapsed = std::chrono::duration<double, std::milli>::zero();
    }

    // For m from 1 to 20, we perform the Filter setup.
    fix_col_file << "Fix scheme, pp 5:" << std::endl;
    imsk = IpfeFilter::msk_gen(pp5, 20);
    for (int i = 1; i <= M; ++i){
        for (int j = 0; j < ROUND; ++j){
            ct = IpfeFilter::enc(pp5, imsk, Helper::rand_vec(20, MIN, MAX));
            sk = IpfeFilter::keygen(pp5, imsk, Helper::rand_mat(20, 5, MIN, MAX));
            // Set the start and the end.
            auto start = std::chrono::high_resolution_clock::now();
            bool test = IpfeFilter::dec(ct, sk);
            auto end = std::chrono::high_resolution_clock::now();
            // Count the time.
            elapsed += (end - start);
        }
        // Output the time to the file
        fix_col_file << "(" << i << ", " << elapsed.count() / ROUND << ")" << std::endl;
        elapsed = std::chrono::duration<double, std::milli>::zero();
    }

    // For m from 1 to 20, we perform the Filter setup.
    fix_fil_file << "Fix scheme, pp 1:" << std::endl;
    for (int i = 5; i <= M; ++i){
        for (int j = 0; j < ROUND; ++j){
            imsk = IpfeFilter::msk_gen(pp1, i);
            ct = IpfeFilter::enc(pp1, imsk, Helper::rand_vec(i, MIN, MAX));
            sk = IpfeFilter::keygen(pp1, imsk, Helper::rand_mat(i, 1, MIN, MAX));
            // Set the start and the end.
            auto start = std::chrono::high_resolution_clock::now();
            bool test = IpfeFilter::dec(ct, sk);
            auto end = std::chrono::high_resolution_clock::now();
            // Count the time.
            elapsed += (end - start);
        }
        // Output the time to the file
        fix_fil_file << "(" << i << ", " << elapsed.count() / ROUND << ")" << std::endl;
        elapsed = std::chrono::duration<double, std::milli>::zero();
    }

    // For m from 1 to 20, we perform the Filter setup.
    fix_fil_file << "Fix scheme, pp 5:" << std::endl;
    for (int i = 5; i <= M; ++i){
        for (int j = 0; j < ROUND; ++j){
            imsk = IpfeFilter::msk_gen(pp5, i);
            ct = IpfeFilter::enc(pp5, imsk, Helper::rand_vec(i, MIN, MAX));
            sk = IpfeFilter::keygen(pp5, imsk, Helper::rand_mat(i, 5, MIN, MAX));
            // Set the start and the end.
            auto start = std::chrono::high_resolution_clock::now();
            bool test = IpfeFilter::dec(ct, sk);
            auto end = std::chrono::high_resolution_clock::now();
            // Count the time.
            elapsed += (end - start);
        }
        // Output the time to the file
        fix_fil_file << "(" << i << ", " << elapsed.count() / ROUND << ")" << std::endl;
        elapsed = std::chrono::duration<double, std::milli>::zero();
    }

    // For m from 1 to 20, we perform the Filter setup.
    fix_col_file << "Fle scheme, pp 1:" << std::endl;
    for (int i = 1; i <= M; ++i){
        for (int j = 0; j < ROUND; ++j){
            fmsk = Filter::msk_gen(pp1, i);
            ct = Filter::enc(pp1, fmsk, Helper::rand_vec(i, MIN, MAX));
            sk = Filter::keygen(pp1, fmsk, Helper::rand_mat(i, 1, MIN, MAX));
            // Set the start and the end.
            auto start = std::chrono::high_resolution_clock::now();
            bool test = Filter::dec(ct, sk);
            auto end = std::chrono::high_resolution_clock::now();
            // Count the time.
            elapsed += (end - start);
        }
        // Output the time to the file
        fix_col_file << "(" << i << ", " << elapsed.count() / ROUND * ((i + 1) / 2.0) / i << ")" << std::endl;
        elapsed = std::chrono::duration<double, std::milli>::zero();
    }

    // For m from 1 to 20, we perform the Filter setup.
    fix_col_file << "Fle scheme, pp 5:" << std::endl;
    for (int i = 1; i <= M; ++i){
        for (int j = 0; j < ROUND; ++j){
            fmsk = Filter::msk_gen(pp5, i);
            ct = Filter::enc(pp5, fmsk, Helper::rand_vec(i, MIN, MAX));
            sk = Filter::keygen(pp5, fmsk, Helper::rand_mat(i, 5, MIN, MAX));
            // Set the start and the end.
            auto start = std::chrono::high_resolution_clock::now();
            bool test = Filter::dec(ct, sk);
            auto end = std::chrono::high_resolution_clock::now();
            // Count the time.
            elapsed += (end - start);
        }
        // Output the time to the file
        fix_col_file << "(" << i << ", " << elapsed.count() / ROUND << ")" << std::endl;
        elapsed = std::chrono::duration<double, std::milli>::zero();
    }

    // For m from 1 to 20, we perform the Filter setup.
    fix_fil_file << "Fle scheme, pp 1:" << std::endl;
    for (int i = 1; i <= M; ++i){
        for (int j = 0; j < ROUND; ++j){
            fmsk = Filter::msk_gen(pp1, 5);
            ct = Filter::enc(pp1, fmsk, Helper::rand_vec(5, MIN, MAX));
            sk = Filter::keygen(pp1, fmsk, Helper::rand_mat(5, 1, MIN, MAX));
            // Set the start and the end.
            auto start = std::chrono::high_resolution_clock::now();
            bool test = Filter::dec(ct, sk);
            auto end = std::chrono::high_resolution_clock::now();
            // Count the time.
            elapsed += (end - start);
        }

        // Output the time to the file
        fix_fil_file << "(" << 5 << ", " << elapsed.count() / ROUND * ((5 + 1) / 2.0) / 5 << ")" << std::endl;
        elapsed = std::chrono::duration<double, std::milli>::zero();
    }

    // For m from 1 to 20, we perform the Filter setup.
    fix_fil_file << "Fle scheme, pp 5:" << std::endl;
    for (int i = 1; i <= M; ++i){
        for (int j = 0; j < ROUND; ++j){
            fmsk = Filter::msk_gen(pp5, 5);
            ct = Filter::enc(pp5, fmsk, Helper::rand_vec(5, MIN, MAX));
            sk = Filter::keygen(pp5, fmsk, Helper::rand_mat(5, 5, MIN, MAX));
            // Set the start and the end.
            auto start = std::chrono::high_resolution_clock::now();
            bool test = Filter::dec( ct, sk);
            auto end = std::chrono::high_resolution_clock::now();
            // Count the time.
            elapsed += (end - start);
        }
        // Output the time to the file
        fix_fil_file << "(" << 5 << ", " << elapsed.count() / ROUND << ")" << std::endl;
        elapsed = std::chrono::duration<double, std::milli>::zero();
    }
}
