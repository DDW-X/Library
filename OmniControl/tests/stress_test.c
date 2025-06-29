#include "omnicontrol.h"
#include <assert.h>

void test_perf_monitoring() {
    perfmon_start_monitoring(100);
    sleep(1);
    CpuProfile cpu = perfmon_get_cpu_profile();
    assert(cpu.utilization > 0 && cpu.utilization < 100);
    perfmon_stop_monitoring();
}

void test_hardware_management() {
    HardwareProfile profile;
    hwmgr_scan_hardware(&profile);
    assert(profile.count > 0);
    assert(hwmgr_set_power_limit(0, 80));
}

// سایر تست‌ها

int main() {
    test_perf_monitoring();
    test_hardware_management();
    // ...
    printf("All tests passed!\n");
    return 0;
}