/* ماکروهای مشترک برای اسمبلی لینوکس (GAS) */
.macro PROLOGUE
    pushq %rbp
    movq %rsp, %rbp
.endm

.macro EPILOGUE
    movq %rbp, %rsp
    popq %rbp
    ret
.endm

.macro SYSCALL num
    movq \num, %rax
    syscall
.endm