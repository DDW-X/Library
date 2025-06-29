// pci_scan.c - Placeholder content
#include "omniaccess.h"
#include <stdio.h>
#include <inttypes.h>

int main() {
    if (omni_init() != 0) {
        fprintf(stderr, "Failed to initialize OmniAccess library\n");
        return 1;
    }

    printf("Scanning PCI devices...\n");
    printf("Bus | Device | Func | Vendor ID | Device ID | Class\n");
    printf("--------------------------------------------------\n");
    
    for (int bus = 0; bus < 256; bus++) {
        for (int device = 0; device < 32; device++) {
            for (int function = 0; function < 8; function++) {
                uint32_t vendor_device = omni_pci_read(bus, device, function, 0);
                uint16_t vendor_id = vendor_device & 0xFFFF;
                uint16_t device_id = vendor_device >> 16;
                
                if (vendor_id != 0xFFFF) {
                    uint32_t class_rev = omni_pci_read(bus, device, function, 8);
                    uint8_t class_code = (class_rev >> 24) & 0xFF;
                    uint8_t subclass = (class_rev >> 16) & 0xFF;
                    
                    printf("%3d | %6d | %4d | 0x%04X   | 0x%04X   | 0x%02X.%02X\n", 
                           bus, device, function, vendor_id, device_id, class_code, subclass);
                }
            }
        }
    }
    
    omni_cleanup();
    return 0;
}