#include "notification.hpp"
#include <curl/curl.h>
#include <iostream>
#include <vector>
#include <sstream>

Notification::Notification(const std::string& token, const std::string& id) 
    : bot_token(token), chat_id(id) {}

void Notification::sendAlert(const std::string& message) {
    std::string command = "curl -s -X POST https://api.telegram.org/bot" + bot_token + "/sendMessage "
                          "--data-urlencode \"chat_id=" + chat_id + "\" "
                          "--data-urlencode \"text=" + message + "\" > /dev/null";
    system(command.c_str());
}

void Notification::sendDiscoveryReport(int totalChecked, int foundCount, const std::vector<std::string>& hosts, const std::string& target_id) {
    std::stringstream ss;
    ss << "📊 *Aura-Agent Report*\n"
       << "Checked: " << totalChecked << "\n"
       << "Found: " << foundCount << "\n";
    
    if (!hosts.empty()) {
        ss << "\n📍 *Active Nodes:*\n";
        for (const auto& ip : hosts) {
            ss << "• `" << ip << "`\n";
        }
    }

    std::string message = ss.str();

    std::string command = "curl -s -X POST https://api.telegram.org/bot" + bot_token + "/sendMessage "
                          "--data-urlencode \"chat_id=" + target_id + "\" "
                          "--data-urlencode \"parse_mode=Markdown\" "
                          "--data-urlencode \"text=" + message + "\" > /dev/null";
    system(command.c_str());
    std::cout << "[NOTIFICATION] Periodic discovery report sent to Telegram." << std::endl;
}

void Notification::checkLimits(double cpu, double ram, double disk) {
    if (cpu > 90.0 || ram > 90.0 || disk > 90.0) {
        std::stringstream ss;
        ss << "⚠️ *Resource Alert*\n"
           << "CPU: " << cpu << "%\n"
           << "RAM: " << ram << "%\n"
           << "Disk: " << disk << "%";
        
        std::string message = ss.str();
        
        std::string command = "curl -s -X POST https://api.telegram.org/bot" + bot_token + "/sendMessage "
                              "--data-urlencode \"chat_id=" + chat_id + "\" "
                              "--data-urlencode \"parse_mode=Markdown\" "
                              "--data-urlencode \"text=" + message + "\" > /dev/null";
        system(command.c_str());
    }
}

void Notification::notifyDiscovery(std::string ip, std::string os) {
    std::string msg = "🚀 **New Agent Discovered!**\n"
                      "📍 IP: " + ip + "\n"
                      "🖥 OS: " + os + "\n"
                      "✅ Added to monitoring automatically.";
    sendAlert(msg);
}
