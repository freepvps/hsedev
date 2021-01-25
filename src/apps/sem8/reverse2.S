SC: .asciz "%d"
PR: .asciz "%d\n"

    .text
    .global main

read_int32:  // non-cdecl
    subl $12, %esp

    leal 8(%esp), %eax

    movl %eax, 4(%esp)
    movl $SC, (%esp)
    call scanf
    movl 8(%esp), %eax

    addl $12, %esp
    ret

write_int32:  // non-cdecl
    subl $12, %esp
    movl %eax, 4(%esp)
    movl $PR, (%esp)
    call printf
    addl $12, %esp
    ret

main:
    pushl %ebp
    pushl %ebx
    pushl %esi
    mov %esp, %ebp
    andl $-16, %esp
    subl $16, %esp

    call read_int32

    movl %eax, %ebx
    lea (%eax, %eax), %eax
    lea (%eax, %eax), %eax
    addl $16, %eax
    andl $-16, %eax
    subl %eax, %esp

    // esi - index
    // ebx - elements count
    // esp - memory

    xor %esi, %esi  // esi = 0

read_loop:
    cmp %esi, %ebx  // set flags for (esi - ebx)
    jz read_loop_end
    call read_int32
    movl %eax, (%esp, %esi, 4)  // *(esp + esi * 4) = eax
    inc %esi
    jmp read_loop
read_loop_end:
    movl %esi, %eax
write_loop:
    test %esi, %esi  // set flags for (esi & esi)
    jz write_loop_end
    dec %esi

    movl (%esp, %esi, 4), %eax
    call write_int32
    jmp write_loop
write_loop_end:

    xor %eax, %eax
    mov %ebp, %esp
    popl %esi
    popl %ebx
    popl %ebp
    ret
