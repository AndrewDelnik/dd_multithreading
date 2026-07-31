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

The `benchmarks` and `tests` targets uphold the following, which you may rely on:
1. A message never exceeds 256 characters, including formatted output.
2. A stop is requested only after all producer threads have joined.
3. The consumer thread is joined before the logger is destroyed.
4. At most one logger object exists at any time.

Feel free to experiment with your implementation in [`src/main.cpp`](src/main.cpp).

<details>
<summary><b>Advanced Configuration</b></summary>

The tests and benchmarks harness supports the following traits:

#### Divisor
Divides the per-thread message count by this value, for loggers too slow to run the full count.

```cpp
static constexpr std::size_t tests_divisor{ 256 };
```

#### Managed Thread
Suppresses the check that `print` is called on the `run` thread.
Set this for loggers whose backend owns the consumer thread.

```cpp
static constexpr bool tests_managed_thread{ true };
```

#### Templated Post
Makes the harness pass the format string as a template argument instead of a function argument,
replacing `logger.post("format {}", 1)` with `logger.post<"format {}">(1)`.

```cpp
static constexpr bool tests_templated_post{ true };
```

</details>

## Build
Requirements for building this project:
* Compiler with C++20 support.
* [CMake](https://cmake.org/download/) version 3.25 or later
* [Ninja](https://github.com/ninja-build/ninja/releases)

Requirements for building benchmarks (optional):
* [Vcpkg](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started)

### Windows
On Windows, you can install all dependencies from the Visual Studio Installer:
* Desktop development with C++
  - C++ CMake tools for Windows
  - vcpkg package manager

Then open this project as a directory in Visual Studio.

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
