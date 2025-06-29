; ماکروهای مشترک برای اسمبلی ویندوز (MASM)
OPTION CASEMAP:NONE
OPTION LITERALS:ON

; ماکرو برای تنظیم فریم پشته
PROLOGUE MACRO
    push rbp
    mov rbp, rsp
ENDM

; ماکرو برای بازگردانی فریم پشته
EPILOGUE MACRO
    mov rsp, rbp
    pop rbp
    ret
ENDM

; ماکرو برای فراخوانی توابع سیستمی
SYSCALL MACRO index
    mov r10, rcx
    mov eax, index
    syscall
ENDM