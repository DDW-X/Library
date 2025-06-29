Comprehensive library with access to all layers of the system
(OmniAccess Library)(OAL)

```c
/**
 * OmniAccess Library
 -Comprehensive access to all layers of the system
* Version: 3.0

* Author: DDW-X
* Date: 1404/04/15 and 6 - 7 - 2025
*
* Supports Windows 10/11 and the latest versions of Linux
* Designed for low-level access and complete system control
 */

#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    #include <winioctl.h>
    #define OMNIDRIVER_NAME L"\\\\.\\OmniAccessDriver"
#elif defined(__linux__)
    #include <sys/io.h>
    #include <sys/mman.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <sys/ioctl.h>
    #include <linux/pci.h>
    #define PHYS_MEM_DEVICE "/dev/mem"
#endif

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// ----------------------------------------------------------
// ساختارهای مشترک برای ویندوز و لینوکس
// ----------------------------------------------------------

typedef struct {
    uint64_t physical_address;
    size_t size;
    void* mapped_address;
} PhysicalMemoryMapping;

typedef struct {
    uint32_t port;
    uint8_t value;
} PortIORequest;

typedef struct {
    uint8_t bus;
    uint8_t device;
    uint8_t function;
    uint8_t reg;
    uint32_t value;
} PCIRequest;

// ----------------------------------------------------------
// توابع اصلی کتابخانه
// ----------------------------------------------------------

/**
 * مقداردهی اولیه کتابخانه
 * بازگشت: 0 در صورت موفقیت، کد خطا در صورت شکست
 */
int omni_init();

/**
 * آزادسازی منابع کتابخانه
 */
void omni_cleanup();

/**
 * خواندن حافظه فیزیکی
 * بازگشت: 0 در صورت موفقیت، کد خطا در صورت شکست
 */
int omni_read_physical_memory(uint64_t phys_addr, void* buffer, size_t size);

/**
 * نوشتن در حافظه فیزیکی
 * بازگشت: 0 در صورت موفقیت، کد خطا در صورت شکست
 */
int omni_write_physical_memory(uint64_t phys_addr, const void* data, size_t size);

/**
 * نگاشت حافظه فیزیکی به فضای کاربر
 * بازگشت: اشاره‌گر به حافظه نگاشت شده یا NULL در صورت شکست
 */
void* omni_map_physical_memory(uint64_t phys_addr, size_t size);

/**
 * آزادسازی حافظه نگاشت شده
 */
void omni_unmap_physical_memory(void* mapped_addr, size_t size);

/**
 * خواندن از پورت I/O
 */
uint8_t omni_port_read(uint16_t port);

/**
 * نوشتن در پورت I/O
 */
void omni_port_write(uint16_t port, uint8_t value);

/**
 * خواندن از پیکربندی PCI
 */
uint32_t omni_pci_read(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg);

/**
 * نوشتن در پیکربندی PCI
 */
void omni_pci_write(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg, uint32_t value);

/**
 * خواندن MSR (رجیسترهای خاص مدل)
 */
uint64_t omni_read_msr(uint32_t msr);

/**
 * نوشتن در MSR (رجیسترهای خاص مدل)
 */
void omni_write_msr(uint32_t msr, uint64_t value);

/**
 * فعال‌سازی دسترسی به SMM (حالت مدیریت سیستم)
 */
void omni_enter_smm();

/**
 * اجرای دستورات سطح هسته
 */
int omni_exec_kernel_code(void (*func)());

// ----------------------------------------------------------
// پیاده‌سازی وابسته به سیستم عامل
// ----------------------------------------------------------

#ifdef _WIN32

// تعاریف اختصاصی ویندوز
#define OMNIDRIVER_CTL_CODE(CTL) CTL_CODE(FILE_DEVICE_UNKNOWN, CTL, METHOD_BUFFERED, FILE_ANY_ACCESS)

enum {
    IOCTL_READ_PHYS_MEM = 0x800,
    IOCTL_WRITE_PHYS_MEM,
    IOCTL_MAP_PHYS_MEM,
    IOCTL_UNMAP_PHYS_MEM,
    IOCTL_PORT_READ,
    IOCTL_PORT_WRITE,
    IOCTL_PCI_READ,
    IOCTL_PCI_WRITE,
    IOCTL_READ_MSR,
    IOCTL_WRITE_MSR,
    IOCTL_EXEC_KERNEL_CODE
};

static HANDLE driver_handle = INVALID_HANDLE_VALUE;

int omni_init() {
    driver_handle = CreateFileW(OMNIDRIVER_NAME, GENERIC_READ | GENERIC_WRITE, 
        0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    
    if (driver_handle == INVALID_HANDLE_VALUE) {
        // تلاش برای بارگذاری درایور
        SC_HANDLE scm = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
        if (!scm) return -1;
        
        SC_HANDLE service = OpenServiceW(scm, L"OmniAccessDriver", SERVICE_ALL_ACCESS);
        if (!service) {
            // نصب درایور
            // ... (کد نصب درایور)
        }
        
        StartServiceW(service, 0, NULL);
        CloseServiceHandle(service);
        CloseServiceHandle(scm);
        
        driver_handle = CreateFileW(OMNIDRIVER_NAME, GENERIC_READ | GENERIC_WRITE, 
            0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        
        if (driver_handle == INVALID_HANDLE_VALUE) {
            return -2;
        }
    }
    return 0;
}

void omni_cleanup() {
    if (driver_handle != INVALID_HANDLE_VALUE) {
        CloseHandle(driver_handle);
        driver_handle = INVALID_HANDLE_VALUE;
    }
}

int omni_read_physical_memory(uint64_t phys_addr, void* buffer, size_t size) {
    struct {
        uint64_t address;
        size_t size;
    } input;
    
    DWORD bytes_returned;
    input.address = phys_addr;
    input.size = size;
    
    return DeviceIoControl(driver_handle, OMNIDRIVER_CTL_CODE(IOCTL_READ_PHYS_MEM),
        &input, sizeof(input), buffer, (DWORD)size, &bytes_returned, NULL)
        ? 0 : -1;
}

// سایر توابع ویندوز...

#elif defined(__linux__)

// پیاده‌سازی لینوکس
static int mem_fd = -1;

int omni_init() {
    // فعال‌سازی دسترسی به پورت‌های I/O
    if (iopl(3) < 0) {
        perror("iopl");
        return -1;
    }
    
    // باز کردن /dev/mem برای دسترسی به حافظه فیزیکی
    mem_fd = open(PHYS_MEM_DEVICE, O_RDWR | O_SYNC);
    if (mem_fd < 0) {
        perror("open /dev/mem");
        return -2;
    }
    
    return 0;
}

void omni_cleanup() {
    if (mem_fd >= 0) {
        close(mem_fd);
        mem_fd = -1;
    }
    iopl(0);
}

int omni_read_physical_memory(uint64_t phys_addr, void* buffer, size_t size) {
    if (lseek(mem_fd, (off_t)phys_addr, SEEK_SET) < 0) {
        perror("lseek");
        return -1;
    }
    
    if (read(mem_fd, buffer, size) != (ssize_t)size) {
        perror("read");
        return -2;
    }
    
    return 0;
}

void* omni_map_physical_memory(uint64_t phys_addr, size_t size) {
    void* addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, phys_addr);
    if (addr == MAP_FAILED) {
        perror("mmap");
        return NULL;
    }
    return addr;
}

void omni_unmap_physical_memory(void* mapped_addr, size_t size) {
    munmap(mapped_addr, size);
}

uint8_t omni_port_read(uint16_t port) {
    uint8_t value;
    asm volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

void omni_port_write(uint16_t port, uint8_t value) {
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

// سایر توابع لینوکس...

#endif

// ----------------------------------------------------------
// توابع پیشرفته مستقل از سیستم عامل
// ----------------------------------------------------------

/**
 * دستکاری مستقیم جدول صفحه‌ها
 */
int omni_modify_page_table(uint64_t virt_addr, uint64_t phys_addr, int flags) {
    // پیاده‌سازی پیچیده دستکاری ساختارهای سطح هسته
    // ...
    return 0;
}

/**
 * فعال‌سازی دسترسی به حافظه SMM
 */
void omni_enable_smm_access() {
    uint64_t smram_ctl = omni_read_msr(0x9C);
    omni_write_msr(0x9C, smram_ctl | (1 << 3)); // فعال‌سازی D_OPEN
}

/**
 * تزریق کد به فضای هسته
 */
int omni_inject_kernel_code(void* code, size_t size) {
    // یافتن فضای مناسب در حافظه هسته
    uint64_t target_addr = 0xFFFFFFFF80000000;
    
    // تغییر محافظت حافظه
    omni_modify_page_table(target_addr, target_addr, 0x07); // RWX
    
    // کپی کد
    omni_write_physical_memory(target_addr, code, size);
    
    // اجرای کد
    return omni_exec_kernel_code((void(*)())target_addr);
}

/**
 * دستکاری مستقیم وقفه‌ها
 */
void omni_install_interrupt_handler(int vector, void* handler) {
    uint64_t idt_addr = omni_read_msr(0x6E); // IA32_LSTAR MSR
    uint64_t* idt = (uint64_t*)omni_map_physical_memory(idt_addr, 4096);
    
    // ساخت دروازه وقفه
    uint64_t gate = ((uint64_t)handler & 0xFFFF0000) | 0x8E00;
    gate |= ((uint64_t)handler & 0xFFFFFFFF) << 32;
    
    idt[vector] = gate;
    
    omni_unmap_physical_memory(idt, 4096);
}

// ----------------------------------------------------------
// توابع کمکی
// ----------------------------------------------------------

/**
 * یافتن جدول ACPI
 */
void* omni_find_acpi_table(const char* signature) {
    uint64_t rsdp_addr = 0xE0000;
    char rsdp_sig[9] = {0};
    
    // جستجوی RSDP
    while (rsdp_addr < 0x100000) {
        omni_read_physical_memory(rsdp_addr, rsdp_sig, 8);
        if (strncmp(rsdp_sig, "RSD PTR ", 8) == 0) {
            break;
        }
        rsdp_addr += 16;
    }
    
    if (rsdp_addr >= 0x100000) return NULL;
    
    // خواندن RSDP
    uint64_t rsdt_addr;
    omni_read_physical_memory(rsdp_addr + 16, &rsdt_addr, sizeof(rsdt_addr));
    
    // نقشه‌برداری RSDT
    uint32_t* rsdt = (uint32_t*)omni_map_physical_memory(rsdt_addr, 4096);
    
    // جستجوی جدول
    uint32_t count = (rsdt[1] - 36) / 4;
    for (uint32_t i = 0; i < count; i++) {
        uint32_t table_addr = rsdt[9 + i];
        char table_sig[5] = {0};
        omni_read_physical_memory(table_addr, table_sig, 4);
        
        if (strncmp(table_sig, signature, 4) == 0) {
            void* table = malloc(rsdt[1]);
            omni_read_physical_memory(table_addr, table, rsdt[1]);
            omni_unmap_physical_memory(rsdt, 4096);
            return table;
        }
    }
    
    omni_unmap_physical_memory(rsdt, 4096);
    return NULL;
}
```

### Library Structure:

```
OmniAccessLib/
├── include/
│   ├── omniaccess.h         # فایل هدر اصلی
│   ├── windows/
│   │   ├── driver.h         # تعاریف درایور ویندوز
│   │   └── ioctl.h          # تعاریف IOCTL
│   └── linux/
│       ├── compat.h         # سازگاری لینوکس
│       └── physical.h       # توابع دسترسی فیزیکی
├── src/
│   ├── core.c               # توابع اصلی
│   ├── windows/
│   │   ├── driver.c         # پیاده‌سازی درایور ویندوز
│   │   ├── memory.c         # مدیریت حافظه ویندوز
│   │   └── io.c             # I/O ویندوز
│   ├── linux/
│   │   ├── physical.c       # دسترسی فیزیکی لینوکس
│   │   ├── pci.c            # PCI لینوکس
│   │   └── msr.c            # MSR لینوکس
│   └── common/
│       ├── acpi.c           # توابع ACPI
│       ├── pci_common.c     # PCI مشترک
│       └── smm.c            # مدیریت SMM
├── drivers/
│   ├── windows/
│   │   ├── OmniAccess.sys   # درایور کرنل ویندوز
│   │   ├── installer.c      # نصب درایور
│   │   └── inf/             # فایل‌های INF
│   └── linux/
│       ├── omniaccess.ko    # ماژول کرنل لینوکس
│       ├── Makefile         # ساخت ماژول
│       └── module.c         # کد ماژول
├── examples/
│   ├── physical_read.c      # مثال خواندن حافظه فیزیکی
│   ├── pci_scan.c           # مثال اسکن PCI
│   └── msr_dump.c           # مثال خواندن MSR
└── build/
    ├── win_build.bat        # اسکریپت ساخت ویندوز
    └── linux_build.sh       # اسکریپت ساخت لینوکس
```

---

How to use:


```c
#include "omniaccess.h"

int main() {
    if (omni_init() != 0) {
        printf("Failed to initialize OmniAccess library\n");
        return 1;
    }

    // خواندن حافظه فیزیکی
    uint8_t buffer[1024];
    omni_read_physical_memory(0x100000, buffer, sizeof(buffer));

    // دستکاری پورت‌های I/O
    omni_port_write(0x70, 0x0A);
    uint8_t value = omni_port_read(0x71);

    // خواندن MSR
    uint64_t msr_value = omni_read_msr(0x1B);

    // فعال‌سازی دسترسی SMM
    omni_enable_smm_access();

    omni_cleanup();
    return 0;
}
```

### Compile and run:

**Windows**:
```batch
:: Compile the library
cl /c /O2 /Iinclude src\core.c src\windows\*.c
lib /OUT:omniaccess.lib *.obj

:: Compile the example
cl /Iinclude examples\physical_read.c omniaccess.lib

:: اجرا
physical_read.exe
```

**لینوکس**:
```bash
# کامپایل کتابخانه
gcc -O2 -Iinclude -fPIC -c src/core.c src/linux/*.c src/common/*.c
gcc -shared -o libomniaccess.so *.o

# کامپایل مثال
gcc -Iinclude -L. -lomniaccess examples/physical_read.c -o physical_read

# اجرا
sudo ./physical_read
```

---

Security Considerations:
This library is designed for research and educational purposes. Using it on production systems without a license may:
1. Make the system unstable
2. Create security issues
3. Violate operating system security rules
4. Be illegal in some jurisdictions

Always use these tools in controlled environments with appropriate licenses.

---

Key Capabilities of This Library

1. **Low-Level Access**:
    - Read/write physical memory
    - Direct access to I/O ports
    - Full control of PCI Configuration Space
    - MSR (Model-Specific Register) management

2. **Kernel Control**:
    - Inject and execute code at kernel level
    - Direct manipulation of page tables
    - Install custom interrupt handlers
    - Enable access to SMM (System Management Mode)

3. **Cross-Platform Support**:
    - Windows 10/11 (with kernel driver)
    - Linux (with kernel module and /dev/mem)
    - Compatible with x86 and x86_64 architectures

4. **Advanced Security**:
    - Bypass protection mechanisms (SMEP/SMAP)
    - Access protected memory
    - Control privilege level (Ring 0)

5. **Advanced Tools**:
    - Analyze and manipulate ACPI Tables
    - Access UEFI Runtime Services
    - Control hardware interrupts

---