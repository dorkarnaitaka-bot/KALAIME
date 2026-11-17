#include <benchmark/benchmark.h>

#include "kalaime/core/project.hpp"
#include "kalaime/layouts/phonetic/kala_phonetic_layout.hpp"

static void BenchmarkInfrastructureAbiVersion(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(kalaime::project_abi_version());
    }
}

BENCHMARK(BenchmarkInfrastructureAbiVersion);

static void BenchmarkPhoneticTransliteration(benchmark::State& state) {
    const kalaime::layouts::KalaPhoneticLayout layout;
    for (auto _ : state) {
        auto output = layout.transliterate("sôngkhya");
        benchmark::DoNotOptimize(output);
    }
}

static void BenchmarkPhoneticCandidateGeneration(benchmark::State& state) {
    const kalaime::layouts::KalaPhoneticLayout layout;
    for (auto _ : state) {
        auto candidates = layout.generate_candidates("ch sh");
        benchmark::DoNotOptimize(candidates);
    }
}

BENCHMARK(BenchmarkPhoneticTransliteration);
BENCHMARK(BenchmarkPhoneticCandidateGeneration);
BENCHMARK_MAIN();
