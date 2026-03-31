#ifndef NOTIFICATION_HPP
#define NOTIFICATION_HPP

#include <string>

class Notification {
public:
    Notification(const std::string& token, const std::string& chat_id);
    
    // Отправить произвольный текст
    void sendAlert(const std::string& message);
    
    // Проверка порогов (Thresholds)
    void checkLimits(double cpu, double ram_usage_pct, double disk_usage_pct);

private:
    std::string bot_token;
    std::string chat_id;
    
    // Флаги, чтобы не спамить каждую секунду, пока лимит превышен
    bool cpu_alert_sent = false;
    bool ram_alert_sent = false;
};

#endif
