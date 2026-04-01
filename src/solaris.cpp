#include <prometheus/exposer.hpp>
#include <prometheus/registry.hpp>
#include <prometheus/gauge.hpp>
#include <chrono>
#include <thread>
#include "metrics_solaris.hpp"

int main() {
    using namespace prometheus;

    // Слушаем порт 3100, как и основной агент
    Exposer exposer{"0.0.0.0:3100"};
    auto registry = std::make_shared<Registry>();

    auto& cpu_gauge = Family<Gauge>::Build(*registry, "aura_cpu_usage", "CPU usage").Add({});
    auto& ram_gauge = Family<Gauge>::Build(*registry, "aura_ram_usage", "RAM usage").Add({});

    while (true) {
        cpu_gauge.Set(get_solaris_cpu());
        ram_gauge.Set(get_solaris_ram());
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    return 0;
}
