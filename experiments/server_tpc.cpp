#include <chrono>
#include <fstream>
#include "join.hpp"
#include "ipfe_filter.hpp"

static int ROUND = 100;
static int MIN = 1;
static int MAX = 10000000;

int main(){
    // Open the output files.
    std::ofstream total_filter("tpc_total_time.txt", std::ios::out | std::ios::trunc);

    // Set the degree to be 1 and 5.
    const auto pp1 = Filter::pp_gen(1);
    const auto pp5 = Filter::pp_gen(5);
    join_msk jmsk1, jmsk2;
    ipfe_filter_msk imsk1, imsk2;
    g1_vec ct;
    g2_vec sk;
    gt temp;
    std::chrono::duration<double, std::milli> elapsed = std::chrono::duration<double, std::milli>::zero();

    // For m from 1 to 20, we perform the Filter setup.
    total_filter << "Fix scheme, pp 1:" << std::endl;
    imsk1 = IpfeFilter::msk_gen(pp1, 9);
    imsk2 = IpfeFilter::msk_gen(pp1, 10);
    auto sk1 = IpfeFilter::keygen(pp1, imsk1, Helper::rand_mat(9, 1, MIN, MAX));
    auto sk2 = IpfeFilter::keygen(pp1, imsk2, Helper::rand_mat(10, 1, MIN, MAX));

    for (int j = 0; j < ROUND; ++j){
        auto ct1 = IpfeFilter::enc(pp1, imsk1, Helper::rand_vec(9, MIN, MAX));
        auto ct2 = IpfeFilter::enc(pp1, imsk2, Helper::rand_vec(10, MIN, MAX));

        // Set the start and the end.
        auto start = std::chrono::high_resolution_clock::now();
        bool test = IpfeFilter::dec(ct1, sk1);
        auto end = std::chrono::high_resolution_clock::now();
        // Count the time.
        elapsed += (end - start) * 10;

        start = std::chrono::high_resolution_clock::now();
        test = IpfeFilter::dec(ct2, sk2);
        end = std::chrono::high_resolution_clock::now();
        // Count the time.
        elapsed += (end - start);
    }
    // Output the time to the file
    total_filter << "(" << 100 << ", " << elapsed.count() << ")" << std::endl;
    elapsed = std::chrono::duration<double, std::milli>::zero();


    // For m from 1 to 20, we perform the Filter setup.
    total_filter << "Fix scheme, pp 5:" << std::endl;
    imsk1 = IpfeFilter::msk_gen(pp5, 9);
    imsk2 = IpfeFilter::msk_gen(pp5, 10);
    sk1 = IpfeFilter::keygen(pp5, imsk1, Helper::rand_mat(9, 5, MIN, MAX));
    sk2 = IpfeFilter::keygen(pp5, imsk2, Helper::rand_mat(10, 5, MIN, MAX));

    for (int j = 0; j < ROUND; ++j){
        auto ct1 = IpfeFilter::enc(pp5, imsk1, Helper::rand_vec(9, MIN, MAX));
        auto ct2 = IpfeFilter::enc(pp5, imsk2, Helper::rand_vec(10, MIN, MAX));

        // Set the start and the end.
        auto start = std::chrono::high_resolution_clock::now();
        bool test = IpfeFilter::dec(ct1, sk1);
        auto end = std::chrono::high_resolution_clock::now();
        // Count the time.
        elapsed += (end - start) * 10;

        start = std::chrono::high_resolution_clock::now();
        test = IpfeFilter::dec(ct2, sk2);
        end = std::chrono::high_resolution_clock::now();
        // Count the time.
        elapsed += (end - start);
    }
    // Output the time to the file
    total_filter << "(" << 100 << ", " << elapsed.count() << ")" << std::endl;
    elapsed = std::chrono::duration<double, std::milli>::zero();


    // For m from 1 to 20, we perform the Filter setup.
    total_filter << "Fle scheme, pp 1:" << std::endl;
    jmsk1 = Join::msk_gen(pp1, 2);
    sk1 = Join::keygen(pp1, jmsk1, Helper::rand_mat(1, 1, MIN, MAX));

    for (int j = 0; j < ROUND; ++j){
        auto ct1 = Join::enc(pp1, jmsk1, Helper::rand_vec(2, MIN, MAX));
        auto ct2 = Join::enc(pp1, jmsk1, Helper::rand_vec(2, MIN, MAX));

        // Set the start and the end.
        auto start = std::chrono::high_resolution_clock::now();
        Join::dec(temp, ct1, sk1);
        auto end = std::chrono::high_resolution_clock::now();
        // Count the time.
        elapsed += (end - start) * 11;
    }
    // Output the time to the file
    total_filter << "(" << 100 << ", " << elapsed.count() << ")" << std::endl;
    elapsed = std::chrono::duration<double, std::milli>::zero();


    // For m from 1 to 20, we perform the Filter setup.
    total_filter << "Fle scheme, pp 5:" << std::endl;
    jmsk1 = Join::msk_gen(pp5, 2);
    sk1 = Join::keygen(pp5, jmsk1, Helper::rand_mat(1, 5, MIN, MAX));

    for (int j = 0; j < ROUND; ++j){
        auto ct1 = Join::enc(pp5, jmsk1, Helper::rand_vec(2, MIN, MAX));
        auto ct2 = Join::enc(pp5, jmsk1, Helper::rand_vec(2, MIN, MAX));

        // Set the start and the end.
        auto start = std::chrono::high_resolution_clock::now();
        Join::dec(temp, ct1, sk1);
        auto end = std::chrono::high_resolution_clock::now();
        // Count the time.
        elapsed += (end - start) * 11;
    }
    // Output the time to the file
    total_filter << "(" << 100 << ", " << elapsed.count() << ")" << std::endl;
}
