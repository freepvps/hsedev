SC: .asciz "%d"
PR: .asciz "%d\n"

#define F(A, B) movl %A, %B

    .text
    .global main
main:
    pushl %ebp
    mov %esp, %ebp
    andl $-16, %esp
    subl $32, %esp

    movl $SC, (%esp)

    //movl %esp, %eax
    F(esp, eax)
    addl $8, %eax
    //lea 8(%esp), %eax
    movl %eax, 4(%esp)
    call scanf
    movl 8(%esp), %eax
    movl %eax, 4(%esp)

main_loop:
    movl 8(%esp), %eax
    cmp $0, %eax
    jz main_end

    movl %eax, 4(%esp)
    movl $PR, (%esp)
    subl $1, %eax
    movl %eax, 8(%esp)
    call printf
    jmp main_loop
main_end:

    xor %eax, %eax
    mov %ebp, %esp
    popl %ebp
    ret
