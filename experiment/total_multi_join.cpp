#include "exp.hpp"

/// Note that the testing data can be found at:
/// https://github.com/EquiJoins/EquiJoinsOverEncryptedData/tree/main/hash_based_impl/data

void ipe_total_multi_join_time(const int round){
    // Open the output files.
    std::ofstream file("total_multi_join_time.txt", std::ios_base::app);
    file << "IPE Timings" << std::endl;

    // Read the data.
    StrMat order = Helper::read_tbl("data/orders.tbl", 150000);
    StrMat customer = Helper::read_tbl("data/customer.tbl", 1500000);

    // Create pp and msk.
    auto pp_order = IpeJoin::pp_gen(5, 9);
    auto msk_order = IpeJoin::msk_gen(pp_order);
    auto pp_customer = IpeJoin::pp_gen(5, 8);
    auto msk_customer = IpeJoin::msk_gen(pp_customer);

    for (int num_col = 2; num_col <= 15; ++num_col){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Compute round number of total time.
        for (int i = 0; i < round; ++i){
            // Create a random vector of desired length.
            auto y_order = Helper::rand_int_mat(8, 5, 1, std::numeric_limits<int>::max());
            auto y_customer = Helper::rand_int_mat(7, 5, 1, std::numeric_limits<int>::max());

            // Total timings.
            auto start = std::chrono::high_resolution_clock::now();
            auto sk_order = IpeJoin::keygen(pp_order, msk_order, y_order);
            auto sk_customer = IpeJoin::keygen(pp_customer, msk_customer, y_customer);
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;

            // Timing for order.
            for (auto& order_row: order){
                auto ct_order = IpeJoin::enc(pp_order, msk_order, order_row, 1);
                // Total timings.
                start = std::chrono::high_resolution_clock::now();
                std::ignore = IpeJoin::dec(ct_order, sk_order);
                end = std::chrono::high_resolution_clock::now();
                time += end - start;
            }

            // Timing for customer.
            for (auto& customer_row: customer){
                auto ct_customer = IpeJoin::enc(pp_order, msk_order, customer_row, 1);
                // Total timings.
                start = std::chrono::high_resolution_clock::now();
                std::ignore = IpeJoin::dec(ct_customer, sk_customer);
                end = std::chrono::high_resolution_clock::now();
                time += end - start;
            }
        }

        // Output the time.
        file << "(" << num_col << ", " << time.count() / round << ")" << std::endl;
    }

    // Close the BP.
    BP::close();

    // Create some blank spaces.
    file << std::endl << std::endl;
}

void our_total_multi_join_time(const int round){
    // Open the output files.
    std::ofstream file("total_multi_join_time.txt", std::ios_base::app);
    file << "Our Timings" << std::endl;

    // Read the data.
    StrMat order = Helper::read_tbl("data/orders.tbl", 150000);
    StrMat customer = Helper::read_tbl("data/customer.tbl", 1500000);

    // Create pp and msk.
    auto pp_order = Join::pp_gen(5, 9);
    auto msk_order = Join::msk_gen(pp_order);
    auto pp_customer = Join::pp_gen(5, 8);
    auto msk_customer = Join::msk_gen(pp_customer);

    for (int num_col = 2; num_col <= 15; ++num_col){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        // Compute round number of total time.
        for (int i = 0; i < round; ++i){
            // Create sel based on num_col.
            IntVec sel_order, sel_customer;
            IntMat y_order, y_customer;

            // Depends on the number of columns, make selection.
            if (num_col <= 8){
                // Create the secret keys.
                y_order = Helper::rand_int_mat(num_col - 1, 5, 1, std::numeric_limits<int>::max());
                y_customer = Helper::rand_int_mat(1, 5, 1, std::numeric_limits<int>::max());
                // Create the selection vector.
                for (int k = 0; k < num_col - 1; ++k){ sel_order.push_back(k); }
                sel_customer.push_back(0);
            }
            else{
                // Create the secret keys.
                y_order = Helper::rand_int_mat(8, 5, 1, std::numeric_limits<int>::max());
                y_customer = Helper::rand_int_mat(num_col - 8, 5, 1, std::numeric_limits<int>::max());
                // Create the selection vector.
                for (int k = 0; k < 8; ++k){ sel_order.push_back(k); }
                for (int k = 0; k < num_col - 8; ++k){ sel_customer.push_back(k); }
            }

            // Total timings.
            auto start = std::chrono::high_resolution_clock::now();
            auto sk_order = Join::keygen(pp_order, msk_order, y_order, sel_order);
            auto sk_customer = Join::keygen(pp_customer, msk_customer, y_customer, sel_customer);
            auto end = std::chrono::high_resolution_clock::now();
            time += end - start;

            // Timing for order.
            for (auto& order_row: order){
                auto ct_order = Join::enc(pp_order, msk_order, order_row, 1);
                // Total timings.
                start = std::chrono::high_resolution_clock::now();
                std::ignore = Join::dec(pp_order, ct_order, sk_order, sel_order);
                end = std::chrono::high_resolution_clock::now();
                time += end - start;
            }

            // Timing for customer.
            for (auto& customer_row: customer){
                auto ct_customer = Join::enc(pp_customer, msk_customer, customer_row, 1);
                // Total timings.
                start = std::chrono::high_resolution_clock::now();
                std::ignore = Join::dec(pp_customer, ct_customer, sk_customer, sel_customer);
                end = std::chrono::high_resolution_clock::now();
                time += end - start;
            }
        }

        // Output the time.
        file << "(" << num_col << ", " << time.count() / round << ")" << std::endl;
    }

    // Close the BP.
    BP::close();

    // Create some blank spaces.
    file << std::endl << std::endl;
}

void bench_total_multi_join_time(const int round){
    ipe_total_multi_join_time(round);
    our_total_multi_join_time(round);
}
