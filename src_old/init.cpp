#include "init.hpp"

void init_get_order(zp& p) {
    // Initiate relic.
    core_init();
    pc_param_set_any();

    // Get order.
    pc_get_ord(p);
}

