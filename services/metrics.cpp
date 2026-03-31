void Metrics::updateRamMetrics() {
    const char* proc_ev = std::getenv("HOST_PROC");
    std::string memPath = proc_env ? std::string(proc_env) + "/meminfo" : "/proc/meminfo";

    std::ifstream file(memPath);
    std::string line;
    double total = 0, available = 0;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        double value;
        std::string unit;
        if (iss >> key >> value >> unit) {
            // В meminfo данные в kB. Переводим в байты: * 1024
            if (key == "MemTotal:") total = value * 1024;
            if (key == "MemAvailable:") available = value * 1024;
        }
    }
    ram_total.Set(total);
    ram_free.Set(available);
}

void Metrics::updateCpuMetrics() {
    const char* proc_env = std::getenv("HOST_PROC");
    std::string loadPath = proc_env ? std::string(proc_env) + "/loadavg" : "/proc/loadavg";

    std::ifstream file(loadPath);
    double load1;
    if (file >> load1) {
        cpu_load.Set(load1 * 100);
    }
}

// ИСПРАВЛЕНО: Добавлено Metrics:: и исправлены имена переменных
void Metrics::updateDiskMetrics() {
    struct statvfs stat;
    // Если в Docker проброшен корень хоста в /host/root, используй этот путь
    const char* root_path = std::getenv("HOST_ROOT");
    const char* target = root_path ? root_path : "/";

    if (statvfs(target, &stat) == 0) {
        double total = static_cast<double>(stat.f_blocks) * stat.f_frsize;
        double free = static_cast<double>(stat.f_bfree) * stat.f_frsize;

        disk_total.Set(total);
        disk_free.Set(free);
    }
}
