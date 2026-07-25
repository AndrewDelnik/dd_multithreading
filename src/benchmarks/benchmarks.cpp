#include "benchmarks.hpp"
#include <logger.hpp>
#include <iostream>

void (*print)(std::string_view message) = [](std::string_view message) {};
std::span<const std::string_view> benchmark_messages;

BENCHMARK(string<logger>)->Threads(32)->Iterations(benchmark_iterations);
BENCHMARK(format<logger>)->Threads(32)->Iterations(benchmark_iterations);

int main(int argc, char** argv) {
  benchmark::Initialize(&argc, argv);
  if (benchmark::ReportUnrecognizedArguments(argc, argv)) {
    return EXIT_FAILURE;
  }
  constexpr std::size_t benchmark_messages_size{ 1'024 };
  std::string benchmark_messages_text;
  const auto messages = std::invoke([&] {
    std::vector<std::size_t> sizes;
    sizes.resize(benchmark_messages_size);
    auto it = std::back_inserter(benchmark_messages_text);
    for (std::size_t i = 0; i < benchmark_messages_size; i++) {
      const auto start = benchmark_messages_text.size();
      std::format_to(it, "{} string test message", i);
      sizes[i] = benchmark_messages_text.size() - start;
    }
    std::vector<std::string_view> messages;
    messages.resize(benchmark_messages_size);
    auto data = benchmark_messages_text.data();
    for (std::size_t i = 0; i < benchmark_messages_size; i++) {
      messages[i] = std::string_view{ data, sizes[i] };
      data += sizes[i];
    }
    return messages;
  });
  benchmark_messages = messages;
  benchmark::RunSpecifiedBenchmarks();
  benchmark::Shutdown();
}
