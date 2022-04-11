#include <benchmark/benchmark.h>

#include "snapshot-cpp/hello_world.hpp"

static void BenchmarkHelloWorld(benchmark::State& state) {
    for (auto _ : state) {
        HelloWorld();
    }
}

BENCHMARK(BenchmarkHelloWorld);
