#pragma once

#include <gmp.h>
#include "relic/relic.h"

// We use a more understandable name for integers in the field.
using zp = bn_t;

/**
 *
 * @param p
 */
void init_get_order(zp& p);