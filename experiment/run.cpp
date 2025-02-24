#include "exp.hpp"

int main(){
    bench_client_setup_time(100);
    bench_enc_time(100);
    bench_client_single_filter_time(100);
    bench_total_single_filter_time(100);
    bench_server_single_filter_time(100);
    bench_total_multi_filter_time(100);
    bench_total_equality_time(100);
    return 0;
}
