#pragma once
#include <format>
#include <stop_token>

extern void (*print)(std::string_view message);

class logger {
public:
  /// Called on many threads. Must queue the message.
  /// Should return as soon as possible and rarely block.
  void post(std::string_view message) {
    print(message);  // TODO: Queue message instead.
  }

  /// Called on many threads. Must format and queue the message.
  /// Should return as soon as possible and rarely block.
  template <class... Args>
  void post(std::format_string<Args...> fmt, Args&&... args) {
    post(std::format(fmt, std::forward<Args>(args)...));
  }

  /// Called on one thread. Must call `print` once per message.
  void run(std::stop_token stop) {
    while (!stop.stop_requested()) {}  // TODO: Print queued messages instead.
  }
};
