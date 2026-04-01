cmake_minimum_required(VERSION 3.10)
project(aura-agent-solaris)

set(CMAKE_CXX_STANDARD 17)

# Добавляем флаги для Solaris
if(CMAKE_SYSTEM_NAME STREQUAL "SunOS")
    add_definitions(-D__sun -DSOLARIS)
    # kstat — для метрик, nsl и socket — для сети на Solaris
    set(SOLARIS_LIBS "kstat" "nsl" "socket")
endif()

# Ищем prometheus-cpp (он должен быть собран статически под SPARC)
find_package(prometheus-cpp CONFIG REQUIRED)

add_executable(aura-agent-solaris 
    src/solaris.cpp 
    services/metrics_solaris.cpp
)

target_link_libraries(aura-agent-solaris 
    PRIVATE 
    prometheus-cpp::core
    ${SOLARIS_LIBS}
    threads
)

# Статическая линковка — залог того, что "закинул и запустил" сработает
set_target_properties(aura-agent-solaris PROPERTIES LINK_FLAGS "-static")
