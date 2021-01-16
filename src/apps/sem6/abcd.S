PR: .asciz "%d\n"
    .data
    .global A
    .global B
    .global C
    .global D
    .global F

A: .int 3
B: .int 2
C: .int 3
D: .int 4
R: .int 5

    .text
    .global process
    .global main

real_multiply:
    imull %edx
    ret

multiply:
    jmp real_multiply

process:  // cdecl call
    pushl %ebp
    mov %esp, %ebp
    andl $-16, %esp

    movl (C), %eax
    movl (D), %edx
    call multiply
    pushl %eax

    movl (A), %eax
    movl (B), %edx
    call multiply
    popl %edx
    subl %edx, %eax

    movl %eax, (R)

    mov %ebp, %esp
    popl %ebp
    ret

main:
    pushl %ebp
    mov %esp, %ebp
    andl $-16, %esp
    subl $16, %esp

    call process

    movl %eax, 4(%esp)
    movl $PR, (%esp)
    call printf

    xor %eax, %eax
    mov %ebp, %esp
    popl %ebp
    ret
