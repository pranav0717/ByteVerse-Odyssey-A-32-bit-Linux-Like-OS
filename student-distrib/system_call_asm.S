
.text
.global syscall_asm
# .global returnFromHalt

syscall_table:
    .long 0x0, halt, execute, read, write, open, close, getargs, vidmap, set_handler, sigreturn

 #
 # syscall_asm
 #   DESCRIPTION: caller save unused registers and check syscall number to call with eax.
 #                  push edx, ebx, and ecx as parameters.
 #                call system call handlers through jumptable.
 #   INPUTS: eax -- system call number 1-10
 #             edx -- param 3
 #             ecx -- param 2
 #             ebx -- param 1
 #   OUTPUTS: same as corresponding jumptable function outputs
 #   RETURN VALUE: same as corresponding jumptable function outputs OR -1 (fail out of bounds)
 #   SIDE EFFECTS: calls system call handlers
 #
 #   FOR SYSCALL HANDLER INTERFACES CHECK SYSTEM_CALLS.C
 #
 

syscall_asm:

    pushl   %ecx
    pushl   %edx
    pushl   %ebx
    pushl   %esp
    pushl   %ebp
    pushl   %esi
    pushl   %edi
    pushfl

    pushl %edx # push args
    pushl %ecx
    pushl %ebx

    cmpl $1, %eax # check bounds
    jl inval
    cmpl $10, %eax # 10 is max
    jg inval

    call *syscall_table(, %eax, 4);

    done:
    addl    $12, %esp 
    popfl
    popl    %edi
    popl    %esi
    popl    %ebp
    popl    %esp
    popl    %ebx
    popl    %edx
    popl    %ecx
    iret

    inval:
    movl $-1, %eax
    jmp done


# returnFromHalt
#   DESCRIPTION: Returns from a halt state and restores the stack and registers.
#   INPUTS: 
#     - esp: A pointer to the new stack pointer.
#     - ebp: The new base pointer.
#     - status: The status value to return.
#   OUTPUTS: None
#   RETURN VALUE: None
#   SIDE EFFECTS: Restores the stack and registers.

# returnFromHalt:
#     pushl %ebp
#     movl %esp, %ebp
# 
#     movl 8(%ebp), %esp          #esp    arg 1
#     movl 16(%ebp), %eax         #status arg 3
#     movl 12(%ebp), %ebp         #ebp    arg 2
# 
#     leave
#     ret


