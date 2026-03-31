# 🌟 Aura Agent

**High-performance autonomous AI agent** built in modern C++.

Aura Agent is a lightweight, fast and extensible autonomous agent capable of reasoning, tool usage, and long-running task execution.

---

## ✨ Key Features

- **High Performance** — Written in modern C++ for maximum speed and efficiency
- **Autonomous Reasoning** — Advanced decision-making and multi-step planning
- **Modular Architecture** — Clean separation between core, libraries and services
- **Tool Integration** — Easy extension with custom tools and external APIs
- **Production Ready** — Designed with observability, logging and reliability in mind
- **Cross-platform** — Supports Linux, Windows and macOS

## 🛠️ Tech Stack

- **Language**: C++20 / C++23
- **Build System**: CMake
- **Core Libraries**: 
  - Standard Template Library (STL)
  - spdlog (logging)
  - nlohmann/json (JSON handling)
  - cURL / HTTP client (optional)
- **Monitoring**: Prometheus client (optional)
- **Containerization**: Docker

## 🚀 Quick Start

### Prerequisites

- C++20 compatible compiler (GCC 11+, Clang 14+, MSVC 2022+)
- CMake 3.20+
- Git

### Building the Project

```bash
git clone https://github.com/aura-buff/aura-agent.git
cd aura-agent
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
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
