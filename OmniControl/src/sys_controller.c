#include "sys_controller.h"
#include "perf_monitor.h"
#include "hw_manager.h"
#include <math.h>

// تنظیم حالت سیستم با بهینه‌سازی چندلایه
void sysctrl_set_system_mode(SystemMode mode) {
    switch (mode) {
        case SYSCTRL_LOW_POWER:
            // بهینه‌سازی برای مصرف کم انرژی
            sysctrl_adjust_power_plan(30, 40);
            seclayer_enable_protection(SEC_LEVEL_NORMAL);
            break;
        case SYSCTRL_HIGH_PERFORMANCE:
            // بهینه‌سازی برای حداکثر عملکرد
            sysctrl_adjust_power_plan(100, 100);
            sysctrl_optimize_memory();
            seclayer_enable_protection(SEC_LEVEL_ENHANCED);
            break;
        case SYSCTRL_EXTREME:
            // حالت حداکثری با اولویت عملکرد
            sysctrl_adjust_power_plan(120, 120);
            hwmgr_set_clock_speed(0, 5000); // اورکلاک
            seclayer_enable_protection(SEC_LEVEL_HIGH);
            break;
        default: // BALANCED
            sysctrl_adjust_power_plan(70, 80);
            seclayer_enable_protection(SEC_LEVEL_NORMAL);
    }
}

// تنظیم دقیق پارامترهای قدرت
void sysctrl_adjust_power_plan(uint32_t cpu_limit, uint32_t gpu_limit) {
    // دسترسی مستقیم به رجیسترهای کنترل قدرت
    *((volatile uint32_t*)0x1A0) = cpu_limit;
    *((volatile uint32_t*)0x1A4) = gpu_limit;
    
    // تنظیمات پیشرفته مدیریت انرژی
    uint32_t power_reg = *((volatile uint32_t*)0x1B0);
    power_reg = (power_reg & 0xFFFF0000) | ((cpu_limit + gpu_limit) / 2);
    *((volatile uint32_t*)0x1B0) = power_reg;
}

// مدیریت حرارتی پیشرفته
bool sysctrl_manage_thermal(uint32_t critical_temp) {
    CpuProfile cpu = perfmon_get_cpu_profile();
    if (cpu.temperature >= critical_temp) {
        // فعال‌سازی مکانیزم‌های خنک‌کننده اضطراری
        hwmgr_set_power_limit(0, 50); // کاهش قدرت CPU
        sysctrl_adjust_power_plan(50, 70);
        return true;
    }
    return false;
}