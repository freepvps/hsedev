SC: .asciz "%lf%lf%lf%lf"
PR: .asciz "%.10g\n"

    .data
ZERO:
    .long 0
    .long 0

    .text
    .global main

write_value:  // non-cdecl
    subl $12, %esp
    movsd %xmm0, 4(%esp)
    movl $PR, (%esp)
    call printf
    addl $12, %esp
    ret

maximum:  // non-cdecl, magic fastcall(eax, ecx)
    comisd %xmm0, %xmm1
    jbe maximum_end
    movsd %xmm1, %xmm0
maximum_end:
    ret

minimum:  // non-cdecl, magic fastcall(eax, ecx)
    comisd %xmm0, %xmm1
    jae minimum_end
    movsd %xmm1, %xmm0
minimum_end:
    ret

main:
    pushl %ebp
    pushl %ebx
    pushl %edi
    pushl %esi
    mov %esp, %ebp
    andl $-16, %esp
    subl $64, %esp

loop_begin:
    // SCANF
    movl $SC, (%esp)
    lea 20(%esp), %eax  // l1
    mov %eax, 4(%esp)
    lea 28(%esp), %eax  // r1
    mov %eax, 8(%esp)
    lea 36(%esp), %eax  // l2
    mov %eax, 12(%esp)
    lea 44(%esp), %eax  // r2
    mov %eax, 16(%esp)
    call scanf
    cmp $4, %eax
    jnz loop_end

    // xmm2=MAX(l1, l2)
    movsd 20(%esp), %xmm0
    movsd 36(%esp), %xmm1
    call maximum
    movsd %xmm0, %xmm2

    // xmm0=MIN(r1, r2)
    movsd 28(%esp), %xmm0
    movsd 44(%esp), %xmm1
    call minimum

    // xmm0=MAX(xmm0 - xmm2, 0)
    subsd %xmm2, %xmm0
    movsd ZERO, %xmm1
    maxsd %xmm1, %xmm0

    // // PRINTF
    call write_value

    // STEP
    jmp loop_begin
loop_end:

    xor %eax, %eax
    mov %ebp, %esp
    popl %esi
    popl %edi
    popl %ebx
    popl %ebp
    ret
