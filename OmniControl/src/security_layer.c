#include "security_layer.h"
#include <openssl/aes.h>
#include <openssl/sha.h>

// اعتبارسنجی حافظه با الگوریتم SHA-256
bool seclayer_validate_memory_integrity(void* address, size_t size) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, address, size);
    SHA256_Final(hash, &sha256);
    
    // مقایسه با هش قبلی (در عمل باید ذخیره شود)
    unsigned char reference[SHA256_DIGEST_LENGTH] = {0};
    return memcmp(hash, reference, SHA256_DIGEST_LENGTH) == 0;
}

// رمزنگاری AES-256 پیشرفته
void seclayer_encrypt_data(void* data, size_t size, const char* key) {
    AES_KEY aes_key;
    AES_set_encrypt_key((const unsigned char*)key, 256, &aes_key);
    
    size_t blocks = size / AES_BLOCK_SIZE;
    for (size_t i = 0; i < blocks; i++) {
        AES_encrypt(
            (const unsigned char*)data + i * AES_BLOCK_SIZE,
            (unsigned char*)data + i * AES_BLOCK_SIZE,
            &aes_key
        );
    }
}

// تشخیص دستکاری سیستم
bool seclayer_detect_tampering() {
    // بررسی تغییرات در حافظه حیاتی سیستم
    if (!seclayer_validate_memory_integrity((void*)0x1000, 1024)) 
        return true;
    
    // بررسی تغییرات در جدول وقفه‌ها
    if (!seclayer_validate_memory_integrity((void*)0x2000, 4096)) 
        return true;
    
    // بررسی یکپارچگی درایورها
    if (!seclayer_validate_memory_integrity((void*)0x8000, 8192)) 
        return true;
    
    return false;
}