#include <benchmark/benchmark.h>
#include <fmt/format.h>
#include "microfmt.hpp"

#include <sstream>
#include <iomanip>
#include <format>

static void OSS(benchmark::State& state) {
  int x = 0;
  int y = 1000;
  for (auto _ : state) {
    std::ostringstream oss;
    oss
      << std::hex
      << "0x"
      << std::setw(8)
      << std::setfill('0')
      << x
      << std::dec
      << std::setfill(' ')
      << ' '
      << y;
    benchmark::DoNotOptimize(std::move(oss).str());
    x++;
    y += 2;
  }
}
// Register the function as a benchmark
BENCHMARK(OSS);

static void Microfmt(benchmark::State& state) {
  int x = 0;
  int y = 1000;
  for (auto _ : state) {
    benchmark::DoNotOptimize(microfmt::format("0x{>8:0h} {8: d}", x, y));
    x++;
    y += 2;
  }
}
BENCHMARK(Microfmt);

template<typename... T> std::string stringf(T... args) {
    int length = std::snprintf(nullptr, 0, args...);
    if(length < 0) {
        throw std::logic_error("invalid arguments to stringf");
    }
    std::string str(length, 0);
    // .data is const char* in c++11, but &str[0] should be legal
    std::snprintf(&str[0], length + 1, args...);
    return str;
}

static void Stringf(benchmark::State& state) {
  int x = 0;
  int y = 1000;
  for (auto _ : state) {
    benchmark::DoNotOptimize(stringf("%0#10x %-8d", x, y));
    x++;
    y += 2;
  }
}
BENCHMARK(Stringf);

static void Stdfmt(benchmark::State& state) {
  int x = 0;
  int y = 1000;
  for (auto _ : state) {
    benchmark::DoNotOptimize(std::format("{:#010x} {:<8}", x, y));
    x++;
    y += 2;
  }
}
BENCHMARK(Stdfmt);

static void Fmt(benchmark::State& state) {
  int x = 0;
  int y = 1000;
  for (auto _ : state) {
    benchmark::DoNotOptimize(fmt::format("{:#010x} {:<8}", x, y));
    x++;
    y += 2;
  }
}
BENCHMARK(Fmt);

BENCHMARK_MAIN();
