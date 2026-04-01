#ifndef DISCOVERY_HPP
#define DISCOVERY_HPP

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "notification.hpp"

struct Network {
    std::string name;
    std::string subnet;
    int mask;
};

struct ScannerConfig {
    int agent_port;
    int timeout_ms;
    int scan_interval;
};

class NetworkScanner {
public:
    NetworkScanner(const std::string& configPath);
    void scanAll();
    void startAutoScan(int intervalSeconds);

private:
    bool checkAgent(const std::string& ip);
    bool checkPort(const std::string& ip, int port); // Объявляем тут!
    void saveTargets(const std::vector<std::string>& ips); 
    std::vector<Network> networks;

    int scanCounter = 0; 
    Notification* notifier = nullptr;

    ScannerConfig config; // Теперь переменная config будет видна в .cpp
};

#endif
