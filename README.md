# Aura Agent

**High-performance modular C++ application**

Aura Agent is a lightweight, efficient and well-structured C++ project built with focus on performance, maintainability and clean architecture.

---

## ✨ Features

- Modern C++20 / C++23 codebase
- Clean and modular project structure
- High performance with low overhead
- Easy extensibility through libraries and services
- Ready for production deployment
- Docker support
- Optional Prometheus metrics for monitoring

## 🛠️ Tech Stack

- **Language**: C++20 / C++23
- **Build System**: CMake
- **Logging**: spdlog
- **JSON Processing**: nlohmann/json
- **Containerization**: Docker
- **Monitoring**: Prometheus (optional)

## 🚀 Quick Start

### Prerequisites

- C++20 compatible compiler (GCC 11+, Clang 14+, MSVC 2022+)
- CMake 3.20+
- Git

### Building from Source

```bash
git clone https://github.com/aura-buff/aura-agent.git
cd aura-agent

mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release -j$(nproc)
```

Running the Agent
```bash
./aura-agent
```
Or using Docker:
```bash
docker build -t aura-agent .
docker run --rm -it aura-agent
```
