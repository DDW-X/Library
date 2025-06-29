// pci_common.c - Placeholder content
#include "omniaccess.h"
#include <stdint.h>

// توابع مشترک برای کار با PCI در معماری x86
uint32_t pci_config_read(uint32_t bus, uint32_t device, uint32_t function, uint32_t offset) {
    // ساخت آدرس PCI Configuration
    uint32_t address = (1 << 31) | (bus << 16) | (device << 11) | (function << 8) | (offset & 0xFC);
    
    // نوشتن آدرس به پورت 0xCF8
    #ifdef _WIN32
    WRITE_PORT_ULONG((PULONG)0xCF8, address);
    #else
    asm volatile("outl %0, %1" : : "a"(address), "Nd"(0xCF8));
    #endif
    
    // خواندن داده از پورت 0xCFC
    #ifdef _WIN32
    return READ_PORT_ULONG((PULONG)0xCFC);
    #else
    uint32_t data;
    asm volatile("inl %1, %0" : "=a"(data) : "Nd"(0xCFC));
    return data;
    #endif
}

void pci_config_write(uint32_t bus, uint32_t device, uint32_t function, uint32_t offset, uint32_t value) {
    // ساخت آدرس PCI Configuration
    uint32_t address = (1 << 31) | (bus << 16) | (device << 11) | (function << 8) | (offset & 0xFC);
    
    // نوشتن آدرس به پورت 0xCF8
    #ifdef _WIN32
    WRITE_PORT_ULONG((PULONG)0xCF8, address);
    #else
    asm volatile("outl %0, %1" : : "a"(address), "Nd"(0xCF8));
    #endif
    
    // نوشتن داده به پورت 0xCFC
    #ifdef _WIN32
    WRITE_PORT_ULONG((PULONG)0xCFC, value);
    #else
    asm volatile("outl %0, %1" : : "a"(value), "Nd"(0xCFC));
    #endif
}

// اسکن تمام دستگاه‌های PCI در سیستم
void pci_scan_all() {
    for (uint32_t bus = 0; bus < 256; bus++) {
        for (uint32_t device = 0; device < 32; device++) {
            for (uint32_t function = 0; function < 8; function++) {
                uint32_t vendor_id = pci_config_read(bus, device, function, 0) & 0xFFFF;
                if (vendor_id != 0xFFFF) {
                    uint32_t device_id = pci_config_read(bus, device, function, 0) >> 16;
                    // پردازش دستگاه پیدا شده
                }
            }
        }
    }
}