#include <unistd.h>
#include <stdlib.h>

double get_solaris_cpu() {
    double load_array; // Явно создаем массив из 1 элемента
    // Передаем load_array — в C++ имя массива само по себе является указателем (double*)
    if (getloadavg(&load_array, 1)) {
        return load_array;
    }
    return 0.0;
}

double get_solaris_ram() {
    long pagesize = sysconf(_SC_PAGESIZE);
    long phys_pages = sysconf(_SC_PHYS_PAGES);
    long avphys_pages = sysconf(_SC_AVPHYS_PAGES);
    
    if (pagesize <= 0 || phys_pages <= 0) return 0.0;
    
    // Используем явное приведение к double для точности
    double used_gb = (static_cast<double>(phys_pages) - static_cast<double>(avphys_pages)) 
                     * static_cast<double>(pagesize) / (1024.0 * 1024.0 * 1024.0);
    return used_gb;
}

double get_solaris_disk() {
    return 0.0; // Заглушка
}
