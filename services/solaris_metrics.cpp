#include "../libs/solaris_metrics.hpp"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

SolarisMetrics::SolarisMetrics() {
    kc = kstat_open(); // Открываем интерфейс к статистике ядра Solaris
}

SolarisMetrics::~SolarisMetrics() {
    if (kc) kstat_close(kc);
}

void SolarisMetrics::updateCpu() {
    // На SPARC ищем системную статистику в модуле unix
    kstat_t *ksp = kstat_lookup(kc, (char*)"unix", 0, (char*)"system_misc");
    if (ksp && kstat_read(kc, ksp, NULL) != -1) {
        // avenrun_1min хранит Load Average в фиксированной запятой
        knote_t *kn = (knote_t*)kstat_data_lookup(ksp, (char*)"avenrun_1min");
        if (kn) {
            last_cpu_load = (double)kn->value.ui64 / 256.0; 
        }
    }
}

void SolarisMetrics::updateRam() {
    // В Solaris память измеряется страницами (pages)
    kstat_t *ksp = kstat_lookup(kc, (char*)"unix", 0, (char*)"system_pages");
    if (ksp && kstat_read(kc, ksp, NULL) != -1) {
        // freemem — свободные страницы, availrmem — доступные системе
        uint64_t free_p = ((knote_t*)kstat_data_lookup(ksp, (char*)"freemem"))->value.ui64;
        uint64_t total_p = ((knote_t*)kstat_data_lookup(ksp, (char*)"availrmem"))->value.ui64;
        
        if (total_p > 0) {
            last_ram_pct = 100.0 * (1.0 - ((double)free_p / (double)total_p));
        }
    }
}

void SolarisMetrics::updateAll() {
    if (kc && kstat_chain_update(kc) != -1) {
        updateCpu();
        updateRam();
    }
}

void SolarisMetrics::startHttpServer(int port) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    while (true) {
        int new_socket = accept(server_fd, NULL, NULL);
        updateAll(); // Обновляем kstat перед отдачей [cite: 31]

        // Формируем ответ в формате Prometheus
        std::string response = 
            "HTTP/1.1 200 OK\nContent-Type: text/plain\n\n"
            "# HELP cpu_load Current CPU load from kstat\n"
            "# TYPE cpu_load gauge\n"
            "cpu_load " + std::to_string(getCpuLoad()) + "\n"
            "# HELP ram_usage RAM usage percentage\n"
            "# TYPE ram_usage gauge\n"
            "ram_usage " + std::to_string(getRamUsagePct()) + "\n";

        send(new_socket, response.c_str(), response.length(), 0);
        close(new_socket);
    }
}

double SolarisMetrics::getCpuLoad() { return last_cpu_load; }
double SolarisMetrics::getRamUsagePct() { return last_ram_pct; }
