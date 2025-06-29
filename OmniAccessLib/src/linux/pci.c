// pci.c - Placeholder content
#include "linux/compat.h"
#include <linux/pci.h>

#ifdef __KERNEL__

uint32_t linux_pci_read(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg) {
    return pci_bus_read_config_dword(pci_find_bus(0, bus), 
        PCI_DEVFN(device, function), reg, NULL);
}

void linux_pci_write(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg, uint32_t value) {
    pci_bus_write_config_dword(pci_find_bus(0, bus), 
        PCI_DEVFN(device, function), reg, value);
}

#else

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

uint32_t linux_pci_read(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg) {
    char path[256];
    snprintf(path, sizeof(path), "/sys/bus/pci/devices/0000:%02x:%02x.%d/config",
        bus, device, function);
    
    int fd = open(path, O_RDONLY);
    if (fd < 0) return 0xFFFFFFFF;
    
    lseek(fd, reg, SEEK_SET);
    uint32_t value;
    read(fd, &value, sizeof(value));
    close(fd);
    return value;
}

void linux_pci_write(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg, uint32_t value) {
    char path[256];
    snprintf(path, sizeof(path), "/sys/bus/pci/devices/0000:%02x:%02x.%d/config",
        bus, device, function);
    
    int fd = open(path, O_WRONLY);
    if (fd < 0) return;
    
    lseek(fd, reg, SEEK_SET);
    write(fd, &value, sizeof(value));
    close(fd);
}

#endif