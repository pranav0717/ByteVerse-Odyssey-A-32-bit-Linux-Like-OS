#ifndef _IDT_INIT_H
#define _IDT_INIT_H

#include "idtfunctions_asm.h"
#include "x86_desc.h"
#include "system_call_asm.h"

#define EXCEPTIONS  0x1F

#define DIVIDE  0      
#define DEBUG  1      
#define NMI 2      
#define BREAKP  3      
#define OVERFLOW  4      
#define BOUND  5      
#define INVALOPCODE  6      
#define DEVNOTAV  7      
#define DOUBLEF  8      
#define INVTSS  10    
#define SEGNOTP  11    
#define STACKSEG  12    
#define GPF  13    
#define PAGEF  14    
#define X87FP  16    
#define ALIGNF  17    
#define MACH  18    
#define SIMDFP  19    
// #define VIRT    20
// #define CONTPRT     21
// #define HYPVIS  28
// #define VMMC    29
// #define SECEXP  30
 
#define SYSCALL  0x80
#define KEYBOARDI  0x21
#define RTC 0x28

void init_idt();

#endif // IDT_INIT_H
