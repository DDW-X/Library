#include "omniasm.h"
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define ITERATIONS 10000000

void test_port_io() {
    clock_t start = clock();
    for (int i = 0; i < ITERATIONS; i++) {
        omni_asm_port_write(0x80, i & 0xFF);
        uint8_t val = omni_asm_port_read(0x80);
        (void)val; // Suppress unused warning
    }
    double duration = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf("Port I/O: %.2f ns/op\n", (duration * 1e9) / ITERATIONS);
}

void test_msr() {
    uint64_t sum = 0;
    clock_t start = clock();
    for (int i = 0; i < ITERATIONS; i++) {
        sum += omni_asm_read_msr(0x10); // MSR_TIME_STAMP_COUNTER
    }
    double duration = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf("MSR Read: %.2f ns/op (sum=%llu)\n", (duration * 1e9) / ITERATIONS, sum);
}

void test_cpuid() {
    uint32_t eax, ebx, ecx, edx;
    clock_t start = clock();
    for (int i = 0; i < ITERATIONS; i++) {
        omni_asm_cpuid(0, 0, &eax, &ebx, &ecx, &edx);
    }
    double duration = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf("CPUID: %.2f ns/op\n", (duration * 1e9) / ITERATIONS);
}

void test_tsc() {
    uint32_t eax, edx, ecx;
    uint64_t sum = 0;
    clock_t start = clock();
    for (int i = 0; i < ITERATIONS; i++) {
        omni_asm_rdtscp(&eax, &edx, &ecx);
        sum += ((uint64_t)edx << 32) | eax;
    }
    double duration = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf("RDTSCP: %.2f ns/op (sum=%llu)\n", (duration * 1e9) / ITERATIONS, sum);
}

int main() {
    printf("Performance Tests (x%d iterations):\n", ITERATIONS);
    test_port_io();
    test_msr();
    test_cpuid();
    test_tsc();
    
    // Test AVX enablement
    omni_asm_enable_avx();
    printf("AVX extensions enabled\n");
    
    return 0;
}