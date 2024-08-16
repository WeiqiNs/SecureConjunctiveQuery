// apply selection first: skip decryption of row if row doesn't satisfy where condition

#include <chrono>
#include <fstream>
#include <string>
#include "group.hpp"

int main(){
    zp p;
    init_get_order(p);

    std::string sum;
    std::string temp = "abcdabcdabcdabcdabcd";
    gt_t one, two;
    gt_null(one);
    gt_null(two);
    gt_new(one);
    gt_new(two);
    gt_rand(one);
    gt_rand(two);

    std::ofstream join("join.txt", std::ios::out | std::ios::trunc);

    for (int s = 1; s <= 10; ++s){
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 15000 * s; ++i){
            for (int j = 0; j < 150 * s; ++j){
                gt_cmp(one, two);
                sum = temp + temp;
            }
            for (int j = 0; j < 1350 * s; ++j){
                gt_cmp(one, two);
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        join << "(" << s / 100.0 << ", " << elapsed.count() << ")" << std::endl;
    }
}
