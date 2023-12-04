#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H

#include "types.h"
#include "lib.h"
#include "system_calls.h"

// All functions print exception name and stop process with a while loop
void divideException();
void debugException();
void NMIException();
void breakpointException();
void overflowException();
void boundRangeExceededException();
void invalidOpcodeException();
void deviceNotAvailableException();
void doubleFaultException();
void invalidTSSException();
void segmentNotPresentException();
void stackSegFaultException();
void generalProtectionFaultException();
void pageFaultException();
void x87floatingPointException();
void alignmentCheckException();
void machineCheckException();
void SIMDFPException();
void virtualizationException();
void controlProtectionException();
void hypervisorInjectionException();
void VMMCommsException();
void securityException();
void sysCall();

#endif // EXCEPTIONS_H
