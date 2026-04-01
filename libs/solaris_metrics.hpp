#ifndef SOLARIS_METRICS_HPP
#define SOLARIS_METRICS_HPP

#include <kstat.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <string>

class SolarisMetrics {
public:
    SolarisMetrics();
    ~SolarisMetrics();

    void updateAll();
    
    // Геттеры для алертов (совпадают с Linux версией)
    double getCpuLoad();
    double getRamUsagePct();

private:
    kstat_ctl_t *kc; // Дескриптор для работы с kstat API
    
    double last_cpu_load = 0.0;
    double last_ram_pct = 0.0;

    void updateCpu();
    void updateRam();
};

#endif
