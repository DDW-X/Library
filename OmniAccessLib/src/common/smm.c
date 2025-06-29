// smm.c - Placeholder content
#include "omniaccess.h"

void omni_enter_smm() {
    // Write to SMI command port
    omni_port_write(0xB2, 0xDE);
}

void omni_enable_smm_access() {
    // Get SMRAM control
    uint64_t smram_ctl = omni_read_msr(0x9C);
    
    // Enable D_OPEN bit
    smram_ctl |= (1 << 3);
    
    // Write back to SMRAM control
    omni_write_msr(0x9C, smram_ctl);
}

int omni_access_smm_memory(uint64_t smram_base, void* buffer, size_t size, int write) {
    // First enable SMM access
    omni_enable_smm_access();
    
    // Map SMRAM
    void* mapped = omni_map_physical_memory(smram_base, size);
    if (!mapped) return -1;
    
    // Access memory
    if (write) {
        memcpy(mapped, buffer, size);
    } else {
        memcpy(buffer, mapped, size);
    }
    
    // Unmap and clean up
    omni_unmap_physical_memory(mapped, size);
    return 0;
}