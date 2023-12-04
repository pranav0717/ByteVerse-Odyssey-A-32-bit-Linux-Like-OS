/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7  */
uint8_t slave_mask;  /* IRQs 8-15 */

/* Initialize the 8259 PIC */

/* 
 * i8259_init
 *   DESCRIPTION: initialize both pics by writing to master and slave pics
 *                as described in os dev wiki
 *   INPUTS: none
 *   OUTPUTS: writes data to master and slave command/data ports
 *   RETURN VALUE: none
 *   SIDE EFFECTS: intitializes pics and clears all IRQ lines
 */
void i8259_init(void) {
    
    master_mask = inb(MASTER_8259_PORT + 1); // mask all at first
    slave_mask = inb(SLAVE_8259_PORT + 1);

    outb(0xFF, MASTER_8259_PORT + 1); // 0xFF = all IRQs masked
    outb(0xFF, SLAVE_8259_PORT + 1);

    outb(ICW1, MASTER_8259_PORT); // cmd port
    outb(ICW2_MASTER, MASTER_8259_PORT + 1); //data port
    outb(ICW3_MASTER, MASTER_8259_PORT + 1);
    outb(ICW4, MASTER_8259_PORT + 1);

    
    // cmd port
    
    outb(ICW1, SLAVE_8259_PORT);

    // rest are to data port
    
    outb(ICW2_SLAVE, SLAVE_8259_PORT + 1);
    outb(ICW3_SLAVE, SLAVE_8259_PORT + 1);
    outb(ICW4, SLAVE_8259_PORT + 1);

    outb(master_mask, MASTER_8259_PORT + 1);
    outb(slave_mask, SLAVE_8259_PORT + 1);

}

/* Enable (unmask) the specified IRQ */

/* 
 * enable_irq
 *   DESCRIPTION: enable one IRQ line in either the master or slave pic 
 *   INPUTS: irq_num -- which IRQ to enable from 0-15 inclusive
 *   OUTPUTS: writes mask to data port to enable desired IRQ
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
void enable_irq(uint32_t irq_num) {

    uint16_t port;
    uint8_t value;

    if (irq_num > 15 || irq_num < 0) return; // invalid check
 
    if(irq_num < NUM_IRQS) {
        port = MASTER_8259_PORT + 1; // data port
    } else {
        port = SLAVE_8259_PORT + 1;
        irq_num -= NUM_IRQS;
    }
    value = inb(port) & ~(1 << irq_num);
    outb(value, port); 


}

/* Disable (mask) the specified IRQ */

/* 
 * disable_irq
 *   DESCRIPTION: disable one IRQ line in either the master or slave pic 
 *   INPUTS: irq_num -- which IRQ to disable from 0-15 inclusive
 *   OUTPUTS: writes mask to data port to disable desired IRQ
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
void disable_irq(uint32_t irq_num) {
    
    uint16_t port;
    uint8_t value;

    if (irq_num > 15 || irq_num < 0) return; // invalid check
 
    if(irq_num < NUM_IRQS) {
        port = MASTER_8259_PORT + 1; // data port
    } else {
        port = SLAVE_8259_PORT + 1;
        irq_num -= NUM_IRQS;
    }
    value = inb(port) | (1 << irq_num);
    outb(value, port); 
}

/* Send end-of-interrupt signal for the specified IRQ */

/* 
 * send_eoi
 *   DESCRIPTION: send eoi signal to desired IRQ on either master or slave pic 
 *   INPUTS: irq_num -- which IRQ to send eoi to from 0-15 inclusive
 *   OUTPUTS: writes eoi to data port of desired IRQ
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
void send_eoi(uint32_t irq_num) {

    if (irq_num > 15 || irq_num < 0) return; // invalid check
    
    if(irq_num >= NUM_IRQS)
		outb(PIC_EOI, SLAVE_8259_PORT);
 
	outb(PIC_EOI, MASTER_8259_PORT);

}
