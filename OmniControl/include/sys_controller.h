#pragma once
#include <stdint.h>
#include <stdbool.h>

// سطوح کنترل سیستم
typedef enum {
    SYSCTRL_LOW_POWER,
    SYSCTRL_BALANCED,
    SYSCTRL_HIGH_PERFORMANCE,
    SYSCTRL_EXTREME
} SystemMode;

// API کنترل سیستم
void sysctrl_set_system_mode(SystemMode mode);
void sysctrl_adjust_power_plan(uint32_t cpu_limit, uint32_t gpu_limit);
bool sysctrl_manage_thermal(uint32_t critical_temp);
void sysctrl_optimize_memory();
void sysctrl_configure_network(uint32_t bandwidth_mbps, uint32_t latency_ms);
void sysctrl_emergency_shutdown();