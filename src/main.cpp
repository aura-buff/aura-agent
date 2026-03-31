#include <prometheus/exposer.h>
#include <prometheus/registry.h>
#include <chrono>
#include <thread>
#include <memory>
#include <iostream>

#include "../libs/metrics.hpp"

int main() {
    using namespace prometheus;

    auto registry = std::make_shared<Registry>();

    Metrics metrics(registry);

    Exposer exposer{"0.0.0.0:3100"};
    
    exposer.RegisterCollectable(registry);

    std::cout << "Aura Agent started on port 3100..." << std::endl;
    std::cout << "Metrics available at http://localhost:3100/metrics" << std::endl;

    while (true) {
        try {
            metrics.updateAll();
        } catch (const std::exception& e) {
            std::cerr << "Error updating metrics: " << e.what() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    return 0;
}
