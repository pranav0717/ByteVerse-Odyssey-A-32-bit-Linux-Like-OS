#ifndef _SYSTEM_CALLS_H
#define _SYSTEM_CALLS_H

#include "file_system.h"
#include "paging.h"
#include "keyboard.h"
#include "rtc.h"
#include "x86_desc.h"
#include "system_call_asm.h"
#include "terminal.h"

#define NUM_ELF             4
#define MAGICB0         0x7F
#define MAGICB1         0x45
#define MAGICB2         0x4C
#define MAGICB3         0x46
#define MAX_ARGS        3

#define PROG_ADDRESS    0x08048000
#define KB_8            8192
#define ALOT            10000000
#define PROG_EP_OFFSET  24
#define EXCEPTION_STATUS    100
#define RETURN_EXCEPTION    256

int32_t halt (int8_t status);
int32_t execute (const uint8_t* command);
int32_t read (int32_t fd, void* buf, int32_t nbytes);
int32_t write (int32_t fd, const void* buf, int32_t nbytes);
int32_t open (const int8_t* filename);
int32_t close (int32_t fd);
int32_t getargs (int8_t* buf, int32_t nbytes);
int32_t vidmap (int8_t** screen_start);
int32_t set_handler (int32_t signum, void* handler_address);
int32_t sigreturn (void);

pcb_t * get_active_pcb();
int get_active_kernel_stack_addr(int32_t pid);

void build_pcb(pcb_t * pcb, int32_t prog_eip, int8_t * args, int pid_num);

int lsFlag;

#endif /* SYSTEM_CALLS_H */
