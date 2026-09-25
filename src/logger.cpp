// This file exists for your convenience. It is not necessary to use it.
// Do not define the global `void (*print)(std::string_view message)` symbol here, as it would break all tests.
#include "logger.hpp"



/// Called on many threads. Must queue the message.
/// Should return as soon as possible and rarely block.
void logger::post(std::string_view message) {
  std::unique_lock<std::mutex> lock(queue_mutex);
  not_full.wait(lock, [&]{ return log_queue.size() < QUEUE_SIZE;});

  log_queue.push_back(std::string(message));

  lock.unlock();
  not_empty.notify_one();
}

/// Called on one thread. Must call `print` once per message.
void logger::run(std::stop_token stop) {
  std::unique_lock<std::mutex> lock(queue_mutex, std::defer_lock);
  std::string tmp;

  while (true) {
    lock.lock();
    if (!not_empty.wait(lock, stop, [&]{ return !log_queue.empty();})) {break;}
    tmp = std::move(log_queue.front());
    log_queue.pop_front();

    lock.unlock();
    not_full.notify_one();
    print(tmp);
  }
}