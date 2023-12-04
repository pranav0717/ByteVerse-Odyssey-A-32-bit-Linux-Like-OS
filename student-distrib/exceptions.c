#include "exceptions.h"

/* 
 * divideException
 *   DESCRIPTION: prints "divide exception" to screen and stops process
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints "divide exception" to screen
 */
void divideException(){
    printf("divide exception\n");
    
    // while(1){}
    halt((int8_t)EXCEPTION_STATUS);;
}

/* 
 * debugException
 *   DESCRIPTION: prints "debug exception" to screen and stops process
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints "debug exception" to screen
 */
void debugException(){
    printf("debug exception\n");
    
    // while(1){}
    halt((int8_t)EXCEPTION_STATUS);;
}

/* 
 * NMIException
 *   DESCRIPTION: prints "non maskable interrupt exception" to screen and stops process
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints "non maskable interrupt exception" to screen
 */
void NMIException(){
    printf("non maskable interrupt exception\n");
    
    // while(1){}
    halt((int8_t)EXCEPTION_STATUS);;
}

/* 
 * breakpointException
 *   DESCRIPTION: prints "breakpoint exception" to screen and stops process
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints "breakpoint exception" to screen
 */
void breakpointException(){
    printf("breakpoint exception\n");
    
    // while(1){}
    halt((int8_t)EXCEPTION_STATUS);
}

/* 
 * overflowException
 *   DESCRIPTION: prints "overflow exception" to screen and stops process
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints "overflow exception" to screen
 */
void overflowException(){
    printf("overflow exception\n");
    // while(1){}
    halt((int8_t)EXCEPTION_STATUS);;
}

/* 
 * boundRangeExceededException
 *   DESCRIPTION: prints "bound range exceeded exception" to screen and stops process
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints "bound range exceeded exception" to screen
 */
void boundRangeExceededException(){
    printf("bound range exceeded exception\n");
    
    // while(1){}
    halt((int8_t)EXCEPTION_STATUS);;
}

/* 
 * invalidOpcodeException
 *   DESCRIPTION: prints "invalid opcode exception" to screen and stops process
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints "invalid opcode exception" to screen
 */
void invalidOpcodeException(){
    printf("invalid opcode exception\n");
    
    // while(1){}
    halt((int8_t)EXCEPTION_STATUS);;
}

/* 
 * deviceNotAvailableException
 *   DESCRIPTION: prints "device not available exception" to screen and stops process
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints "device not available exception" to screen
 */
void deviceNotAvailableException(){
    printf("device not available exception\n");
    
    // while(1){}
    halt((int8_t)EXCEPTION_STATUS);;
}

/* 
 * doubleFaultException
 *   DESCRIPTION: prints "double fault exception" to screen and stops process
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints "double fault exception" to screen
 */
void doubleFaultException(){
    printf("double fault exception\n");
    
    // while(1){}
    halt((int8_t)EXCEPTION_STATUS);;
}

/* 
 * invalidTSSException
 *   DESCRIPTION: prints "invalid TSS exception" to screen and stops process
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints "invalid TSS exception" to screen
 */
void invalidTSSException(){
    printf("invalid TSS exception\n");
    
    // while(1){}
    halt((int8_t)EXCEPTION_STATUS);;
}

/* 
 * segmentNotPresentException
 *   DESCRIPTION: prints "segment not present exception" to screen and stops process
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints "segment not present exception" to screen
 */
void segmentNotPresentException(){
    printf("segment not present exception\n");
    
    // while(1){}
    halt((int8_t)EXCEPTION_STATUS);;
}

/* 
 * stackSegFaultException
 *   DESCRIPTION: prints "stack segmentation fault exception" to screen and stops process
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints "stack segmentation fault exception" to screen
 */
void stackSegFaultException(){
    printf("stack segmentation fault exception\n");
    
    // while(1){}
    halt((int8_t)EXCEPTION_STATUS);;
}

/* 
 * generalProtectionFaultException
 *   DESCRIPTION: prints "general protection fault exception" to screen and stops process
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints "general protection fault exception" to screen
 */
void generalProtectionFaultException(){
    printf("general protection fault exception\n");
    
    // while(1){}
    halt((int8_t)EXCEPTION_STATUS);;
}

/* 
 * pageFaultException
 *   DESCRIPTION: prints "page fault exception" to screen and stops process
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints "page fault exception" to screen
 */
void pageFaultException(){
    printf("page fault exception\n");
    
    // while(1){}
    halt((int8_t)EXCEPTION_STATUS);;
}

/* 
 * x87floatingPointException
 *   DESCRIPTION: prints "x87 floating point exception" to screen and stops process
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints "x87 floating point exception" to screen
 */
void x87floatingPointException(){
    printf("x87 floating point exception\n");
    
    // while(1){}
    halt((int8_t)EXCEPTION_STATUS);;
}

/* 
 * alignmentCheckException
 *   DESCRIPTION: prints "alignment check exception" to screen and stops process
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints "alignment check exception" to screen
 */
void alignmentCheckException(){
    printf("alignment check exception\n");
    
    // while(1){}
    halt((int8_t)EXCEPTION_STATUS);;
}

/* 
 * machineCheckException
 *   DESCRIPTION: prints "machine check exception" to screen and stops process
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints "machine check exception" to screen
 */
void machineCheckException(){
    printf("machine check exception\n");
    
    // while(1){}
    halt((int8_t)EXCEPTION_STATUS);;
}

/* 
 * SIMDFPException
 *   DESCRIPTION: prints "simd floating point exception" to screen and stops process
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints "simd floating point exception" to screen
 */
void SIMDFPException(){
    printf("simd floating point exception\n");
    
    // while(1){}
    halt((int8_t)EXCEPTION_STATUS);;
}

/* 
 * virtualizationException
 *   DESCRIPTION: prints "virtualization exception" to screen and stops process
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints "virtualization exception" to screen
 */
void virtualizationException(){
    printf("virtualization exception\n");
    
    // while(1){}
    halt((int8_t)EXCEPTION_STATUS);;
}

/* 
 * controlProtectionException
 *   DESCRIPTION: prints "control protection exception" to screen and stops process
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints "control protection exception" to screen
 */
void controlProtectionException(){
    printf("control protection exception\n");
    
    // while(1){}
    halt((int8_t)EXCEPTION_STATUS);;
}

/* 
 * hypervisorInjectionException
 *   DESCRIPTION: prints "hypervisor injection exception" to screen and stops process
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints "hypervisor injection exception" to screen
 */
void hypervisorInjectionException(){
    printf("hypervisor injection exception\n");
    
    // while(1){}
    halt((int8_t)EXCEPTION_STATUS);
}

/* 
 * VMMCommsException
 *   DESCRIPTION: prints "VMM communications exception" to screen and stops process
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints "VMM communications exception" to screen
 */
void VMMCommsException(){
    printf("VMM communications exception\n");
    
    // while(1){}
    halt((int8_t)EXCEPTION_STATUS);;
}

/* 
 * securityException
 *   DESCRIPTION: prints "security exception" to screen and stops process
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints "security exception" to screen
 */
void securityException(){
    printf("security exception\n");
    
    // while(1){}
    halt((int8_t)EXCEPTION_STATUS);;
}

/* 
 * sysCall
 *   DESCRIPTION: prints "handling system call" to screen and stops process
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints "handling system call" to screen
 */
void sysCall(){
    printf("handling system call");
    // while(1){}
    halt((int8_t)EXCEPTION_STATUS);;
}
