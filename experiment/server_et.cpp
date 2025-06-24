#include "exp.hpp"

void ipe_server_et_time(const int round){
    // Open the output files.
    std::ofstream file("server_et_time.txt", std::ios_base::app);
    file << "IPE Timings" << std::endl;

    // Create pp and msk.
    auto pp = IpeEt::pp_gen(20);
    auto msk = IpeEt::msk_gen(pp);

    for (int num_col = 1; num_col <= 20; ++num_col){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Perform round number of Enc.
        for (int i = 0; i < round; ++i){
            // Create a random vector of desired length.
            auto x = Helper::rand_int_vec(20, 1, std::numeric_limits<int>::max());
            auto y = Helper::rand_int_vec(20, 1, std::numeric_limits<int>::max());
            // Set the unselected portion to zero.
            for (int j = num_col + 1; j < 20; ++j){ y[j] = 0; }

            // Compute ct and sk.
            auto ct = IpeEt::enc(pp, msk, x);
            auto sk = IpeEt::keygen(pp, msk, y, 100);

            // Decryption timings.
            auto start = std::chrono::high_resolution_clock::now();
            std::ignore = IpeFilter::dec(ct, sk);
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

void our_server_et_time(const int round){
    // Open the output files.
    std::ofstream file("server_et_time.txt", std::ios_base::app);
    file << "Our Timings" << std::endl;


    // Create pp and msk.
    auto pp = FlexDbEt::pp_gen(20);
    auto msk = FlexDbEt::msk_gen(pp);

    for (int num_col = 1; num_col <= 20; ++num_col){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Perform round number of Enc.
        for (int i = 0; i < round; ++i){
            // Create a random vector of desired length.
            auto x = Helper::rand_int_vec(20, 1, std::numeric_limits<int>::max());
            // Create a random vector of desired length.
            auto y = Helper::rand_int_vec(num_col, 1, std::numeric_limits<int>::max());

            // Set the unselected portion to zero.
            IntVec sel;
            for (int j = 0; j < num_col; ++j){ sel.push_back(j); }

            // Compute ct and sk.
            auto ct = FlexDbEt::enc(pp, msk, x);
            auto sk = FlexDbEt::keygen(pp, msk, y, 100, sel);

            // Decryption timings.
            auto start = std::chrono::high_resolution_clock::now();
            std::ignore = FlexDbEt::dec(ct, sk, sel);
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

void bench_server_et_time(const int round){
    ipe_server_et_time(round);
    our_server_et_time(round);
}
