# Multithreading
This is a C++ take-home assignment for our interview discussion. You are welcome to approach it however you see fit.
We suggest treating your solution as a reflection of your skill and judgment at the level you are applying for.

Using AI tools is discouraged. It is important that you are able to explain what your code does and why.

Using third-party libraries is optional. If you choose to use one, please be ready to justify it.

## Task
Implement `logger` as a multi-producer, single-consumer (MPSC) message queue in [`src/logger.hpp`](src/logger.hpp):

```cpp
#pragma once
#include <format>
#include <stop_token>

extern void (*print)(std::string_view message);

class logger {
public:
  /// Called on many threads. Must queue the message.
  /// Should return as soon as possible and rarely block.
  void post(std::string_view message) {
    // TODO: Queue message.
  }

  /// Called on many threads. Must format and queue the message.
  /// Should return as soon as possible and rarely block.
  template <class... Args>
  void post(std::format_string<Args...> fmt, Args&&... args) {
    post(std::format(fmt, std::forward<Args>(args)...));
  }

  /// Called on one thread. Must call `print` once per message.
  void run(std::stop_token stop) {
    // TODO: Print queued messages.
  }
};
```

## Build
Requirements for building this project:
* [CMake](https://cmake.org/download/) version 3.25 or later
* [Ninja](https://github.com/ninja-build/ninja/releases)
* Compiler with C++20 support.

Requirements for building benchmarks (optional):
* [Vcpkg](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started)

### Windows
On Windows, you can get all dependencies in the Visual Studio Installer:
* Desktop development with C++
  - C++ CMake tools for Windows
  - vcpkg package manager

### Linux
On Linux, you can use CMake directly:

```sh
# Install dependencies.
sudo apt install -y g++ git cmake ninja-build

# Configure project.
cmake --preset default

# Build and run main.
cmake --build --preset debug
build/Debug/main

# Build and run tests.
cmake --build --preset debug --target tests
build/src/tests/Debug/tests
```

You can also use the provided [compose.yaml](compose.yaml) file:

```sh
# Install docker-compose(1).
sudo apt install -y docker-compose

# Build image.
docker compose build

# Configure project.
docker compose run --rm -u "$(id -u):$(id -g)" dev cmake --preset default

# Build and run main.
docker compose run --rm -u "$(id -u):$(id -g)" dev cmake --build --preset debug
docker compose run --rm -u "$(id -u):$(id -g)" dev build/Debug/main

# Build and run tests.
docker compose run --rm -u "$(id -u):$(id -g)" dev cmake --build --preset debug --target tests
docker compose run --rm -u "$(id -u):$(id -g)" dev build/src/tests/Debug/tests

# Build and run benchmarks.
docker compose run --rm -u "$(id -u):$(id -g)" dev cmake --build --preset release --target benchmarks
docker compose run --rm -u "$(id -u):$(id -g)" dev setarch --addr-no-randomize build/src/benchmarks/Release/benchmarks
```

<!--
# Remove everything.
docker compose down --rmi all -v --remove-orphans
docker builder prune --force
-->
