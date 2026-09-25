#pragma once
#include <format>
#include <stop_token>
#include <deque>
#include <condition_variable>
#include <string>
#include <string_view>
#include <mutex>
#include <utility>

extern void (*print)(std::string_view message);

class logger {
private:
  std::mutex queue_mutex;
  std::deque<std::string> log_queue;

  /// Conditional variables
  std::condition_variable_any not_empty;
  std::condition_variable not_full;
  /// Size of the queue not over this number.
  static constexpr std::size_t QUEUE_SIZE = 1024;

public:
  /// Called on many threads. Must queue the message.
  /// Should return as soon as possible and rarely block.
  void post(std::string_view message);

  /// Called on many threads. Must format and queue the message.
  /// Should return as soon as possible and rarely block.
  template <class... Args>
  void post(std::format_string<Args...> fmt, Args&&... args) {
    post(std::format(fmt, std::forward<Args>(args)...));
  }

  /// Called on one thread. Must call `print` once per message.
  void run(std::stop_token stop);
};
