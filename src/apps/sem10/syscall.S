#include <asm/unistd_32.h>

    .text
    .global my_write
    .global my_write2
my_write:
    pushl %ebx
    pushl %ecx
    pushl %edx
    pushl %ebp
    mov %esp, %ebp
    andl $-16, %esp

    // syscall id: eax
    // syscall input: ebx, ecx, edx, esi, edi
    // syscall output: eax
    movl $__NR_write, %eax
    movl 20(%ebp), %ebx
    movl 24(%ebp), %ecx
    movl 28(%ebp), %edx
    int $0x80

    mov %ebp, %esp
    popl %ebp
    popl %edx
    popl %ecx
    popl %ebx

    ret

my_write2:
    pushl %ebp
    mov %esp, %ebp
    andl $-16, %esp
    subl $16, %esp
    
    movl 8(%ebp), %eax
    movl %eax, (%esp)
    movl 12(%ebp), %eax
    movl %eax, 4(%esp)
    movl 16(%ebp), %eax
    movl %eax, 8(%esp)
    call write

    mov %ebp, %esp
    popl %ebp
    ret
