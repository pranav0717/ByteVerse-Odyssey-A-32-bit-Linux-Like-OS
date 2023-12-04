
#define ASM     1

 #
 # EXCP_LINK (macro)
 #   DESCRIPTION: caller save and calls exception handlers
 #                converts handlers to assembly calls
 #   INPUTS: none
 #   OUTPUTS: same as function outputs
 #   RETURN VALUE: none
 #   SIDE EFFECTS: calls exception handlers
 #
 #   FOR EXCEPTION HANDLER INTERFACES CHECK EXCEPTIONS.C
 #
#define EXCP_LINK(name, func)   ; \
    .globl name                 ; \
    name:                       ; \
        pushal                  ; \
        pushfl                  ; \
        call func               ; \
        popfl                   ; \
        popal                   ; \
        iret                    ; \

EXCP_LINK(divideExceptionASM, divideException);
EXCP_LINK(debugExceptionASM, debugException);
EXCP_LINK(NMIExceptionASM, NMIException);
EXCP_LINK(breakpointExceptionASM, breakpointException);
EXCP_LINK(overflowExceptionASM, overflowException);
EXCP_LINK(boundRangeExceededExceptionASM, boundRangeExceededException);
EXCP_LINK(invalidOpcodeExceptionASM, invalidOpcodeException);
EXCP_LINK(deviceNotAvailableExceptionASM, deviceNotAvailableException);
EXCP_LINK(doubleFaultExceptionASM, doubleFaultException);
EXCP_LINK(invalidTSSExceptionASM, invalidTSSException);
EXCP_LINK(segmentNotPresentExceptionASM, segmentNotPresentException);
EXCP_LINK(stackSegFaultExceptionASM, stackSegFaultException);
EXCP_LINK(generalProtectionFaultExceptionASM, generalProtectionFaultException);
EXCP_LINK(pageFaultExceptionASM, pageFaultException);
EXCP_LINK(x87floatingPointExceptionASM, x87floatingPointException);
EXCP_LINK(alignmentCheckExceptionASM, alignmentCheckException);
EXCP_LINK(machineCheckExceptionASM, machineCheckException);
EXCP_LINK(SIMDFPExceptionASM, SIMDFPException);
EXCP_LINK(virtualizationExceptionASM, virtualizationException);
EXCP_LINK(controlProtectionExceptionASM, controlProtectionException);
EXCP_LINK(hypervisorInjectionExceptionASM, hypervisorInjectionException);
EXCP_LINK(VMMCommsExceptionASM, VMMCommsException);
EXCP_LINK(securityExceptionASM, securityException);


// EXCP_LINK(sysCallASM, sysCall);
EXCP_LINK(rtc_handlerASM, rtc_handler);
EXCP_LINK(keyboard_handlerASM, keyboard_handler);

EXCP_LINK(pit_handlerASM, pit_handler);
