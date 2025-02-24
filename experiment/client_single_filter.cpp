#include "exp.hpp"

void ipe_client_single_filter_time(const int round){
    // Open the output files.
    std::ofstream time_file("client_single_filter_time.txt", std::ios_base::app);
    time_file << "IPE Timings" << std::endl;

    std::ofstream storage_file("client_single_filter_storage.txt", std::ios_base::app);
    storage_file << "IPE Storage" << std::endl;

    // Create pp and msk.
    auto pp = IpeFilter::pp_gen(1, 20);
    auto msk = IpeFilter::msk_gen(pp);

    for (int num_row = 1; num_row <= 20; ++num_row){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};
        G2Vec sk;

        // Perform round number of Enc.
        for (int i = 0; i < round; ++i){
            // Create a random vector of desired length.
            auto y = Helper::rand_int_mat(20, 1, 1, std::numeric_limits<int>::max());

            // Keygen timings.
            auto start = std::chrono::high_resolution_clock::now();
            sk = IpeFilter::keygen(pp, msk, y);
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;
        }

        // Compute the sk size.
        unsigned long sk_size = sk.size() * sizeof(uint8_t) * pp.pairing_group->Gp->g2_size;

        // Output the time.
        time_file << "(" << num_row << ", " << time.count() / round << ")" << std::endl;

        // Output the storage.
        storage_file << "(" << num_row << ", " << sk_size << ")" << std::endl;

        // Close the BP.
        BP::close();
    }

    // Create some blank spaces.
    time_file << std::endl << std::endl;
    storage_file << std::endl << std::endl;
}

void sse_client_single_filter_time(const int round){
    // Open the output files.
    std::ofstream time_file("client_single_filter_time.txt", std::ios_base::app);
    time_file << "SSE Timings" << std::endl;

    std::ofstream storage_file("client_single_filter_storage.txt", std::ios_base::app);
    storage_file << "SSE Storage" << std::endl;

    // Create pp and msk.
    auto msk = SseFilter::msk_gen();
    CharMat sk;

    for (int num_row = 1; num_row <= 20; ++num_row){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Perform ROUND number of setup.
        for (int i = 0; i < round; ++i){
            // Create a random vector of desired length.
            auto y = Helper::rand_int_vec(20, 1, std::numeric_limits<int>::max());

            // Keygen timings.
            auto start = std::chrono::high_resolution_clock::now();
            sk = SseFilter::keygen(msk, y, static_cast<int>(std::pow(2, num_row)));
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;
        }

        // Also create holder for size.
        unsigned long sk_size = sk.size() * sk[0].size() * sizeof(unsigned char);

        // Output the time.
        time_file << "(" << num_row << ", " << time.count() / round << ")" << std::endl;

        // Output the storage.
        storage_file << "(" << num_row << ", " << sk_size << ")" << std::endl;
    }

    // Create some blank spaces.
    time_file << std::endl << std::endl;
    storage_file << std::endl << std::endl;
}

void our_client_single_filter_time(const int round){
    // Open the output files.
    std::ofstream time_file("client_single_filter_time.txt", std::ios_base::app);
    time_file << "Our Timings" << std::endl;

    std::ofstream storage_file("client_single_filter_storage.txt", std::ios_base::app);
    storage_file << "Our Storage" << std::endl;

    // Create pp and msk.
    auto pp = Filter::pp_gen(1, 20);
    auto msk = Filter::msk_gen(pp);

    for (int num_row = 1; num_row <= 20; ++num_row){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};
        G2Vec sk;

        // Perform round number of Enc.
        for (int i = 0; i < round; ++i){
            // Create a random vector of desired length.
            auto y = Helper::rand_int_vec(20, 1, std::numeric_limits<int>::max());

            // Keygen timings.
            auto start = std::chrono::high_resolution_clock::now();
            sk = Filter::keygen(pp, msk, y);
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;
        }

        // Compute the secret key size.
        unsigned long sk_size = sk.size() * sizeof(uint8_t) * pp.pairing_group->Gp->g2_size;

        // Output the time.
        time_file << "(" << num_row << ", " << time.count() / round << ")" << std::endl;

        // Output the storage.
        storage_file << "(" << num_row << ", " << sk_size << ")" << std::endl;

        // Close the BP.
        BP::close();
    }

    // Create some blank spaces.
    time_file << std::endl << std::endl;
    storage_file << std::endl << std::endl;
}

void bench_client_single_filter_time(const int round){
    ipe_client_single_filter_time(round);
    sse_client_single_filter_time(round);
    our_client_single_filter_time(round);
}
