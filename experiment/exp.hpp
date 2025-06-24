#pragma once

#include <limits>
#include <chrono>
#include <fstream>
#include "flexdb_sf.hpp"
#include "flexdb_et.hpp"
#include "flexdb_mf.hpp"
#include "flexdb_join.hpp"
#include "ipe_ej.hpp"
#include "ipe_et.hpp"
#include "ipe_filter.hpp"

/// Benchmark for client timings.
void bench_client_setup_time(int round);
void bench_client_enc_time(int round);
void bench_client_keygen_time(int round);

/// Benchmark for server timings.
void bench_server_sf_time(int round);
void bench_server_mf_time(int round);
void bench_server_et_time(int round);

/// Benchmark for total timings.
void bench_total_sf_time(int round);
void bench_total_mf_time(int round);
void bench_total_et_time(int round);
void bench_total_mf_join_time(int round);

/// Benchmark for performing the final join step.
void bench_server_join_time(int round);

/// Benchmark for simulated number of pairings.
void bench_sim_pairing(int round);
