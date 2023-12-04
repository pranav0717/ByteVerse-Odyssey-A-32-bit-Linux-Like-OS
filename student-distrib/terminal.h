#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include "types.h"
#include "lib.h"
#include "system_calls.h"
#include "keyboard.h"
#include "paging.h"

typedef struct terminal {
    
    volatile int rtc_interrupt_occured;
    volatile int rtc_counter;
    volatile int counter_freqRate; 

    int32_t screen_x;
    int32_t screen_y;

    int8_t active_pid;
    int8_t base_pid;
    int8_t num_pids;

    int8_t status;
    int8_t r_flag;              // read flag -- to restrict terminal read in non active terminals

    uint8_t read_buffer[128];
    uint8_t rb_idx;

    uint32_t VRAM_addr;
    // uint8_t attribute;

    // How do i save current screen

} terminal_t;

// extern terminal_t terminals[3];

terminal_t terminals[3];

volatile int running_term;
volatile int sched_term;
uint8_t visible_term;
uint8_t next_terminal;

void terminal_init(void);
void switch_terminal(uint8_t terminalNumber);
terminal_t * get_visible_term();
terminal_t * get_running_term();
void scheduler(void);




#endif
