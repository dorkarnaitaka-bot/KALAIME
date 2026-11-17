#include <benchmark/benchmark.h>

#include "kalaime/core/project.hpp"

static void BenchmarkInfrastructureAbiVersion(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(kalaime::project_abi_version());
    }
}

BENCHMARK(BenchmarkInfrastructureAbiVersion);
BENCHMARK_MAIN();
