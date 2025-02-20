#pragma once

#include <limits>
#include <chrono>
#include <fstream>
#include "filter.hpp"
#include "ipe_filter.hpp"

/// Benchmark for setup.
void ipe_setup_time(int round);
void our_setup_time(int round);
void sse_setup_time(int round);
void bench_setup_time(int round);

/// Benchmark for encryption.
void ipe_enc_time(int round);
void our_enc_time(int round);
void sse_enc_time(int round);
void bench_enc_time(int round);

/// Benchmark for generating queries for filtering multiple rows.
void ipe_mul_row_time(int round);
void our_mul_row_time(int round);
void sse_mul_row_time(int round);
void bench_mul_row_time(int round);

/// Benchmark for generating single query while selecting different number of columns.
void ipe_sel_col_time(int round);
void our_sel_col_time(int round);
void sse_sel_col_time(int round);
void bench_sel_col_time(int round);
