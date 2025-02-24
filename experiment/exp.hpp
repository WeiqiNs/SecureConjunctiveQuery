#pragma once

#include <limits>
#include <chrono>
#include <fstream>
#include "aggre.hpp"
#include "filter.hpp"
#include "ipe_aggre.hpp"
#include "ipe_filter.hpp"
#include "sse_filter.hpp"

/// Benchmark for setup.
void ipe_client_setup_time(int round);
void sse_client_setup_time(int round);
void our_client_setup_time(int round);
void bench_client_setup_time(int round);

/// Benchmark for encryption.
void ipe_client_enc_time(int round);
void sse_client_enc_time(int round);
void our_client_enc_time(int round);
void bench_enc_time(int round);

/// Benchmark for generating queries for filtering multiple rows.
void ipe_client_single_filter_time(int round);
void sse_client_single_filter_time(int round);
void our_client_single_filter_time(int round);
void bench_client_single_filter_time(int round);

/// Benchmark for generating single query while selecting different number of columns.
void ipe_total_single_filter_time(int round);
void sse_total_single_filter_time(int round);
void our_total_single_filter_time(int round);
void bench_total_single_filter_time(int round);

/// Benchmark for performing decryption on filter single value.
void ipe_server_single_filter_time(int round);
void sse_server_single_filter_time(int round);
void our_server_single_filter_time(int round);
void bench_server_single_filter_time(int round);

/// Benchmark for performing decryption on filter multiple value.
void ipe_total_multi_filter_time(int round);
void our_total_multi_filter_time(int round);
void bench_total_multi_filter_time(int round);

/// Benchmark for performing equality test.
void ipe_total_equality_time(int round);
void kim_total_equality_time(int round);
void our_total_equality_time(int round);
void bench_total_equality_time(int round);
