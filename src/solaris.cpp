#include <prometheus/exposer.hpp>
#include <prometheus/registry.hpp>
#include <prometheus/gauge.hpp>
#include <chrono>
#include <thread>
#include <iostream>

// Подключаем твой файл с метриками Solaris
#include "solaris_metrics.hpp"

int main() {
    using namespace prometheus;

    try {
        // Запускаем HTTP-сервер на порту 3100
        // "0.0.0.0" позволяет центральному серверу подключаться к Solaris снаружи
        Exposer exposer{"0.0.0.0:3100"};
        
        std::cout << "[INFO] Aura Solaris Exporter started on port 3100" << std::endl;

        // Создаем реестр метрик
        auto registry = std::make_shared<Registry>();

        // Определяем метрики (семьи и сами значения Gauge)
        auto& cpu_family = Family<Gauge>::Build(*registry, "aura_cpu_load", "Solaris CPU Load Average").Add({});
        auto& ram_family = Family<Gauge>::Build(*registry, "aura_ram_usage_gb", "Solaris Used RAM in GB").Add({});
        auto& disk_family = Family<Gauge>::Build(*registry, "aura_disk_usage_gb", "Solaris Used Disk in GB").Add({});

        // Регистрируем наш реестр в экспоузере
        exposer.RegisterCollectable(registry);

        // Бесконечный цикл обновления данных
        while (true) {
            // Обновляем значения из твоего файла solaris_metrics.cpp
            cpu_family.Set(get_solaris_cpu());
            ram_family.Set(get_solaris_ram());
            disk_family.Set(get_solaris_disk());

            // Спим 5 секунд перед следующим замером
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Runtime error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
