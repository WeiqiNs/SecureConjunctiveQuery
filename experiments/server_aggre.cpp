#include <chrono>
#include <fstream>
#include "aggre.hpp"

static int ROUND = 3000;
static int MIN = 1;
static int MAX = 10000000;

int main(){
    // Open the output files.
    std::ofstream aggre("server_aggre.txt", std::ios::out | std::ios::trunc);

    // Set the degree to be 1 and 5.
    const auto pp = Aggre::pp_gen();

    const auto msk2 = Aggre::msk_gen(pp, 2);
    const auto msk5 = Aggre::msk_gen(pp, 5);
    const auto msk10 = Aggre::msk_gen(pp, 10);
    const auto msk15 = Aggre::msk_gen(pp, 15);

    constexpr int p = 10;
    g1_vec ct;
    g2_vec sk;

    std::chrono::duration<double, std::milli> elapsed = std::chrono::duration<double, std::milli>::zero();

    // For m from 1 to 20, we perform the Filter setup.
    aggre << "Select 2 Cols:" << std::endl;
    for (int j = 0; j < ROUND; ++j){
        ct = Aggre::enc(pp, msk2, Helper::rand_vec(2, MIN, MAX));
        sk = Aggre::keygen(pp, msk2, Helper::rand_vec(2, MIN, MAX), p);
        // Set the start and the end.
        auto start = std::chrono::high_resolution_clock::now();
        bool test = Aggre::dec(ct, sk);
        auto end = std::chrono::high_resolution_clock::now();
        // Count the time.
        elapsed += (end - start);
    }
    aggre << "(" << 2 << ", " << elapsed.count() << ")" << std::endl;

    elapsed = std::chrono::duration<double, std::milli>::zero();

    aggre << "Select 5 Cols:" << std::endl;
    for (int j = 0; j < ROUND; ++j){
        ct = Aggre::enc(pp, msk5, Helper::rand_vec(5, MIN, MAX));
        sk = Aggre::keygen(pp, msk5, Helper::rand_vec(5, MIN, MAX), p);
        // Set the start and the end.
        auto start = std::chrono::high_resolution_clock::now();
        bool test = Aggre::dec(ct, sk);
        auto end = std::chrono::high_resolution_clock::now();
        // Count the time.
        elapsed += (end - start);
    }
    aggre << "(" << 5 << ", " << elapsed.count() << ")" << std::endl;

    elapsed = std::chrono::duration<double, std::milli>::zero();

    aggre << "Select 10 Cols:" << std::endl;
    for (int j = 0; j < ROUND; ++j){
        ct = Aggre::enc(pp, msk10, Helper::rand_vec(10, MIN, MAX));
        sk = Aggre::keygen(pp, msk10, Helper::rand_vec(10, MIN, MAX), p);
        // Set the start and the end.
        auto start = std::chrono::high_resolution_clock::now();
        bool test = Aggre::dec(ct, sk);
        auto end = std::chrono::high_resolution_clock::now();
        // Count the time.
        elapsed += (end - start);
    }
    aggre << "(" << 10 << ", " << elapsed.count() << ")" << std::endl;

    elapsed = std::chrono::duration<double, std::milli>::zero();

    aggre << "Select 15 Cols:" << std::endl;
    for (int j = 0; j < ROUND; ++j){
        ct = Aggre::enc(pp, msk15, Helper::rand_vec(15, MIN, MAX));
        sk = Aggre::keygen(pp, msk15, Helper::rand_vec(15, MIN, MAX), p);
        // Set the start and the end.
        auto start = std::chrono::high_resolution_clock::now();
        bool test = Aggre::dec(ct, sk);
        auto end = std::chrono::high_resolution_clock::now();
        // Count the time.
        elapsed += (end - start);
    }
    aggre << "(" << 15 << ", " << elapsed.count() << ")" << std::endl;

}
