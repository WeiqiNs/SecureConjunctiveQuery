#include "exp.hpp"

int main(){
    /// Benchmark for client timings.
    bench_client_setup_time(100);
    bench_client_enc_time(100);
    bench_client_keygen_time(100);

    /// Benchmark for server timings.
    bench_server_sf_time(100);
    bench_server_mf_time(100);
    bench_server_et_time(100);

    /// Benchmark for total timings.
    bench_total_sf_time(100);
    bench_total_mf_time(100);
    bench_total_et_time(100);
    bench_total_mf_join_time(100);

    /// Benchmark for performing the final join step.
    bench_server_join_time(100);

    /// Benchmark for simulated number of pairings.
    bench_sim_pairing(100);

    return 0;
}
