#include "rtc.h"
#include "i8259.h"
#include "lib.h"
#include "terminal.h"

volatile int rtc_interrupt_occured;
volatile int counter_freqRate = 1024/2;
volatile int counter;

/* 
 * rtc_init
 *   DESCRIPTION: Initializes the rtc hardware through the PIC
 *   INPUTS: none
 *   OUTPUTS: writes bits to RTC data port according to os dev
 *   RETURN VALUE: none
 *   SIDE EFFECTS: enables the irq line on the secondary PIC
 */
void rtc_init(void) {

    outb(REG_A, RTC_PORT);          // select Status Register A, and disable NMI (by setting the 0x80 bit)
    // outb(0x20, RTC_DATA);           // write to CMOS/RTC RAM - setting 5th bit as high
    char prev = inb(RTC_DATA);      // read the current value of register 

    outb(REG_B, RTC_PORT);          // select register B, and disable NMI
    // char prev = inb(RTC_DATA);      // read the current value of register 
    
    outb(prev | 0x40, RTC_DATA);    // write the previous value ORed with 0x40. This turns on bit 6 of register B

    enable_irq(RTC_IRQ_LINE);
    // printf("RTC enabled !!!!!!");
    
    // rtc_interrupt_occured = 0;
    // counter = 0;

}

/* 
 * rtc_handler
 *   DESCRIPTION: handle rtc interrupt according to os dev and send eoi when done
 *   INPUTS: none
 *   OUTPUTS: writes data to rtc port to select register C
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
void rtc_handler(void) {
    // printf("a\n");
    terminals[0].rtc_counter++;
    terminals[1].rtc_counter++;
    terminals[2].rtc_counter++;

    if (terminals[sched_term].rtc_counter >= terminals[sched_term].counter_freqRate) {

        terminals[sched_term].rtc_interrupt_occured = 1;
        terminals[sched_term].rtc_counter = 0;
        
    } else {
        terminals[sched_term].rtc_interrupt_occured = 0;        
    }  
    

    outb(REG_C, RTC_PORT);	        // select register C
    inb(RTC_DATA);		            // just throw away content

    send_eoi(RTC_IRQ_LINE);         // end interrupt


}


/* 
 * rtc_open
 *   DESCRIPTION: "Opens" RTC
 *   INPUTS: filename
 *   RETURN VALUE: int32_t
 *   SIDE EFFECTS: sets default frequency
 */
int32_t rtc_open (const uint8_t* filename) {
    terminals[sched_term].counter_freqRate = 1024/2;
    // terminals[sched_term].rtc_counter = 0;
    return 0;

}

/* 
 * rtc_close
 *   DESCRIPTION: "Closes" RTC
 *   INPUTS: filename
 *   RETURN VALUE: int32_t
 *   SIDE EFFECTS: None
 */
int32_t rtc_close (const uint8_t* filename) {
    return 0;
}

/* 
 * rtc_write
 *   DESCRIPTION: "Closes" RTC
 *   INPUTS: fd, buf, nbytes
 *   RETURN VALUE: int32_t
 *   SIDE EFFECTS: Changes RTC frequency
 */
int32_t rtc_write(int32_t fd, const int32_t* buf, int32_t nbytes) {
    
    if (buf == NULL) {
        return -1;
    }

    if (sizeof(buf) != sizeof(uint32_t)) {
        return -1;
    }

    int32_t frequency = *buf;

    // Check if input frequency is valid (within bounds?)
    if (*buf < RTC_MIN || *buf > RTC_MAX) {

        return -1;

    } else if (!((*buf - 1) & 1)) {

        return -1;

    } else {

        terminals[sched_term].counter_freqRate = (RTC_MAX / (frequency));   // Calculating interval to virtaully set frequency
        
    }

    terminals[sched_term].rtc_counter = 0;

    return 0;
}

/* 
 * rtc_read
 *   DESCRIPTION: Handles RTC Read (Virtuilization)
 *   INPUTS: none
 *   RETURN VALUE: int32_t
 *   SIDE EFFECTS: Sets interrupt flag to 0 and waits for interrupt to occur
 */
int32_t rtc_read (int32_t fd, int32_t* buf, int32_t nbytes) {

    // Looping to hold processor in wait state until rtc interrupt occurs.
    while (terminals[sched_term].rtc_interrupt_occured != 1) ;
    terminals[sched_term].rtc_interrupt_occured = 0;              // Setting flag to 0


    return 0;
}
