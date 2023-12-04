#include "pit.h"

#define PIT_MAGIC   1193180
#define CMD_BYTE    0x36
#define CMD_MAGIC   0x46
#define FORTY_HEX   0x40

/* 
 * pic_init
 *   DESCRIPTION: Initializes the pit hardware through the PIC
 *   INPUTS: none
 *   OUTPUTS: writes bits to pit data port according to os dev
 *   RETURN VALUE: none
 *   SIDE EFFECTS: enables the irq0 line on the primary pic
 */
void pit_init(void) {

    // Credits to: http://www.osdever.net/bkerndev/Docs/pit.htm

    int divisor = PIT_MAGIC / 100 ; // 100 HZ -- /* Calculate our divisor */

    outb(CMD_BYTE, CMD_MAGIC);           /* Set our command byte 0x36 */
    outb(divisor & 0xFF, FORTY_HEX); /* Set low byte of divisor */
    outb(divisor >> 8, FORTY_HEX);   /* Set high byte of divisor */

    enable_irq(0);  // enable irq0 for pit

}

/* 
 * pit_handler
 *   DESCRIPTION: handle pit interrupt by sending eoi and calling scheduler
 *   INPUTS: none
 *   OUTPUTS: writes to pic by sending eoi
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
void pit_handler(void) {

    cli();
    send_eoi(0); // send eoi to irq0 for pit
    sti();

    // cli();
    scheduler();
    // sti();

    return;

}
