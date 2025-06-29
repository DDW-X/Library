// physical.h - Placeholder content
#pragma once
#include "compat.h"

#ifdef __KERNEL__
#include <linux/io.h>
#include <asm/msr.h>
#include <asm/pci.h>

// توابع کرنل
int linux_map_physical(uint64_t phys_addr, size_t size, void **virt_addr);
int linux_unmap_physical(void *virt_addr, size_t size);
uint8_t linux_port_read(uint16_t port);
void linux_port_write(uint16_t port, uint8_t value);
uint32_t linux_pci_read(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg);
void linux_pci_write(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg, uint32_t value);
uint64_t linux_read_msr(uint32_t msr);
void linux_write_msr(uint32_t msr, uint64_t value);
int linux_exec_kernel_code(void (*func)());

#else
// توابع فضای کاربر
int linux_user_map_physical(uint64_t phys_addr, size_t size, void **virt_addr);
int linux_user_unmap_physical(void *virt_addr, size_t size);
uint8_t linux_user_port_read(uint16_t port);
void linux_user_port_write(uint16_t port, uint8_t value);
#endif