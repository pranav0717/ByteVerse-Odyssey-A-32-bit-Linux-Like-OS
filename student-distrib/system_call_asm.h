#ifndef _SYSCALL_ASM_H
#define _SYSCALL_ASM_H

#include "system_calls.h"

extern void syscall_asm(void); // args are passed directly into regs
// extern void returnFromHalt(uint32_t prev_esp, uint32_t prev_ebp, int8_t status);

#endif /* SYSCALL_ASM_H */
