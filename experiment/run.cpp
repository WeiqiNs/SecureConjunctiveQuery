#include "exp.hpp"

int main(){
    bench_setup_time(1000);
    bench_enc_time(1000);
    bench_mul_row_time(1000);
    bench_sel_col_time(1000);
    bench_single_dec_time(1000);
    bench_multi_dec_time(1000);
    bench_equality_time(1000);
    return 0;
}
