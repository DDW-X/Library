#include "omnicontrol.h"
#include <stdio.h>
#include <unistd.h>

int main() {
    ControlConfig config = {
        .sampling_interval = 500,
        .security_level = SEC_LEVEL_ENHANCED,
        .enable_hw_monitoring = true,
        .enable_perf_tuning = true
    };
    
    omnicontrol_init(&config);
    
    for (int i = 0; i < 10; i++) {
        SystemStatus status = omnicontrol_get_system_status();
        printf("CPU: %.1f%%, Mem: %.1f/%.1f GB, Temp: %.1fC, Power: %dW\n",
               status.cpu_usage,
               (status.total_memory - status.free_memory) / 1e9,
               status.total_memory / 1e9,
               status.temperature,
               status.power_consumption);
        sleep(1);
    }
    
    omnicontrol_shutdown();
    return 0;
}