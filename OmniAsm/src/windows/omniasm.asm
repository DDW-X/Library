include windows/asm_macros.inc
includelib kernel32.lib

.CODE

; void* omni_asm_map_physical(uint64_t phys_addr, size_t size)
omni_asm_map_physical PROC
    PROLOGUE
    ; RCX = phys_addr, RDX = size
    mov r9, rdx                  ; size
    mov r8, 4                    ; PAGE_READWRITE
    mov rdx, 00300000h           ; MEM_MAP_PHYSICAL (flag custom)
    mov rcx, rcx                 ; phys_addr
    call VirtualAlloc
    EPILOGUE
omni_asm_map_physical ENDP

; void omni_asm_unmap_physical(void* virt_addr, size_t size)
omni_asm_unmap_physical PROC
    PROLOGUE
    ; RCX = virt_addr, RDX = size
    mov r8, rdx                 ; size
    mov rdx, 0                  ; MEM_RELEASE
    call VirtualFree
    EPILOGUE
omni_asm_unmap_physical ENDP

; uint8_t omni_asm_port_read(uint16_t port)
omni_asm_port_read PROC
    mov dx, cx     ; port
    in al, dx
    ret
omni_asm_port_read ENDP

; void omni_asm_port_write(uint16_t port, uint8_t value)
omni_asm_port_write PROC
    mov dx, cx     ; port
    mov al, dl     ; value
    out dx, al
    ret
omni_asm_port_write ENDP

; uint64_t omni_asm_read_msr(uint32_t msr)
omni_asm_read_msr PROC
    mov ecx, ecx   ; msr
    rdmsr          ; EDX:EAX = MSR value
    shl rdx, 32
    or rax, rdx
    ret
omni_asm_read_msr ENDP

; void omni_asm_write_msr(uint32_t msr, uint64_t value)
omni_asm_write_msr PROC
    mov r8, rdx    ; value (high)
    mov ecx, ecx   ; msr
    mov eax, edx   ; value (low)
    mov edx, r8d   ; value (high)
    wrmsr
    ret
omni_asm_write_msr ENDP

; void omni_asm_invlpg(void* addr)
omni_asm_invlpg PROC
    invlpg [rcx]
    ret
omni_asm_invlpg ENDP

; void omni_asm_wbinvd()
omni_asm_wbinvd PROC
    wbinvd
    ret
omni_asm_wbinvd ENDP

; void omni_asm_clflush(void* addr)
omni_asm_clflush PROC
    clflush [rcx]
    ret
omni_asm_clflush ENDP

; void omni_asm_cpuid(uint32_t eax_in, uint32_t ecx_in, uint32_t* eax_out, uint32_t* ebx_out, uint32_t* ecx_out, uint32_t* edx_out)
omni_asm_cpuid PROC
    PROLOGUE
    push rbx
    push rsi
    
    ; پارامترها:
    ; ECX = eax_in
    ; EDX = ecx_in
    ; R8 = eax_out
    ; R9 = ebx_out
    ; [rbp+48] = ecx_out
    ; [rbp+56] = edx_out
    
    mov eax, ecx
    mov ecx, edx
    cpuid
    
    ; ذخیره خروجی‌ها
    mov [r8], eax
    mov [r9], ebx
    
    mov rsi, [rbp+48]
    mov [rsi], ecx
    
    mov rsi, [rbp+56]
    mov [rsi], edx
    
    pop rsi
    pop rbx
    EPILOGUE
omni_asm_cpuid ENDP

; void omni_asm_rdtsc(uint32_t* eax_out, uint32_t* edx_out)
omni_asm_rdtsc PROC
    rdtsc
    mov [rcx], eax
    mov [rdx], edx
    ret
omni_asm_rdtsc ENDP

; void omni_asm_rdtscp(uint32_t* eax_out, uint32_t* edx_out, uint32_t* ecx_out)
omni_asm_rdtscp PROC
    rdtscp
    mov [rcx], eax
    mov [rdx], edx
    mov [r8], ecx
    ret
omni_asm_rdtscp ENDP

; void omni_asm_enable_sse()
omni_asm_enable_sse PROC
    mov rax, cr0
    and ax, 0FFFBh      ; Clear CR0.EM
    or ax, 2            ; Set CR0.MP
    mov cr0, rax
    
    mov rax, cr4
    or ax, 600h         ; Set CR4.OSFXSR | CR4.OSXMMEXCPT
    mov cr4, rax
    ret
omni_asm_enable_sse ENDP

; void omni_asm_enable_avx()
omni_asm_enable_avx PROC
    ; فعال‌سازی SSE (پیش‌نیاز)
    call omni_asm_enable_sse
    
    ; فعال‌سازی XSAVE
    mov rax, cr4
    or eax, 040000h     ; Set CR4.OSXSAVE
    mov cr4, rax
    
    ; فعال‌سازی AVX در XCR0
    xor rcx, rcx
    xgetbv
    or eax, 7           ; Set XCR0.SSE | XCR0.AVX | XCR0.OPMASK
    xsetbv
    
    ret
omni_asm_enable_avx ENDP

END