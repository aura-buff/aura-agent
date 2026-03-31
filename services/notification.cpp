#include "notification.hpp"
#include <curl/curl.h>
#include <iostream>

Notification::Notification(const std::string& token, const std::string& id) 
    : bot_token(token), chat_id(id) {}

void Notification::sendAlert(const std::string& message) {
    CURL* curl = curl_easy_init();
    if (curl) {
        std::string url = "https://api.telegram.org/bot" + bot_token + "/sendMessage";
        std::string post_fields = "chat_id=" + chat_id + "&text=" + message;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields.c_str());

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Telegram delivery failed: " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
    }
}

void Notification::checkLimits(double cpu, double ram_pct, double disk_pct) {
    // Пример: Алерт если CPU > 90%
    if (cpu > 0.9 && !cpu_alert_sent) {
        if (!cpu_alert_sent) { // Если алерт еще не был отправлен
            sendAlert("⚠️ [Auri] High CPU Load: " + std::to_string(int(cpu * 100)) + "%");
            cpu_alert_sent = true;
        }
    } else if (cpu < 0.5) {
	if (cpu_alert_sent) {
            sendAlert("✅ [Auri] CPU Load normalized: " + std::to_string(int(cpu * 100)) + "%");
            cpu_alert_sent = false; // Сбрасываем флаг, теперь мы снова в норме
        }
    }

    // Пример: Алерт если Диск > 95%
    if (disk_pct > 80.0) {
        sendAlert("[Auri]🚨 Disk Space Critical: " + std::to_string(int(disk_pct)) + "% used!");
    }
}
