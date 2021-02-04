SC: .asciz "%d%d%d%d"
PR: .asciz "%d\n"

    .text
    .global main

write_int32:  // non-cdecl
    subl $12, %esp
    movl %eax, 4(%esp)
    movl $PR, (%esp)
    call printf
    addl $12, %esp
    ret

max_int32:  // non-cdecl, magic fastcall(eax, ecx)
    cmp %eax, %ecx
    jl max_int32_end
    mov %ecx, %eax
max_int32_end:
    ret

min_int32:  // non-cdecl, magic fastcall(eax, ecx)
    cmp %eax, %ecx
    jg min_int32_end
    mov %ecx, %eax
min_int32_end:
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
    lea 24(%esp), %eax  // r1
    mov %eax, 8(%esp)
    lea 28(%esp), %eax  // l2
    mov %eax, 12(%esp)
    lea 32(%esp), %eax  // r2
    mov %eax, 16(%esp)
    call scanf
    cmp $4, %eax
    jnz loop_end

    // ebx=MAX(l1, l2)
    mov 20(%esp), %eax
    mov 28(%esp), %ecx
    call max_int32
    movl %eax, %ebx

    // eax=MIN(r1, r2)
    mov 24(%esp), %eax
    mov 32(%esp), %ecx
    call min_int32

    // eax=MAX(eax - ebx, 0)
    subl %ebx, %eax
    mov $0, %ecx
    call max_int32

    // PRINTF
    call write_int32

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
