#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "perf_monitor.h"
#include "hw_manager.h"
#include "sys_controller.h"
#include "security_layer.h"

#define OMNI_CONTROL_VERSION "1.0.0"

// وضعیت سیستم
typedef struct {
    uint64_t total_memory;
    uint64_t free_memory;
    double cpu_usage;
    double temperature;
    uint32_t power_consumption;
} SystemStatus;

// تنظیمات کنترلر
typedef struct {
    uint32_t sampling_interval;
    uint8_t security_level;
    bool enable_hw_monitoring;
    bool enable_perf_tuning;
} ControlConfig;

// API اصلی
int omnicontrol_init(const ControlConfig* config);
void omnicontrol_shutdown();
SystemStatus omnicontrol_get_system_status();
void omnicontrol_optimize_performance();
void omnicontrol_enable_security_mode(uint8_t level);
void omnicontrol_manage_hardware(HardwareProfile* profile);