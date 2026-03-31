#include "../libs/metrics.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <sys/statvfs.h>

Metrics::Metrics(std::shared_ptr<prometheus::Registry> registry)
    : ram_family(prometheus::BuildGauge().Name("node_memory_bytes").Help("Memory stats").Register(*registry)),
      ram_total(ram_family.Add({{"type", "total"}})),
      ram_available(ram_family.Add({{"type", "available"}})),
      
      cpu_family(prometheus::BuildGauge().Name("node_cpu_load").Help("CPU load average").Register(*registry)),
      cpu_load(cpu_family.Add({{"period", "1m"}})),
      
      disk_family(prometheus::BuildGauge().Name("node_disk_bytes").Help("Disk usage stats").Register(*registry)),
      disk_total(disk_family.Add({{"device", "sde"}})),
      disk_used(disk_family.Add({{"device", "sde"}})),
      
      net_family(prometheus::BuildCounter().Name("node_network_bytes").Help("Network traffic").Register(*registry)),
      net_rx_bytes(net_family.Add({{"device", "all"}, {"direction", "rx"}})),
      net_tx_bytes(net_family.Add({{"device", "all"}, {"direction", "tx"}})) 
{}

void Metrics::updateAll() {
    updateRamMetrics();
    updateCpuMetrics();
    updateDiskMetrics();
    updateNetworkMetrics();
}

void Metrics::updateRamMetrics() {
    std::ifstream file("/proc/meminfo");
    std::string line;
    double total = 0, available = 0;
    while (std::getline(file, line)) {
        if (line.find("MemTotal:") == 0) std::stringstream(line.substr(9)) >> total;
        if (line.find("MemAvailable:") == 0) std::stringstream(line.substr(13)) >> available;
    }
    ram_total.Set(total * 1024);
    ram_available.Set(available * 1024);
    last_ram_pct = ((total - available) / total) * 100.0;
}

void Metrics::updateCpuMetrics() {
    std::ifstream file("/proc/loadavg");
    double load;
    if (file >> load) {
        cpu_load.Set(load);
	last_cpu_load = load;
    }
}

void Metrics::updateDiskMetrics() {
    struct statvfs stat;
    const char* path = "/"; 
    if (statvfs(path, &stat) == 0) {
        double total = (double)stat.f_blocks * stat.f_frsize;
        double free = (double)stat.f_bfree * stat.f_frsize;
        disk_total.Set(total);
        disk_used.Set(total - free);
    }
}

void Metrics::updateNetworkMetrics() {
    std::ifstream file("/proc/net/dev");
    std::string line;
    unsigned long long rx = 0, tx = 0;
    unsigned long long r_bytes, t_bytes;
    std::string iface;

    std::getline(file, line);
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        ss >> iface; 
        if (iface.find("lo:") != std::string::npos) continue;
        ss >> r_bytes;
        for(int i=0; i<7; ++i) { unsigned long long dummy; ss >> dummy; }
        ss >> t_bytes;

        rx += r_bytes;
        tx += t_bytes;
    }
    static unsigned long long last_rx = 0, last_tx = 0;
    if (rx > last_rx) net_rx_bytes.Increment(rx - last_rx);
    if (tx > last_tx) net_tx_bytes.Increment(tx - last_tx);
    last_rx = rx; last_tx = tx;
}
