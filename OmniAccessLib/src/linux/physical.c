// physical.c - Placeholder content
#include "linux/physical.h"
#include "linux/compat.h"
#include <linux/io.h>
#include <linux/mman.h>
#include <asm/io.h>

#ifdef __KERNEL__

int linux_map_physical(uint64_t phys_addr, size_t size, void **virt_addr) {
    *virt_addr = ioremap(phys_addr, size);
    return *virt_addr ? 0 : -ENOMEM;
}

int linux_unmap_physical(void *virt_addr, size_t size) {
    iounmap(virt_addr);
    return 0;
}

uint8_t linux_port_read(uint16_t port) {
    return inb(port);
}

void linux_port_write(uint16_t port, uint8_t value) {
    outb(value, port);
}

uint32_t linux_pci_read(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg) {
    return readl(PCI_ADDR(bus, device, function, reg));
}

void linux_pci_write(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg, uint32_t value) {
    writel(value, PCI_ADDR(bus, device, function, reg));
}

uint64_t linux_read_msr(uint32_t msr) {
    uint32_t low, high;
    asm volatile("rdmsr" : "=a"(low), "=d"(high) : "c"(msr));
    return ((uint64_t)high << 32) | low;
}

void linux_write_msr(uint32_t msr, uint64_t value) {
    uint32_t low = value & 0xFFFFFFFF;
    uint32_t high = value >> 32;
    asm volatile("wrmsr" : : "a"(low), "d"(high), "c"(msr));
}

int linux_exec_kernel_code(void (*func)()) {
    // Not safe - only for demonstration
    func();
    return 0;
}

#else

int linux_user_map_physical(uint64_t phys_addr, size_t size, void **virt_addr) {
    *virt_addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, (off_t)phys_addr);
    return (*virt_addr == MAP_FAILED) ? -1 : 0;
}

int linux_user_unmap_physical(void *virt_addr, size_t size) {
    return munmap(virt_addr, size);
}

uint8_t linux_user_port_read(uint16_t port) {
    uint8_t value;
    asm volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

void linux_user_port_write(uint16_t port, uint8_t value) {
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

#endif