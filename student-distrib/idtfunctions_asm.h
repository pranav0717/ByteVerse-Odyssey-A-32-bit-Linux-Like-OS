#ifndef _IDTFUNCTIONS_ASM_H
#define _IDTFUNCTIONS_ASM_H

#ifndef ASM

#include "exceptions.h"
#include "rtc.h"
#include "keyboard.h"
#include "pit.h"

// function headers
// for function interfaces, check exceptions.c
void divideExceptionASM();
void debugExceptionASM();
void NMIExceptionASM();
void breakpointExceptionASM();
void overflowExceptionASM();
void boundRangeExceededExceptionASM();
void invalidOpcodeExceptionASM();
void deviceNotAvailableExceptionASM();
void doubleFaultExceptionASM();
void invalidTSSExceptionASM();
void segmentNotPresentExceptionASM();
void stackSegFaultExceptionASM();
void generalProtectionFaultExceptionASM();
void pageFaultExceptionASM();
void x87floatingPointExceptionASM();
void alignmentCheckExceptionASM();
void machineCheckExceptionASM();
void SIMDFPExceptionASM();
void virtualizationExceptionASM();
void controlProtectionExceptionASM();
void hypervisorInjectionExceptionASM();
void VMMCommsExceptionASM();
void securityExceptionASM();
// void sysCallASM();
void rtc_handlerASM();
void keyboard_handlerASM();
void pit_handlerASM();

#endif
#endif
