#include "exp.hpp"

void ipe_client_setup_time(const int round){
    // Open the output files.
    std::ofstream file("client_setup_time.txt", std::ios_base::app);
    file << "IPE Timings" << std::endl;

    for (int length = 10; length <= 200; length += 10){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Perform ROUND number of setup.
        for (int i = 0; i < round; ++i){
            // Setup timings.
            auto start = std::chrono::high_resolution_clock::now();
            // Create pp and msk.
            auto pp = IpeFilter::pp_gen(5, length);
            std::ignore = IpeFilter::msk_gen(pp);
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;
            // Close the pairing group.
            BP::close();
        }

        // Output the time.
        file << "(" << length << ", " << time.count() / round << ")" << std::endl;
    }
    // Create some blank spaces.
    file << std::endl << std::endl;
}

void our_client_setup_time(const int round){
    // Open the output files.
    std::ofstream file("client_setup_time.txt", std::ios_base::app);
    file << "Our Timings" << std::endl;

    for (int length = 10; length <= 200; length += 10){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Perform ROUND number of setup.
        for (int i = 0; i < round; ++i){
            // Setup timings.
            auto start = std::chrono::high_resolution_clock::now();
            // Create pp and msk.
            auto pp = FlexDbMf::pp_gen(5, length);
            std::ignore = FlexDbMf::msk_gen(pp);
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;
            // Close the pairing group.
            BP::close();
        }

        // Output the time.
        file << "(" << length << ", " << time.count() / round << ")" << std::endl;
    }
    // Create some blank spaces.
    file << std::endl << std::endl;
}

void bench_client_setup_time(const int round){
    ipe_client_setup_time(round);
    our_client_setup_time(round);
}
