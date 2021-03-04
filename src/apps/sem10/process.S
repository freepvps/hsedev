    .data
ZERO:
    .long 0
    .long 0

    .text
    .global dot_product
dot_product:
    pushl %ebx
    pushl %ecx
    pushl %ebp
    mov %esp, %ebp
    andl $-16, %esp
    subl $16, %esp

    movl 16(%ebp), %ecx  // N

    movl 20(%ebp), %eax  // a
    movl 24(%ebp), %ebx  // b

    movss ZERO, %xmm2  // ans

main_loop_4:
    cmp $4, %ecx
    jl main_loop_end_4

    movups (%eax), %xmm0  // unaligned mov packed single
    movups (%ebx), %xmm1
    mulps %xmm0, %xmm1  // mul packed single
    movaps %xmm1, (%esp)  // aligned mov packed single
    addss (%esp), %xmm2  // add single single
    addss 4(%esp), %xmm2
    addss 8(%esp), %xmm2
    addss 12(%esp), %xmm2

    subl $4, %ecx
    lea 16(%eax), %eax
    lea 16(%ebx), %ebx
    jmp main_loop_4
main_loop_end_4:

main_loop_1:
    cmpl $1, %ecx
    jl main_loop_end_1

    movss (%eax), %xmm0  // mov single single
    movss (%ebx), %xmm1
    mulss %xmm0, %xmm1  // mul single single
    addss %xmm1, %xmm2  // add single single

    subl $1, %ecx
    lea 4(%eax), %eax
    lea 4(%ebx), %ebx
    jmp main_loop_1
main_loop_end_1:

    // mov float from %xmm2 to coprocessor x87 stack
    movss %xmm2, (%esp)
    flds (%esp)

    mov %ebp, %esp
    popl %ebp
    popl %ecx
    popl %ebx

    ret
