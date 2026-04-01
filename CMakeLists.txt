cmake_minimum_required(VERSION 3.10)
project(aura-agent VERSION 1.0.0)

# 1. Настройки стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Поиск необходимых системных библиотек
find_package(Threads REQUIRED)

# Поиск prometheus-cpp (должна быть установлена в систему через make install)
find_package(prometheus-cpp CONFIG REQUIRED)

# 3. Сбор списка исходных файлов
# Используем явный список, чтобы CMake понимал структуру проекта
set(SOURCES
    src/main.cpp
    services/metrics.cpp
    services/discovery.cpp
    services/notification.cpp
    # Если добавишь notification.cpp или другие — просто допиши их сюда
)

# 4. Создание исполняемого файла
add_executable(${PROJECT_NAME} ${SOURCES})

# 5. Указание путей к заголовочным файлам (.hpp)
# Это позволит писать #include "metrics.hpp" вместо длинных путей
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/libs
    ${CMAKE_CURRENT_SOURCE_DIR}/services
)

# 6. Линковка библиотек
target_link_libraries(${PROJECT_NAME} PRIVATE
    prometheus-cpp::pull
    prometheus-cpp::core
    Threads::Threads
    # Системные библиотеки для работы сети и диска в Linux
    z
    curl
)

# 7. Настройки для инсталляции (команда 'make install')
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# Сообщение для отладки при запуске cmake
message(STATUS "Configuring ${PROJECT_NAME} v${PROJECT_VERSION}")
