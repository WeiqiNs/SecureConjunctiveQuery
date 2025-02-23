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

/// Benchmark for performing decryption on filter single value.
void ipe_single_dec_time(int round);
void our_single_dec_time(int round);
void sse_single_dec_time(int round);
void bench_single_dec_time(int round);

/// Benchmark for performing decryption on filter multiple value.
void ipe_multi_dec_time(int round);
void our_multi_dec_time(int round);
void bench_multi_dec_time(int round);

/// Benchmark for performing equality test.
void ipe_equality_time(int round);
void our_equality_time(int round);
void kim_equality_time(int round);
void bench_equality_time(int round);