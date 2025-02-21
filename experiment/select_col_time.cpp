#include "exp.hpp"

void ipe_sel_col_time(const int round){
    // Open the output files.
    std::ofstream file("sel_col_time.txt", std::ios_base::app);
    file << "IPE Sel Col Timings" << std::endl;

    for (int num_col = 1; num_col <= 20; ++num_col){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Create pp and msk.
        auto pp = IpeFilter::pp_gen(1, 20);
        auto msk = IpeFilter::msk_gen(pp);

        // Perform round number of Enc.
        for (int i = 0; i < round; ++i){
            // Create a random vector of desired length.
            auto y = Helper::rand_int_mat(20, 1, 1, std::numeric_limits<int>::max());

            // Set the unselected portion to zero.
            for (int j = num_col + 1; j < 20; ++j){ y[num_col][0] = 0; }

            // Keygen timings.
            auto start = std::chrono::high_resolution_clock::now();
            auto sk = IpeFilter::keygen(pp, msk, y);
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;
        }

        // Output the time.
        file << "(" << num_col << ", " << time.count() / round << ")" << std::endl;

        // Close the BP.
        BP::close();
    }
    // Create some blank spaces.
    file << std::endl << std::endl;
}

void our_sel_col_time(const int round){
    // Open the output files.
    std::ofstream file("sel_col_time.txt", std::ios_base::app);
    file << "Our Sel Col Timings" << std::endl;

    for (int num_col = 1; num_col <= 20; ++num_col){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Create pp and msk.
        auto pp = Filter::pp_gen(1, 20);
        auto msk = Filter::msk_gen(pp);

        // Perform round number of Enc.
        for (int i = 0; i < round; ++i){
            // Create a random vector of desired length.
            auto y = Helper::rand_int_vec(num_col, 1, std::numeric_limits<int>::max());

            // Set the unselected portion to zero.
            IntVec sel;
            for (int j = 0; j < num_col; ++j){ sel.push_back(j); }

            // Keygen timings.
            auto start = std::chrono::high_resolution_clock::now();
            auto sk = Filter::keygen(pp, msk, y, sel);
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;
        }

        // Output the time.
        file << "(" << num_col << ", " << time.count() / round << ")" << std::endl;

        // Close the BP.
        BP::close();
    }
    // Create some blank spaces.
    file << std::endl << std::endl;
}

void sse_sel_col_time(const int round){
    // Open the output files.
    std::ofstream file("sel_col_time.txt", std::ios_base::app);
    file << "SSE Sel Col Timings" << std::endl;

    for (int num_col = 1; num_col <= 20; ++num_col){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Create pp and msk.
        auto msk = SseFilter::msk_gen();

        // Perform round number of Enc.
        for (int i = 0; i < round; ++i){
            // Create a random vector of desired length.
            auto y = Helper::rand_int_vec(num_col, 1, std::numeric_limits<int>::max());

            // Keygen timings.
            auto start = std::chrono::high_resolution_clock::now();
            auto sk = SseFilter::keygen(msk, y);
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;
        }

        // Output the time.
        file << "(" << num_col << ", " << time.count() / round << ")" << std::endl;
    }
    // Create some blank spaces.
    file << std::endl << std::endl;
}

void bench_sel_col_time(const int round){
    ipe_sel_col_time(round);
    our_sel_col_time(round);
    sse_sel_col_time(round);
}
