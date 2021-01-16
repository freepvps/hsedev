SC: .asciz "%d%d"
PR: .asciz "%d\n"

    .text
    .global main
main:
    pushl %ebp
    mov %esp, %ebp
    andl $-16, %esp
    subl $16, %esp

    movl 8(%ebp), %eax
    movl %eax, 4(%esp)
    movl $PR, (%esp)
    call printf

    xor %eax, %eax
    mov %ebp, %esp
    popl %ebp
    ret
