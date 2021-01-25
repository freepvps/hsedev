PR: .asciz "%d %d\n"

    .text
    .global main
main:
    pushl %ebp
    mov %esp, %ebp
    andl $-16, %esp
    subl $16, %esp

    movl 8(%ebp), %eax
    movl %eax, 4(%esp)

    // PTR COMPUTATION
    // Format: [OFF_CONST](BASE_REG[, INDEX_REG[, SCALE_CONST]])
    // Format: OFF_CONST
    // OFF + BASE + INDEX * SCALE
    movl $1, %eax
    movl $2, %edx
    lea 3(%eax, %edx, 8), %eax  // 3 + eax + edx * 8 = 3 + 1 + 2 * 8 = 4 + 16 = 20
    lea 123, %eax  // movl $123, %eax

    movl $1, %edx
    movl %eax, 4(%esp, %edx, 4)  // ((int32*)(4 + esp))[edx] = eax
    // movl 40, %edx  // *((int32_t*)(40))
    // END

    movl $PR, (%esp)
    call printf

    xor %eax, %eax
    mov %ebp, %esp
    popl %ebp
    ret
