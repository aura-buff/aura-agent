cmake_minimum_required(VERSION 3.10)
project(aura-agent-solaris)

set(CMAKE_CXX_STANDARD 17)

# Ищем библиотеку, которую мы только что установили в /usr/sparc64-linux-gnu/
find_package(prometheus-cpp CONFIG REQUIRED)

add_executable(aura-agent-solaris 
    src/solaris.cpp 
    services/solaris_metrics.cpp
)

# Линкуем ядро прометея и системные либы Solaris (они нужны для запуска на целевой машине)
target_link_libraries(aura-agent-solaris 
    PRIVATE 
    prometheus-cpp::core
    prometheus-cpp::pull
    # Эти либы будут искаться уже на самом Solaris при запуске
    kstat
    nsl
    socket
)
