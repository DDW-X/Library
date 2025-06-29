// physical_read.c - Placeholder content
#include "omniaccess.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

int main() {
    if (omni_init() != 0) {
        fprintf(stderr, "Failed to initialize OmniAccess library\n");
        return 1;
    }

    uint64_t address;
    size_t size;
    
    printf("Enter physical address (hex): ");
    scanf("%" SCNx64, &address);
    
    printf("Enter size (decimal): ");
    scanf("%zu", &size);
    
    unsigned char* buffer = (unsigned char*)malloc(size);
    if (!buffer) {
        fprintf(stderr, "Memory allocation failed\n");
        omni_cleanup();
        return 1;
    }
    
    if (omni_read_physical_memory(address, buffer, size) != 0) {
        fprintf(stderr, "Failed to read physical memory\n");
    } else {
        printf("\nMemory dump at 0x%016" PRIx64 ":\n", address);
        for (size_t i = 0; i < size; i++) {
            if (i % 16 == 0) {
                printf("\n%08zx: ", i);
            }
            printf("%02X ", buffer[i]);
        }
        printf("\n");
    }
    
    free(buffer);
    omni_cleanup();
    return 0;
}