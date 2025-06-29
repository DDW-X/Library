#include "asm_macros.inc"

.section .text
.global omni_asm_map_physical
.type omni_asm_map_physical, @function
omni_asm_map_physical:
    PROLOGUE
    # RDI = phys_addr, RSI = size
    mov rdx, rdi                # phys_addr
    mov rsi, rsi                # size
    mov rdi, 0                  # addr (NULL)
    mov rdx, 3                  # PROT_READ | PROT_WRITE
    mov r10, 0x22               # MAP_PRIVATE | MAP_ANONYMOUS
    mov r8, -1                  # fd (-1)
    mov r9, 0                   # offset
    mov rax, 9                  # sys_mmap
    syscall
    EPILOGUE

.global omni_asm_unmap_physical
.type omni_asm_unmap_physical, @function
omni_asm_unmap_physical:
    PROLOGUE
    # RDI = virt_addr, RSI = size
    mov rsi, rsi                # size
    mov rax, 11                 # sys_munmap
    syscall
    EPILOGUE

.global omni_asm_port_read
.type omni_asm_port_read, @function
omni_asm_port_read:
    movw %di, %dx
    inb %dx, %al
    ret

.global omni_asm_port_write
.type omni_asm_port_write, @function
omni_asm_port_write:
    movw %di, %dx     # port
    movb %sil, %al    # value
    outb %al, %dx
    ret

.global omni_asm_read_msr
.type omni_asm_read_msr, @function
omni_asm_read_msr:
    movl %edi, %ecx   # msr
    rdmsr              # EDX:EAX = MSR value
    shlq $32, %rdx
    orq %rdx, %rax
    ret

.global omni_asm_write_msr
.type omni_asm_write_msr, @function
omni_asm_write_msr:
    movl %edi, %ecx   # msr
    movq %rsi, %rax   # value (low)
    shrq $32, %rsi
    movl %esi, %edx   # value (high)
    wrmsr
    ret

.global omni_asm_invlpg
.type omni_asm_invlpg, @function
omni_asm_invlpg:
    invlpg (%rdi)
    ret

.global omni_asm_wbinvd
.type omni_asm_wbinvd, @function
omni_asm_wbinvd:
    wbinvd
    ret

.global omni_asm_clflush
.type omni_asm_clflush, @function
omni_asm_clflush:
    clflush (%rdi)
    ret

.global omni_asm_cpuid
.type omni_asm_cpuid, @function
omni_asm_cpuid:
    PROLOGUE
    # EDI = eax_in, ESI = ecx_in
    # RDX = eax_out, RCX = ebx_out
    # R8 = ecx_out, R9 = edx_out
    
    push %rbx
    push %r12
    
    mov %edi, %eax
    mov %esi, %ecx
    cpuid
    
    # ذخیره خروجی‌ها
    mov %eax, (%rdx)
    mov %ebx, (%rcx)
    mov %ecx, (%r8)
    mov %edx, (%r9)
    
    pop %r12
    pop %rbx
    EPILOGUE

.global omni_asm_rdtsc
.type omni_asm_rdtsc, @function
omni_asm_rdtsc:
    rdtsc
    mov %eax, (%rdi)
    mov %edx, (%rsi)
    ret

.global omni_asm_rdtscp
.type omni_asm_rdtscp, @function
omni_asm_rdtscp:
    rdtscp
    mov %eax, (%rdi)
    mov %edx, (%rsi)
    mov %ecx, (%rdx)
    ret

.global omni_asm_enable_sse
.type omni_asm_enable_sse, @function
omni_asm_enable_sse:
    mov %cr0, %rax
    and $0xFFFB, %ax      # Clear CR0.EM
    or $0x2, %ax          # Set CR0.MP
    mov %rax, %cr0
    
    mov %cr4, %rax
    or $0x600, %ax        # Set CR4.OSFXSR | CR4.OSXMMEXCPT
    mov %rax, %cr4
    ret

.global omni_asm_enable_avx
.type omni_asm_enable_avx, @function
omni_asm_enable_avx:
    call omni_asm_enable_sse
    
    # فعال‌سازی XSAVE
    mov %cr4, %rax
    or $0x040000, %eax    # Set CR4.OSXSAVE
    mov %rax, %cr4
    
    # فعال‌سازی AVX در XCR0
    xor %rcx, %rcx
    xgetbv
    or $0x7, %eax         # Set XCR0.SSE | XCR0.AVX | XCR0.OPMASK
    xsetbv
    
    ret