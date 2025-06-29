#pragma once
#include <stdint.h>
#include <stddef.h>

#ifdef _WIN32
#define OMNIASM_ABI __cdecl
#else
#define OMNIASM_ABI
#endif

// توابع اسمبلی
#ifdef __cplusplus
extern "C" {
#endif

void* OMNIASM_ABI omni_asm_map_physical(uint64_t phys_addr, size_t size);
void OMNIASM_ABI omni_asm_unmap_physical(void* virt_addr, size_t size);
uint8_t OMNIASM_ABI omni_asm_port_read(uint16_t port);
void OMNIASM_ABI omni_asm_port_write(uint16_t port, uint8_t value);
uint64_t OMNIASM_ABI omni_asm_read_msr(uint32_t msr);
void OMNIASM_ABI omni_asm_write_msr(uint32_t msr, uint64_t value);
void OMNIASM_ABI omni_asm_invlpg(void* addr);
void OMNIASM_ABI omni_asm_wbinvd();
void OMNIASM_ABI omni_asm_clflush(void* addr);
void OMNIASM_ABI omni_asm_cpuid(uint32_t eax_in, uint32_t ecx_in, uint32_t* eax_out, uint32_t* ebx_out, uint32_t* ecx_out, uint32_t* edx_out);
void OMNIASM_ABI omni_asm_rdtsc(uint32_t* eax_out, uint32_t* edx_out);
void OMNIASM_ABI omni_asm_rdtscp(uint32_t* eax_out, uint32_t* edx_out, uint32_t* ecx_out);
void OMNIASM_ABI omni_asm_enable_sse();
void OMNIASM_ABI omni_asm_enable_avx();

#ifdef __cplusplus
}
#endif