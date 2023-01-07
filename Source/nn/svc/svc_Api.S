.arm
.align 4

.macro SVC_BEGIN name
    .section .text.\name, "ax", %progbits
    .global \name
    .type \name, %function
    .align 2
    .cfi_startproc
\name:
.endm

.macro SVC_END
    .cfi_endproc
.endm

SVC_BEGIN _ZN2nn3svc17OutputDebugStringEPKvi
    svc 0x3d
    bx lr
SVC_END
