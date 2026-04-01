#include "discovery.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>

// Конструктор: загружаем конфиг сразу при создании сканера
NetworkScanner::NetworkScanner(const std::string& configPath) {
    std::ifstream f(configPath);
    if (f.is_open()) {
        nlohmann::json data = nlohmann::json::parse(f);
        
        // Парсим основные настройки
        config.agent_port = data["discovery"]["agent_port"];
        config.timeout_ms = data["discovery"]["timeout_ms"];
        config.scan_interval = data["discovery"]["scan_interval_seconds"];

        // Парсим список сетей для сканирования
        for (auto& net : data["networks"]) {
            Network n;
            n.name = net["name"];
            n.subnet = net["subnet"];
            n.mask = net["mask"];
            networks.push_back(n);
        }
        std::cout << "[INIT] Config loaded. Target port: " << config.agent_port << std::endl;
    } else {
        std::cerr << "[ERROR] Could not open config file: " << configPath << std::endl;
    }
}

// Проверка: Жив ли IP (Ping)
bool NetworkScanner::checkAgent(const std::string& ip) {
    std::string command = "ping -c 1 -W 1 " + ip + " > /dev/null 2>&1";
    return (system(command.c_str()) == 0);
}

// Проверка: Запущен ли там наш агент (TCP Port Check)
bool NetworkScanner::checkPort(const std::string& ip, int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return false;

    // Устанавливаем таймаут для попытки соединения
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = config.timeout_ms * 1000; 
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof tv);

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr);

    // Пытаемся подключиться
    int result = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    
    close(sock);
    return (result == 0);
}

// Сохранение найденных агентов для Prometheus
void NetworkScanner::saveTargets(const std::vector<std::string>& ips) {
    nlohmann::json j = nlohmann::json::array();
    
    nlohmann::json targetObj;
    targetObj["targets"] = nlohmann::json::array();
    
    for (const auto& ip : ips) {
        targetObj["targets"].push_back(ip + ":" + std::to_string(config.agent_port));
    }
    
    targetObj["labels"] = {{"job", "aura-discovered"}};
    j.push_back(targetObj);

    std::ofstream file("/app/src/target.json");
    file << j.dump(4);
    file.close();
}

void NetworkScanner::startAutoScan(int intervalSeconds) {
    std::cout << "[AUTOSCAN] Started with interval: " << intervalSeconds << "s" << std::endl;
    
    while (true) {
        try {
            scanAll();
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Scan failed: " << e.what() << std::endl;
        }

        std::cout << "[AUTOSCAN] Sleeping for " << intervalSeconds << " seconds..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(intervalSeconds));
    }
}

// Основной цикл сканирования
void NetworkScanner::scanAll() {
    int totalChecked = 0;
    int foundCount = 0;
    std::vector<std::string> discoveredIPs;

    std::cout << "[DISCOVERY] Starting network scan..." << std::endl;

    for (const auto& net : networks) {
        std::cout << "[DISCOVERY] Scanning network: " << net.name << " (" << net.subnet << ")" << std::endl;
        
        // Логика формирования IP для маски /24
        std::string base = net.subnet.substr(0, net.subnet.find_last_of('.') + 1);
        
        for (int i = 1; i < 255; ++i) {
            std::string targetIp = base + std::to_string(i);
            totalChecked++;

            // 1. Сначала проверяем пингом, есть ли кто живой
            if (checkAgent(targetIp)) {
                // 2. Если живой, проверяем, наш ли это агент на порту 3100
                if (checkPort(targetIp, config.agent_port)) {
                    foundCount++;
                    discoveredIPs.push_back(targetIp);
                    std::cout << "[FOUND] Active agent at: " << targetIp << std::endl;
                }
            }
        }
    }

    // Сохраняем результат в файл для Prometheus
    saveTargets(discoveredIPs);

    std::cout << "------------------------------------------" << std::endl;
    std::cout << "[Auri DISCOVERY] Scan finished." << std::endl;
    std::cout << "[Auri DISCOVERY] Total IPs checked: " << totalChecked << std::endl;
    std::cout << "[Auri DISCOVERY] Agents found: " << foundCount << std::endl;
    std::cout << "------------------------------------------" << std::endl;
}
