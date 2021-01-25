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
    pushl %edi
    pushl %esi
    mov %esp, %ebp
    andl $-16, %esp
    subl $16, %esp

    call read_int32

    movl %eax, %ebx
    lea (%eax, %eax), %eax
    lea (%eax, %eax), %eax
    movl %eax, (%esp)
    call malloc
    mov %eax, %edi

    // esi - index
    // ebx - elements count
    // edi - memory

    xor %esi, %esi  // esi = 0

read_loop:
    cmp %esi, %ebx  // set flags for (esi - ebx)
    jz read_loop_end
    call read_int32
    movl %eax, (%edi, %esi, 4)  // *(edi + esi * 4) = eax
    inc %esi
    jmp read_loop
read_loop_end:
    movl %esi, %eax
write_loop:
    test %esi, %esi  // set flags for (esi & esi)
    jz write_loop_end
    dec %esi

    movl (%edi, %esi, 4), %eax
    call write_int32
    jmp write_loop
write_loop_end:

    movl %edi, (%esp)
    call free

    xor %eax, %eax
    mov %ebp, %esp
    popl %esi
    popl %edi
    popl %ebx
    popl %ebp
    ret
