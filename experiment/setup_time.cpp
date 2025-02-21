#include "exp.hpp"

void ipe_setup_time(const int round){
    // Open the output files.
    std::ofstream file("setup_time.txt", std::ios_base::app);
    file << "IPE Setup Timings" << std::endl;

    for (int length = 10; length <= 200; length += 10){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Perform ROUND number of setup.
        for (int i = 0; i < round; ++i){
            // Setup timings.
            auto start = std::chrono::high_resolution_clock::now();
            // Create pp and msk.
            auto pp = IpeFilter::pp_gen(1, length);
            auto msk = IpeFilter::msk_gen(pp);
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

void our_setup_time(const int round){
    // Open the output files.
    std::ofstream file("setup_time.txt", std::ios_base::app);
    file << "Our Setup Timings" << std::endl;

    for (int length = 10; length <= 200; length += 10){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Perform ROUND number of setup.
        for (int i = 0; i < round; ++i){
            // Setup timings.
            auto start = std::chrono::high_resolution_clock::now();
            // Create pp and msk.
            auto pp = Filter::pp_gen(1, length);
            auto msk = Filter::msk_gen(pp);
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

void sse_setup_time(const int round){
    // Open the output files.
    std::ofstream file("setup_time.txt", std::ios_base::app);
    file << "SSE Setup Timings" << std::endl;

    for (int length = 10; length <= 200; length += 10){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Perform ROUND number of setup.
        for (int i = 0; i < round; ++i){
            // Setup timings.
            auto start = std::chrono::high_resolution_clock::now();
            // Create the PRF object.
            auto msk = SseFilter::msk_gen();
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;
        }

        // Output the time.
        file << "(" << length << ", " << time.count() / round << ")" << std::endl;
    }
    // Create some blank spaces.
    file << std::endl << std::endl;
}

void bench_setup_time(const int round){
    ipe_setup_time(round);
    our_setup_time(round);
    sse_setup_time(round);
}
