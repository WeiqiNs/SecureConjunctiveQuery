#pragma once

#include <limits>
#include <chrono>
#include <fstream>
#include "join.hpp"
#include "aggre.hpp"
#include "filter.hpp"
#include "ipe_join.hpp"
#include "ipe_aggre.hpp"
#include "kim_aggre.hpp"
#include "ipe_filter.hpp"
#include "sse_filter.hpp"

/// Benchmark for client setup time.
void ipe_client_setup_time(int round);
void sse_client_setup_time(int round);
void our_client_setup_time(int round);
void bench_client_setup_time(int round);

/// Benchmark for client encryption time.
void ipe_client_enc_time(int round);
void sse_client_enc_time(int round);
void our_client_enc_time(int round);
void bench_enc_time(int round);

/// Benchmark for client query generation time (2^20 rows).
void ipe_client_single_filter_time(int round);
void sse_client_single_filter_time(int round);
void our_client_single_filter_time(int round);
void bench_client_single_filter_time(int round);

/// Benchmark for performing single decryption on filter single value.
void ipe_server_single_filter_time(int round);
void sse_server_single_filter_time(int round);
void our_server_single_filter_time(int round);
void bench_server_single_filter_time(int round);

/// Benchmark for total time of single value filter.
void ipe_total_single_filter_time(int round);
void sse_total_single_filter_time(int round);
void our_total_single_filter_time(int round);
void bench_total_single_filter_time(int round);

/// Benchmark for total time of multiple value filter.
void ipe_total_multi_filter_time(int round);
void our_total_multi_filter_time(int round);
void bench_total_multi_filter_time(int round);

/// Benchmark for total time of equality test.
void ipe_total_equality_time(int round);
void kim_total_equality_time(int round);
void our_total_equality_time(int round);
void bench_total_equality_time(int round);

/// Benchmark for total time of join and filter single value.
void ipe_total_single_join_time(int round);
void our_total_single_join_time(int round);
void bench_total_single_join_time(int round);

/// Benchmark for total time of join and filter multiple value.
void ipe_total_multi_join_time(int round);
void our_total_multi_join_time(int round);
void bench_total_multi_join_time(int round);