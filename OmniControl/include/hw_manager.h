#pragma once
#include <stdint.h>
#include <stdbool.h>

// انواع دستگاه‌های سخت‌افزاری
typedef enum {
    DEV_CPU,
    DEV_GPU,
    DEV_NIC,
    DEV_STORAGE,
    DEV_FPGA
} DeviceType;

// وضعیت دستگاه
typedef struct {
    DeviceType type;
    uint32_t id;
    char name[64];
    uint32_t temperature;
    uint32_t power_usage;
    uint32_t utilization;
    bool is_healthy;
} DeviceStatus;

// پروفایل سخت‌افزاری
typedef struct {
    DeviceStatus* devices;
    uint32_t count;
    uint32_t total_power;
    uint32_t max_temperature;
} HardwareProfile;

// API مدیریت سخت‌افزار
void hwmgr_scan_hardware(HardwareProfile* profile);
bool hwmgr_set_power_limit(uint32_t device_id, uint32_t power_limit);
bool hwmgr_set_clock_speed(uint32_t device_id, uint32_t clock_speed);
bool hwmgr_reset_device(uint32_t device_id);
bool hwmgr_update_driver(uint32_t device_id, const char* driver_path);