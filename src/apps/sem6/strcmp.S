S1:  .string "Hello, World: %d\n"
S2:  .string "Hello, World: %d\n"
PR:  .string "%d\n"

    .text
    .global main

mystrcmp:
    pushl %ebp
    pushl %ebx
    mov %esp, %ebp

    movl 12(%ebp), %eax
    movl 16(%ebp), %ebx

CMP:
    movb (%eax), %cl
    movb (%ebx), %dl

    cmpb %cl, %dl
    jb CMPB
    ja CMPA

    testb %cl, %cl
    jz EQ

    inc %eax
    inc %ebx
    jmp CMP

CMPB:
    mov $1, %eax
    jmp ENDING
CMPA:
    mov $-1, %eax
    jmp ENDING
EQ:
    xor %eax, %eax
ENDING:

    mov %ebp, %esp
    popl %ebx
    popl %ebp
    ret

main:
    pushl %ebp
    mov %esp, %ebp
    subl $28, %esp

    movl $S2, 4(%esp)
    movl $S1, (%esp)
    call mystrcmp

    movl %eax, 4(%esp)
    movl $PR, (%esp)
    call printf

    addl $28, %esp
    mov %ebp, %esp
    popl %ebp
    ret
