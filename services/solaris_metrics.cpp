#include <kstat.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

// Собираем загрузку CPU
double get_solaris_cpu() {
    double load;
    // getloadavg возвращает количество успешно считанных элементов
    if (getloadavg(load, 3) != -1) {
        // Возвращаем Load Average за последнюю 1 минуту
        return load; 
    }
    return 0.0;
}

// Собираем RAM в Гигабайтах
double get_solaris_ram() {
    // На Solaris sysconf — самый надежный способ получить страницы памяти
    long pagesize = sysconf(_SC_PAGESIZE);
    long phys_pages = sysconf(_SC_PHYS_PAGES);      // Всего страниц
    long avphys_pages = sysconf(_SC_AVPHYS_PAGES);  // Свободных страниц

    if (pagesize == -1 || phys_pages == -1 || avphys_pages == -1) {
        return 0.0;
    }

    double used_bytes = (double)(phys_pages - avphys_pages) * pagesize;
    return used_bytes / (1024.0 * 1024.0 * 1024.0);
}

// Добавим сбор данных по диску (Solaris специфично)
double get_solaris_disk() {
    // Для Solaris проще всего использовать statvfs на корень или рабочую директорию
    #include <sys/statvfs.h>
    struct statvfs vfs;
    if (statvfs("/", &vfs) == 0) {
        unsigned long long total = vfs.f_blocks * vfs.f_frsize;
        unsigned long long free = vfs.f_bfree * vfs.f_frsize;
        return (double)(total - free) / (1024.0 * 1024.0 * 1024.0);
    }
    return 0.0;
}
