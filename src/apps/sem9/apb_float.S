SC: .string "%f%f"
PR: .string "%.10g\n"

    .text
    .global main

main:
    pushl %ebp
    mov %esp, %ebp
    andl $-16, %esp
    subl $48, %esp

    lea 12(%esp), %eax
    lea 16(%esp), %ebx
    movl %ebx, 8(%esp)
    movl %eax, 4(%esp)
    movl $SC, (%esp)
    call scanf

    movss 12(%esp), %xmm0
    movss 16(%esp), %xmm1

    addss %xmm1, %xmm0  // SSE
    cvtss2sd %xmm0, %xmm0
    movsd %xmm0, 4(%esp)
    movl $PR, (%esp)
    call printf

    mov %ebp, %esp
    popl %ebp
    xor %eax, %eax
    ret
