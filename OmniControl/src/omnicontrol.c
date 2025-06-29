#include "omnicontrol.h"
#include "perf_monitor.h"
#include "hw_manager.h"
#include "sys_controller.h"
#include "security_layer.h"

static ControlConfig current_config;
static SystemStatus system_status;
static HardwareProfile hw_profile;

int omnicontrol_init(const ControlConfig* config) {
    if (!config) return -1;
    
    // کپی تنظیمات
    memcpy(&current_config, config, sizeof(ControlConfig));
    
    // مقداردهی اولیه ماژول‌ها
    perfmon_start_monitoring(config->sampling_interval);
    hwmgr_scan_hardware(&hw_profile);
    sysctrl_set_system_mode(SYSCTRL_BALANCED);
    seclayer_enable_protection(config->security_level);
    
    return 0;
}

void omnicontrol_shutdown() {
    perfmon_stop_monitoring();
    seclayer_disable_protection();
    sysctrl_set_system_mode(SYSCTRL_LOW_POWER);
}

SystemStatus omnicontrol_get_system_status() {
    // جمع‌آوری داده‌ها از ماژول‌ها
    CpuProfile cpu = perfmon_get_cpu_profile();
    MemoryProfile mem = perfmon_get_memory_profile();
    
    system_status.cpu_usage = cpu.utilization;
    system_status.total_memory = mem.total;
    system_status.free_memory = mem.used;
    system_status.temperature = hw_profile.max_temperature;
    system_status.power_consumption = hw_profile.total_power;
    
    return system_status;
}

void omnicontrol_optimize_performance() {
    // بهینه‌سازی چندلایه
    sysctrl_set_system_mode(SYSCTRL_HIGH_PERFORMANCE);
    sysctrl_optimize_memory();
    sysctrl_configure_network(1000, 10); // 1Gbps, 10ms
    
    // اورکلاک کنترل شده
    for (uint32_t i = 0; i < hw_profile.count; i++) {
        if (hw_profile.devices[i].type == DEV_CPU ||
            hw_profile.devices[i].type == DEV_GPU) {
            hwmgr_set_clock_speed(hw_profile.devices[i].id, 120);
        }
    }
}

// توابع دیگر برای مدیریت امنیت و سخت‌افزار