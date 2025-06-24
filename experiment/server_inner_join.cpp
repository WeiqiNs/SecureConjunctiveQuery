#include "exp.hpp"

constexpr size_t TOTAL = static_cast<size_t>(150000) * 1500000;

void bench_server_join_time(const int round){
    // Open the output files.
    std::ofstream file("server_inner_join_time.txt", std::ios_base::app);

    // Read some data.
    StrMat order = Helper::read_tbl("data/orders.tbl", 100);
    StrMat customer = Helper::read_tbl("data/customer.tbl", 100);

    // Create an AES and pairing group object.
    const auto _ = BP();
    const auto aes = AES();

    // Perform encryption (each encryption result is a CharVec).
    std::vector<CharMat> encrypted_order, encrypted_customer;

    for (auto& each_row : order){
        CharMat temp;
        for (auto& each_data : each_row)
            temp.push_back(aes.encrypt(Helper::str_to_char_vec(each_data)));
        encrypted_order.push_back(temp);
    }

    for (auto& each_row : customer){
        CharMat temp;
        for (auto& each_data : each_row)
            temp.push_back(aes.encrypt(Helper::str_to_char_vec(each_data)));
        encrypted_customer.push_back(temp);
    }

    // Output the title for this section.
    file << "Gt equivalence check time" << std::endl;

    // The time to check for equivalence.
    for (const int selectivity : {2, 4, 6, 8, 10}){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        for (int i = 0; i < round; ++i){
            // Perform the desired number of checking Gt.
            for (size_t j = 0; j < selectivity * TOTAL / 100; ++j){
                // Create a random temp value.
                Gt temp;
                gt_rand(temp.value);

                // Check for equivalence.
                auto start = std::chrono::high_resolution_clock::now();
                std::ignore = Group::cmp_gt(temp, temp);
                auto end = std::chrono::high_resolution_clock::now();
                time += end - start;
            }

            // Perform the desired number of checking Gt.
            for (size_t j = 0; j < (100 - selectivity) * TOTAL / 100; ++j){
                // Create a random temp value.
                Gt temp_one, temp_two;
                gt_rand(temp_one.value);
                gt_rand(temp_two.value);

                // Check for equivalence.
                auto start = std::chrono::high_resolution_clock::now();
                std::ignore = Group::cmp_gt(temp_one, temp_two);
                auto end = std::chrono::high_resolution_clock::now();
                time += end - start;
            }
        }

        // Output the time.
        file << "(" << selectivity << ", " << time.count() / round << ")" << std::endl;
    }

    // Output the title for this section.
    file << std::endl << std::endl << "Join time" << std::endl;

    // The time to check for join.
    for (const int selectivity : {2, 4, 6, 8, 10}){
        // Create holder for timings.
        std::chrono::duration<double, std::milli> time{};

        for (int i = 0; i < round; ++i){
            // Perform the desired number of checking Gt.
            for (size_t j = 0; j < selectivity * TOTAL / 100; ++j){
                // Get temp encrypted values.
                auto temp_order = encrypted_order[j % 100];
                auto temp_customer = encrypted_customer[j % 100];
                // Check for equivalence.
                auto start = std::chrono::high_resolution_clock::now();
                temp_order.insert(temp_order.end(), temp_customer.begin() + 1, temp_customer.end());
                auto end = std::chrono::high_resolution_clock::now();
                time += end - start;
            }
        }

        // Output the time.
        file << "(" << selectivity << ", " << time.count() / round << ")" << std::endl;
    }

    // Close the bilinear pairing group.
    BP::close();
}
