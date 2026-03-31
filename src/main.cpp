#include <prometheus/exposer.h>
#include <prometheus/registry.h>
#include <chrono>
#include <thread>
#include <memory>
#include <iostream>

#include "../libs/metrics.hpp"
#include "../services/notification.cpp"

int main() {
    using namespace prometheus;

    auto registry = std::make_shared<Registry>();

    Metrics metrics(registry);

    Exposer exposer{"0.0.0.0:3100"};
    
    exposer.RegisterCollectable(registry);

    std::cout << "[Auri] Aura Agent started on port 3100..." << std::endl;
    std::cout << "[Auri] Metrics available at http://localhost:3100/metrics" << std::endl;
    
    Notification notifier("BOT_API", "CHAT_ID");


    while (true) {
        try {
            metrics.updateAll();
	    notifier.checkLimits(
	    	metrics.getCpuLoad(),
		metrics.getRamUsagePct(),
		metrics.getDiskUsagePct()
	    );
        } catch (const std::exception& e) {
            std::cerr << "[Auri] Error updating metrics: " << e.what() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    return 0;
}
