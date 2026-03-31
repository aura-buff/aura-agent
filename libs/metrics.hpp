#pragma once
#include <prmetheus/registry.h>
#include <prometheus/gauge.h>
#include <memory>

class Metrics {
public:
    explicit Metrics(std::shared_ptr<prometheus::Registry> registry);
    void getMetrics();

private:
    // RAM Metrics declaration
    prometheus::Family<prometheus::Gauge>& ram_family;
    prometheus::Gauge& ram_total;
    prometheus::Gauge& ram_free;

    // CPU Metrics declaration
    prometheus::Family<prometheus::Gauge>& cpu_family;
    prometheus::Gauge& cpu_load;

    // Disk Metrics declaration
    prometheus::Family<prometheus::Gauge>& disk_family;
    prometheus::Gauge& disk_total;
    prometheus::Gauge& disk_free;

    // Declarating functions
    void updateRamMetrics();
    void updateCpuMetrics();
    void updateDiskMetrics();
};o
