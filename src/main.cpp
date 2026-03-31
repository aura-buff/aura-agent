#include <prometheus/exposer.h>
#include <prometheus/registry.h>

#include <thread>
#include <chrono>
#include <iostream>

#include "../libs/metrics.hpp"

int main() {
         try {
                prometheus::Exposer exposer{"0.0.0.0:3100"};
                auto registry = std::make_shared<prometheus::Registry>();

                Metrics metrics(registry);

                exposer.RegisterCollectable(registry);

                std::cout << "Auri: Starting on http://0.0.0.0:3100/metrics" << std::endl;

                while (true) {
                        metrics.getMetrics();
                        std::this_thread::sleep_for(std::chrono::seconds(5));
                }
         } catch (const std::exception& e) {
                std::cerr << "Auri: Fatal Error - " << e.what() << std::endl;
                return 1;
         }
         return 0;
}
~
