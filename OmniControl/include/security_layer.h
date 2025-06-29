#pragma once
#include <stdint.h>
#include <stdbool.h>

// سطوح امنیتی
typedef enum {
    SEC_LEVEL_NORMAL,
    SEC_LEVEL_ENHANCED,
    SEC_LEVEL_HIGH,
    SEC_LEVEL_PARANOID
} SecurityLevel;

// API امنیتی
void seclayer_enable_protection(SecurityLevel level);
void seclayer_disable_protection();
bool seclayer_validate_memory_integrity(void* address, size_t size);
bool seclayer_detect_tampering();
void seclayer_encrypt_data(void* data, size_t size, const char* key);
void seclayer_decrypt_data(void* data, size_t size, const char* key);