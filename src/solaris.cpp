#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../libs/solaris_metrics.hpp"

void handle_request(int client_socket) {
    char buffer = {0};
    ssize_t bytes_read = read(client_socket, &buffer, sizeof(buffer)); // Читаем запрос (нам неважно какой)

    std::stringstream body;
    body << "# HELP aura_cpu_load Solaris CPU Load Average\n"
         << "aura_cpu_load " << get_solaris_cpu() << "\n"
         << "# HELP aura_ram_usage_gb Solaris Used RAM in GB\n"
         << "aura_ram_usage_gb " << get_solaris_ram() << "\n"
         << "# HELP aura_disk_usage_gb Solaris Used Disk in GB\n"
         << "aura_disk_usage_gb " << get_solaris_disk() << "\n";

    std::string metrics = body.str();
    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n"
             << "Content-Type: text/plain; version=0.0.4; charset=utf-8\r\n"
             << "Content-Length: " << metrics.length() << "\r\n"
             << "Connection: close\r\n\r\n"
             << metrics;

    std::string full_response = response.str();
    send(client_socket, full_response.c_str(), full_response.length(), 0);
    close(client_socket);
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(3100);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return 1;
    }

    listen(server_fd, 3);
    std::cout << "[AURA] Solaris Exporter running on port 3100..." << std::endl;

    while (true) {
        int addrlen = sizeof(address);
        int client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (client_socket >= 0) {
            handle_request(client_socket);
        }
    }
    return 0;
}
