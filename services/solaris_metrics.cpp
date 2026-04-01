#include <kstat.h>
#include <sys/sysinfo.h>
#include <unistd.h>

// Собираем загрузку CPU (упрощенно для Solaris)
double get_solaris_cpu() {
    kstat_ctl_t *kc = kstat_open();
    if (!kc) return 0.0;
    
    // На Solaris T/M сериях много аппаратных потоков. 
    // Для простоты берем системный замер через sysconf или kstat "cpu_stat"
    long loadavg;
    if (getloadavg(loadavg, 3) != -1) {
        kstat_close(kc);
        return (double)loadavg / 100.0; // Примерный Load Average
    }
    
    kstat_close(kc);
    return 0.0;
}

// Собираем RAM
double get_solaris_ram() {
    long pagesize = sysconf(_SC_PAGESIZE);
    long phys_pages = sysconf(_SC_PHYS_PAGES);
    long avphys_pages = sysconf(_SC_AVPHYS_PAGES);
    double used_gb = (double)(phys_pages - avphys_pages) * pagesize / (1024 * 1024 * 1024);
    return used_gb;
}
