// driver.h - Placeholder content
#pragma once
#include <ntifs.h>

#define DRIVER_DEVICE_NAME L"\\Device\\OmniAccessDriver"
#define DRIVER_SYMBOLIC_LINK L"\\DosDevices\\OmniAccessDriver"

typedef struct {
    PVOID buffer;
    SIZE_T size;
    ULONGLONG address;
    ULONG port_number;
    UCHAR value;
    UCHAR pci_bus;
    UCHAR pci_device;
    UCHAR pci_function;
    UCHAR pci_register;
    ULONG pci_value;
    ULONG msr_register;
    ULONGLONG msr_value;
} KERNEL_REQUEST;

// توابع درایور
NTSTATUS DriverEntry(PDRIVER_OBJECT driver_obj, PUNICODE_STRING reg_path);
NTSTATUS DispatchCreate(PDEVICE_OBJECT device_obj, PIRP irp);
NTSTATUS DispatchClose(PDEVICE_OBJECT device_obj, PIRP irp);
NTSTATUS DispatchIoctl(PDEVICE_OBJECT device_obj, PIRP irp);
NTSTATUS DispatchUnsupported(PDEVICE_OBJECT device_obj, PIRP irp);
VOID DriverUnload(PDRIVER_OBJECT driver_obj);

// توابع کمکی
NTSTATUS HandleReadPhysicalMemory(PIRP irp, PIO_STACK_LOCATION stack);
NTSTATUS HandleWritePhysicalMemory(PIRP irp, PIO_STACK_LOCATION stack);
NTSTATUS HandlePortRead(PIRP irp, PIO_STACK_LOCATION stack);
NTSTATUS HandlePortWrite(PIRP irp, PIO_STACK_LOCATION stack);
NTSTATUS HandlePciRead(PIRP irp, PIO_STACK_LOCATION stack);
NTSTATUS HandlePciWrite(PIRP irp, PIO_STACK_LOCATION stack);
NTSTATUS HandleReadMsr(PIRP irp, PIO_STACK_LOCATION stack);
NTSTATUS HandleWriteMsr(PIRP irp, PIO_STACK_LOCATION stack);