// ioctl.h - Placeholder content
#pragma once
#include <winioctl.h>

#define FILE_DEVICE_OMNIACCESS 0x8000

#define IOCTL_READ_PHYS_MEM CTL_CODE(FILE_DEVICE_OMNIACCESS, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_WRITE_PHYS_MEM CTL_CODE(FILE_DEVICE_OMNIACCESS, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_MAP_PHYS_MEM CTL_CODE(FILE_DEVICE_OMNIACCESS, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_UNMAP_PHYS_MEM CTL_CODE(FILE_DEVICE_OMNIACCESS, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PORT_READ CTL_CODE(FILE_DEVICE_OMNIACCESS, 0x804, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PORT_WRITE CTL_CODE(FILE_DEVICE_OMNIACCESS, 0x805, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PCI_READ CTL_CODE(FILE_DEVICE_OMNIACCESS, 0x806, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PCI_WRITE CTL_CODE(FILE_DEVICE_OMNIACCESS, 0x807, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_READ_MSR CTL_CODE(FILE_DEVICE_OMNIACCESS, 0x808, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_WRITE_MSR CTL_CODE(FILE_DEVICE_OMNIACCESS, 0x809, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_EXEC_KERNEL_CODE CTL_CODE(FILE_DEVICE_OMNIACCESS, 0x80A, METHOD_BUFFERED, FILE_ANY_ACCESS)