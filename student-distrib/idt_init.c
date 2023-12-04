#include "idt_init.h"

// /* An interrupt descriptor entry (goes into the IDT) */
// typedef union idt_desc_t {
//     uint32_t val[2];
//     struct {
//         uint16_t offset_15_00;
//         uint16_t seg_selector;
//         uint8_t  reserved4;
//         uint32_t reserved3 : 1;
//         uint32_t reserved2 : 1;
//         uint32_t reserved1 : 1;
//         uint32_t size      : 1;
//         uint32_t reserved0 : 1;
//         uint32_t dpl       : 2;
//         uint32_t present   : 1;
//         uint16_t offset_31_16;
//     } __attribute__ ((packed));
// } idt_desc_t;

// /* The IDT itself (declared in x86_desc.S */
// extern idt_desc_t idt[NUM_VEC];


/* 
 * init_idt
 *   DESCRIPTION: initialize idt entries
 *                initialize to either trap or interrupt gates
 *                set exceptions, system calls, rtc, and keyboard entries
 *   INPUTS: none
 *   OUTPUTS: writes gate descriptors to idt
 *   RETURN VALUE: none
 *   SIDE EFFECTS: initializes the idt and sets up some entries
 */
void init_idt(){
    int i;

    for (i = 0; i < NUM_VEC; i++) {                     // 256 Entries in IDT

        idt[i].seg_selector = KERNEL_CS;
        idt[i].present = 0;
        idt[i].size = 1;
        idt[i].reserved1 = 1;
        idt[i].reserved2 = 1;
        idt[i].reserved3 = 1;                       // default is trap gate
        idt[i].dpl = 0;

        if (i <= EXCEPTIONS) {                            // Exceptions are entries less than 0x1F
            idt[i].present = 1;                    // set exceptions to present
            idt[i].reserved3 = 0;
        }

        if (i == SYSCALL) {                            // Entry 0x80 used for system calls
            idt[i].dpl = 3;                         // only syscall is dpl 3
            idt[i].present = 1;
        }
        
        if (i <= 0x2F && i >= 0x20) {               // 0x20 - 0x2F addresses reserved for PIC interrupts
            idt[i].reserved3 = 0;                   // pic uses interrupt gates instead of trap gates
            idt[i].present = 1;
        }
    }

    // set offsets using SET_IDT_ENTRY
        // IDT indexes and assigned exceptions / interrups are based on INTEL documentation of IDT TABLE.
    SET_IDT_ENTRY(idt[DIVIDE],      divideExceptionASM                 );
    SET_IDT_ENTRY(idt[DEBUG],       debugExceptionASM                  );
    SET_IDT_ENTRY(idt[NMI],         NMIExceptionASM                    );
    SET_IDT_ENTRY(idt[BREAKP],      breakpointExceptionASM             );
    SET_IDT_ENTRY(idt[OVERFLOW],    overflowExceptionASM               );
    SET_IDT_ENTRY(idt[BOUND],       boundRangeExceededExceptionASM     );
    SET_IDT_ENTRY(idt[INVALOPCODE], invalidOpcodeExceptionASM          );
    SET_IDT_ENTRY(idt[DEVNOTAV],    deviceNotAvailableExceptionASM     );
    SET_IDT_ENTRY(idt[DOUBLEF],     doubleFaultExceptionASM            );
    SET_IDT_ENTRY(idt[INVTSS],      invalidTSSExceptionASM             );
    SET_IDT_ENTRY(idt[SEGNOTP],     segmentNotPresentExceptionASM      );
    SET_IDT_ENTRY(idt[STACKSEG],    stackSegFaultExceptionASM          );
    SET_IDT_ENTRY(idt[GPF],         generalProtectionFaultExceptionASM );
    SET_IDT_ENTRY(idt[PAGEF],       pageFaultExceptionASM              );
    SET_IDT_ENTRY(idt[X87FP],       x87floatingPointExceptionASM       );
    SET_IDT_ENTRY(idt[ALIGNF],      alignmentCheckExceptionASM         );
    SET_IDT_ENTRY(idt[MACH],        machineCheckExceptionASM           );
    SET_IDT_ENTRY(idt[SIMDFP],      SIMDFPExceptionASM                 );

    // SET_IDT_ENTRY(idt[VIRT], virtualizationExceptionASM);
    // SET_IDT_ENTRY(idt[CONTPRT], controlProtectionExceptionASM);
    // SET_IDT_ENTRY(idt[HYPVIS], hypervisorInjectionExceptionASM);
    // SET_IDT_ENTRY(idt[VMMC], VMMCommsExceptionASM);
    // SET_IDT_ENTRY(idt[SECEXP], securityExceptionASM);

    SET_IDT_ENTRY(idt[SYSCALL],     syscall_asm                        );
    SET_IDT_ENTRY(idt[KEYBOARDI],   keyboard_handlerASM                );
    SET_IDT_ENTRY(idt[RTC],         rtc_handlerASM                     );

    SET_IDT_ENTRY(idt[0x20],        pit_handlerASM                     );

}

