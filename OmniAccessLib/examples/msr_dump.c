// msr_dump.c - Placeholder content
#include "omniaccess.h"
#include <stdio.h>
#include <inttypes.h>

int main(int argc, char* argv[]) {
    if (omni_init() != 0) {
        fprintf(stderr, "Failed to initialize OmniAccess library\n");
        return 1;
    }

    uint32_t msr;
    if (argc > 1) {
        msr = (uint32_t)strtoul(argv[1], NULL, 16);
    } else {
        printf("Enter MSR address (hex): ");
        scanf("%x", &msr);
    }

    uint64_t value = omni_read_msr(msr);
    printf("MSR 0x%08X: 0x%016" PRIx64 "\n", msr, value);
    
    // Decode common MSRs
    switch (msr) {
        case 0x1B: // APIC_BASE
            printf("APIC Base Address: 0x%016" PRIx64 "\n", value & 0xFFFFF000);
            printf("APIC Global Enable: %s\n", (value & 0x800) ? "Enabled" : "Disabled");
            break;
            
        case 0xC0000080: // EFER
            printf("System Call Extensions (SCE): %s\n", (value & 1) ? "Enabled" : "Disabled");
            printf("Long Mode Enable (LME): %s\n", (value & 0x100) ? "Enabled" : "Disabled");
            printf("No-Execute Enable (NXE): %s\n", (value & 0x800) ? "Enabled" : "Disabled");
            break;
            
        case 0x3A: // FEATURE_CONTROL
            printf("Lock Bit: %s\n", (value & 1) ? "Locked" : "Unlocked");
            printf("VMXON in SMX: %s\n", (value & 4) ? "Enabled" : "Disabled");
            printf("VMXON outside SMX: %s\n", (value & 2) ? "Enabled" : "Disabled");
            break;
    }
    
    omni_cleanup();
    return 0;
}