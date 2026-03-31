#ifndef METRICS_HPP
#define METRICS_HPP

#include <prometheus/registry.h>
#include <prometheus/gauge.h>
#include <prometheus/counter.h>
#include <memory>
#include <string>
#include <vector>

class Metrics {
public:
    explicit Metrics(std::shared_ptr<prometheus::Registry> registry);

    // Основные методы обновления данных
    void updateAll();
    void updateRamMetrics();
    void updateCpuMetrics();
    void updateDiskMetrics();
    void updateNetworkMetrics();

private:
    // --- RAM ---
    prometheus::Family<prometheus::Gauge>& ram_family;
    prometheus::Gauge& ram_total;
    prometheus::Gauge& ram_available;

    // --- CPU ---
    prometheus::Family<prometheus::Gauge>& cpu_family;
    prometheus::Gauge& cpu_load; // Процент загрузки (1 min average)

    // --- DISK (Физический /dev/sde или аналоги) ---
    prometheus::Family<prometheus::Gauge>& disk_family;
    prometheus::Gauge& disk_total;
    prometheus::Gauge& disk_used;

    // --- NETWORK (Интерфейсы) ---
    // Для сети лучше использовать Counter, так как байты только растут
    prometheus::Family<prometheus::Counter>& net_family;
    prometheus::Counter& net_rx_bytes; // Получено
    prometheus::Counter& net_tx_bytes; // Отправлено

    // Вспомогательные методы для парсинга (только для Linux)
#ifndef __sun
    double getLinuxDiskUsage(const std::string& path);
#endif
};

#endif // METRICS_HPP
