SC: .asciz "%d%d"
PR: .asciz "%d\n"

    .text
    .global main
main:
    pushl %ebp
    mov %esp, %ebp
    andl $-16, %esp
    subl $32, %esp

    movl $SC, (%esp)  // "format"

    lea 12(%esp), %eax  // "value1 ptr"
    movl %eax, 4(%esp)

    lea 16(%esp), %eax
    movl %eax, 8(%esp)
    call scanf

    movl 12(%esp), %eax
    addl 16(%esp), %eax

    movl %eax, 4(%esp)
    movl $PR, (%esp)
    call printf

    xor %eax, %eax
    mov %ebp, %esp
    popl %ebp
    ret
