// compat.h - Placeholder content
#pragma once
#ifdef __KERNEL__
#include <linux/types.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>
#else
#include <stdint.h>
#include <sys/io.h>
#endif

#define PHYS_MEM_DEVICE "/dev/mem"
#define MAX_KERNEL_CODE_SIZE 4096
#define OMNIDRIVER_MAJOR 240
#define DEVICE_NAME "omniaccess"

struct linux_phys_request {
    uint64_t phys_addr;
    size_t size;
    void __user *buffer;
};

struct port_io_request {
    uint16_t port;
    uint8_t value;
};

struct pci_request {
    uint8_t bus;
    uint8_t device;
    uint8_t function;
    uint8_t reg;
    uint32_t value;
};

struct msr_request {
    uint32_t msr;
    uint64_t value;
};

// IOCTL Definitions
#define OMNIIOCTL_READ_PHYS _IOR('O', 0x40, struct linux_phys_request)
#define OMNIIOCTL_WRITE_PHYS _IOW('O', 0x41, struct linux_phys_request)
#define OMNIIOCTL_PORT_READ _IOR('O', 0x42, struct port_io_request)
#define OMNIIOCTL_PORT_WRITE _IOW('O', 0x43, struct port_io_request)
#define OMNIIOCTL_PCI_READ _IOR('O', 0x44, struct pci_request)
#define OMNIIOCTL_PCI_WRITE _IOW('O', 0x45, struct pci_request)
#define OMNIIOCTL_READ_MSR _IOR('O', 0x46, struct msr_request)
#define OMNIIOCTL_WRITE_MSR _IOW('O', 0x47, struct msr_request)