#pragma once
#include <stdint.h>

// پروفایل عملکرد CPU
typedef struct {
    uint32_t clock_speed;
    uint32_t utilization;
    uint32_t cache_misses;
    uint32_t instructions_per_cycle;
} CpuProfile;

// پروفایل عملکرد حافظه
typedef struct {
    uint64_t total;
    uint64_t used;
    uint64_t cache_usage;
    uint32_t page_faults;
} MemoryProfile;

// پروفایل عملکرد شبکه
typedef struct {
    uint64_t bytes_sent;
    uint64_t bytes_received;
    uint32_t packets_sent;
    uint32_t packets_received;
    uint32_t errors;
} NetworkProfile;

// API مانیتورینگ
void perfmon_start_monitoring(uint32_t interval_ms);
void perfmon_stop_monitoring();
CpuProfile perfmon_get_cpu_profile();
MemoryProfile perfmon_get_memory_profile();
NetworkProfile perfmon_get_network_profile();