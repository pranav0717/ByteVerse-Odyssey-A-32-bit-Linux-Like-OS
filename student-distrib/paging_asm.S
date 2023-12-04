#define ASM     1
#include "paging.h"

# loadPageDirectory
#   DESCRIPTION: Loads the page directory base address into the CR3 register, enabling paging.
#   INPUTS: 
#     - page_directory: A pointer to the page directory to be loaded.
#   OUTPUTS: None
#   RETURN VALUE: None
#   SIDE EFFECTS: Loads the page directory and enables paging.
.globl loadPageDirectory
.align 4
loadPageDirectory:
push %ebp
mov %esp, %ebp
mov 8(%ebp), %eax
mov %eax, %cr3
leave
ret

# otherenable
#   DESCRIPTION: Enables additional CPU features for kernel operation.
#   INPUTS: None
#   OUTPUTS: None
#   RETURN VALUE: None
#   SIDE EFFECTS: Enables features such as PAE and paging in the CPU.

.globl  otherenable
.align 4
otherenable:
push %ebp
mov %esp, %ebp
mov %cr4, %eax
or $0x00000010, %eax # Enabled the 0x00000010 bit in %eax (PAE bit)
mov %eax, %cr4
mov %cr0, %eax
or $0x80000000, %eax # Enabled the 0x80000000 bit in %eax (paging bit)
mov %eax, %cr0
leave
ret


