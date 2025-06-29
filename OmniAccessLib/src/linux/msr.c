// msr.c - Placeholder content
#include "linux/compat.h"
#include <asm/msr.h>

#ifdef __KERNEL__

uint64_t linux_read_msr(uint32_t msr) {
    uint64_t msr_value;
    rdmsrl(msr, msr_value);
    return msr_value;
}

void linux_write_msr(uint32_t msr, uint64_t value) {
    wrmsrl(msr, value);
}

#else

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

uint64_t linux_read_msr(uint32_t msr) {
    uint32_t low, high;
    asm volatile("rdmsr" : "=a"(low), "=d"(high) : "c"(msr));
    return ((uint64_t)high << 32) | low;
}

void linux_write_msr(uint32_t msr, uint64_t value) {
    uint32_t low = value & 0xFFFFFFFF;
    uint32_t high = value >> 32;
    asm volatile("wrmsr" : : "a"(low), "d"(high), "c"(msr));
}

#endif