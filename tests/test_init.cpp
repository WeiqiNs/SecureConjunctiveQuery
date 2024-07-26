#include "init.hpp"

int test_asym_init() {
    // Get the prime number p, the order of the group.
    zp p;
    init_get_order(p);
    // The group order should be a prime.
    return bn_is_prime(p) == 1;
}


int main() {
    // Perform test.
    if (test_asym_init() != 1) return 1;

    return 0;
}