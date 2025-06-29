// omniaccess.h - Placeholder content
#pragma once
#include <stdint.h>
#include <stddef.h>

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    #include "windows/driver.h"
    #include "windows/ioctl.h"
#elif defined(__linux__)
    #include "linux/compat.h"
    #include "linux/physical.h"
    #include <sys/mman.h>
    #include <fcntl.h>
    #include <unistd.h>
#endif

// تعاریف عمومی
#define OMNIDRIVER_NAME "OmniAccessDriver"
#define MAX_PHYS_MEM_MAP 16
#define MAX_MSR_ENTRIES 64

// ساختارهای مشترک
typedef struct {
    uint64_t physical_address;
    size_t size;
    void* mapped_address;
} PhysicalMemoryMapping;

typedef struct {
    uint16_t port;
    uint8_t value;
} PortIORequest;

typedef struct {
    uint8_t bus;
    uint8_t device;
    uint8_t function;
    uint8_t reg;
    uint32_t value;
} PCIRequest;

// توابع اصلی
int omni_init();
void omni_cleanup();
int omni_read_physical_memory(uint64_t phys_addr, void* buffer, size_t size);
int omni_write_physical_memory(uint64_t phys_addr, const void* data, size_t size);
void* omni_map_physical_memory(uint64_t phys_addr, size_t size);
void omni_unmap_physical_memory(void* mapped_addr, size_t size);
uint8_t omni_port_read(uint16_t port);
void omni_port_write(uint16_t port, uint8_t value);
uint32_t omni_pci_read(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg);
void omni_pci_write(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg, uint32_t value);
uint64_t omni_read_msr(uint32_t msr);
void omni_write_msr(uint32_t msr, uint64_t value);
int omni_modify_page_table(uint64_t virt_addr, uint64_t phys_addr, int flags);
void omni_enable_smm_access();
int omni_inject_kernel_code(void* code, size_t size);
void omni_install_interrupt_handler(int vector, void* handler);
void* omni_find_acpi_table(const char* signature);
int omni_exec_kernel_code(void (*func)());
void omni_enter_smm();