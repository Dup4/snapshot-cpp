#include <benchmark/benchmark.h>

#include "snapshot/snapshot.h"

using namespace snapshot;

static void BenchmarkToStringInt(benchmark::State& state) {
    for (auto _ : state) {
        Snapshot::ToString(int(1));
    }
}

BENCHMARK(BenchmarkToStringInt);
