#include "exp.hpp"

void bench_our_pairing(const int round){
    // Open the output files.
    std::ofstream file("sim_pairing_time.txt", std::ios_base::app);
    file << "Our Timings" << std::endl;

    // Create the bilinear pairing object.
    const auto bp = BP();

    for (int num_col = 1; num_col <= 20; ++num_col){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Perform round number of Enc.
        for (int i = 0; i < round; ++i){
            // Create a random vector of desired length.
            auto x = Helper::rand_int_vec(num_col * 32 * 2 + 1, 1, std::numeric_limits<int>::max());
            // Create a random vector of desired length.
            auto y = Helper::rand_int_vec(num_col * 32 * 2 + 1, 1, std::numeric_limits<int>::max());

            // Compute ct and sk.
            auto ct = bp.Gp->g1_raise(bp.Zp->from_int(x));
            auto sk = bp.Gp->g2_raise(bp.Zp->from_int(y));

            // Decryption timings.
            auto start = std::chrono::high_resolution_clock::now();
            std::ignore = Group::pair(ct, sk);
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;
        }

        // Output the time.
        file << "(" << num_col << ", " << time.count() / round << ")" << std::endl;
    }

    // Close the BP.
    BP::close();

    // Create some blank spaces.
    file << std::endl << std::endl;
}


void bench_fe_pairing(const int round){
    // Open the output files.
    std::ofstream file("sim_pairing_time.txt", std::ios_base::app);
    file << "FE Timings" << std::endl;

    // Create the bilinear pairing object.
    const auto bp = BP();

    for (int num_col = 1; num_col <= 20; ++num_col){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Perform round number of Enc.
        for (int i = 0; i < round; ++i){
            // Create a random vector of desired length.
            auto x = Helper::rand_int_vec(20 * 32 + 1, 1, std::numeric_limits<int>::max());
            // Create a random vector of desired length.
            auto y = Helper::rand_int_vec(20 * 32 + 1, 1, std::numeric_limits<int>::max());

            // Compute ct and sk.
            auto ct = bp.Gp->g1_raise(bp.Zp->from_int(x));
            auto sk = bp.Gp->g2_raise(bp.Zp->from_int(y));

            // Decryption timings.
            auto start = std::chrono::high_resolution_clock::now();
            std::ignore = Group::pair(ct, sk);
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;
        }

        // Output the time.
        file << "(" << num_col << ", " << time.count() / round << ")" << std::endl;
    }

    // Close the BP.
    BP::close();

    // Create some blank spaces.
    file << std::endl << std::endl;
}


void bench_num_pairing(const int round, const int num_pairing, const int increment){
    // Open the output files.
    std::ofstream file("sim_pairing_time.txt", std::ios_base::app);
    file << "Num Pairing: " << num_pairing << std::endl;

    // Create the bilinear pairing object.
    const auto bp = BP();

    for (int num_col = 1; num_col <= 10; ++num_col){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Perform round number of Enc.
        for (int i = 0; i < round; ++i){
            // Create a random vector of desired length.
            auto x = Helper::rand_int_vec(
                num_pairing + increment * num_col, 1, std::numeric_limits<int>::max());
            // Create a random vector of desired length.
            auto y = Helper::rand_int_vec(
                num_pairing + increment * num_col, 1, std::numeric_limits<int>::max());

            // Compute ct and sk.
            auto ct = bp.Gp->g1_raise(bp.Zp->from_int(x));
            auto sk = bp.Gp->g2_raise(bp.Zp->from_int(y));

            // Decryption timings.
            auto start = std::chrono::high_resolution_clock::now();
            std::ignore = Group::pair(ct, sk);
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;
        }

        // Output the time.
        file << "(" << num_col << ", " << time.count() / round << ")" << std::endl;
    }

    // Close the BP.
    BP::close();

    // Create some blank spaces.
    file << std::endl << std::endl;
}


void bench_sim_pairing(const int round){
    // bench_our_pairing(round);
    // bench_fe_pairing(round);
    bench_num_pairing(round, 463, 0);
    bench_num_pairing(round, 151, 0);
    bench_num_pairing(round, 131, 10);
    bench_num_pairing(round, 24, 64);
    bench_num_pairing(round, 151, 1);
}
