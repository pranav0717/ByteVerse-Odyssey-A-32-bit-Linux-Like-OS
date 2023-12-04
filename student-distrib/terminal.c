#include "terminal.h"

// Do i need a terminal init to set up base values?

/* 
 * get_visible_term
 *   DESCRIPTION: return terminal struct pointer to visible terminal
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: terminal struct pointer to visible terminal
 *   SIDE EFFECTS: none
 */
terminal_t * get_visible_term(){
    return &terminals[visible_term];
}

/* 
 * get_running_term
 *   DESCRIPTION: return terminal struct pointer to running terminal
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: terminal struct pointer to running terminal
 *   SIDE EFFECTS: none
 */
terminal_t * get_running_term(){
    return &terminals[sched_term];
}

/* 
 * terminal_init
 *   DESCRIPTION: initialize the terminal structs with starting values
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: sets video mem address
 */
void terminal_init() {
    int32_t i, idx;

    for (idx = 0; idx < 3; ++idx) {

        // terminals[idx].attribute = (idx == 0) ? 0x1F : ((idx == 1) ? 0x3E : 0x8D);

        terminals[idx].status     = 0;
        terminals[idx].r_flag     = 0;
        terminals[idx].screen_x   = 0;
        terminals[idx].screen_y   = 0;
        terminals[idx].active_pid = 0;
        terminals[idx].num_pids = 0;
        terminals[idx].rb_idx = 0;
        terminals[idx].counter_freqRate = 1024/2;
        terminals[idx].rtc_counter = 0;
        terminals[idx].rtc_interrupt_occured = 0;
        terminals[idx].VRAM_addr = (TERM1_VRAM + idx) * KB_4;
        // video_mem = (char *)terminals[idx].VRAM_addr;
        clear();
        for (i = 0; i < BUFFER_LIMIT; i++) {
            terminals[idx].read_buffer[i] = '\0';
        }
    }

    video_mem = (char *)VIDEO;
    terminals[0].status = 1;
    terminals[0].active_pid = 1;
    terminals[0].base_pid = 1;
    terminals[0].r_flag = 1;
    // Backup Page addresses B9, BA, BB
    terminals[0].VRAM_addr = 0xB9000;   
    terminals[1].VRAM_addr = 0xBA000;
    terminals[2].VRAM_addr = 0xBB000;

    visible_term = 0;
    next_terminal = 0;
    running_term = 0;
    sched_term = 0;
}

/* 
 * switch_terminal
 *   DESCRIPTION: switch the visible terminal
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: changes video paging
 */
void switch_terminal(uint8_t terminalNumber) {
    cli();

    next_terminal = terminalNumber;

    if (next_terminal == visible_term) {
        sti();
        return;
    }

    uint32_t temp = page_table[0xB8].frame;
    page_table[0xB8].frame = 0xB8;
    asm volatile (
        "movl %%cr3, %%eax ;"
        "movl %%eax, %%cr3 ;"
        :
        :
        :"eax","cc"
    );

    terminals[visible_term].rb_idx = buffer_index;

    memcpy(terminals[visible_term].read_buffer, (int8_t*)term_buffer, BUFFER_LIMIT);
    memcpy((uint8_t*)terminals[visible_term].VRAM_addr, (uint8_t*)VIDEO, KB_4);

    buffer_index = terminals[next_terminal].rb_idx;

    memcpy((uint8_t*)VIDEO, (uint8_t*)terminals[next_terminal].VRAM_addr, KB_4);
    memcpy((int8_t*)term_buffer, terminals[next_terminal].read_buffer, BUFFER_LIMIT);

    // set_attribute(terminals[next_terminal].attribute);

    terminals[visible_term].r_flag = 0;
    terminals[next_terminal].r_flag = 1;

    visible_term = next_terminal;                  // LAST STEP

    // video_mem = terminals[visible_term].VRAM_addr;

    update_cursor();

    page_table[0xB8].frame = temp;
    asm volatile (
        "movl %%cr3, %%eax ;"
        "movl %%eax, %%cr3 ;"
        :
        :
        :"eax","cc"
    );

    sti();

    return;

}

/* 
 * scheduler
 *   DESCRIPTION: handles scheduling by switching running process and adding a scheduled process to a round robin.
 *   This way every process gets a time-slice on a uni-processor, making program running more efficient and almost-simultaneous. 
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: sets video mem address
 */
void scheduler(void) {

    running_term = sched_term;

    sched_term = (sched_term + 1) % 3;

    // if (sched_term == running_term) return;

    // Paging Stuff

    if (sched_term == visible_term){
            
        page_table[0xB8].frame = 0xB8;  // visible term page = B8
        page_table_vidmap[sched_term].frame = 0xB8;

    } else {
        
        if (sched_term == 0) {
                page_table[0xB8].frame = 0xB9;      // Backup terminal page 1
                page_table_vidmap[sched_term].frame = 0xB9;
        } 
        else if (sched_term == 1) {
                page_table[0xB8].frame = 0xBA;      // Backup terminal page 2
                page_table_vidmap[sched_term].frame = 0xBA;
        }
        else {
                page_table[0xB8].frame = 0xBB;      // Backup terminal page 3
                page_table_vidmap[sched_term].frame = 0xBB;
        }  

    }  

    //flush tlb
    asm volatile (
        "movl %%cr3, %%eax ;"
        "movl %%eax, %%cr3 ;"
        :
        :
        :"eax","cc"
    );

    if (terminals[sched_term].status == 0) {
        
        terminals[sched_term].status = 1;

        int pid_num;
        for (pid_num = 0; pid_num < MAX_PCBS; pid_num++) {
            if (pid_arr[pid_num] == 0){
                pid_num++;
                break;
            }
        }

        terminals[sched_term].base_pid = pid_num;
        terminals[sched_term].active_pid = pid_num;

        int esp, ebp;

        asm volatile (
            // Save ESP
            "movl %%esp, %0"
            : "=r" (esp)
            :
            : "cc"

        );

        asm volatile (
            // Save EBP
            "movl %%ebp, %0"
            : "=r" (ebp)
            :
            : "cc"
        );

        pcb_t * prev_pcb = get_active_pcb(terminals[running_term].active_pid);

        prev_pcb->esp = esp;
        prev_pcb->ebp = ebp;
        prev_pcb->tss_esp0 = tss.esp0;

        clear();

        execute((uint8_t*)"shell");
        return;

    } else {

        pcb_t * curr_pcb = get_active_pcb(terminals[running_term].active_pid);
        pcb_t * next_pcb = get_active_pcb(terminals[sched_term].active_pid);

        page_directory[32].pde_4mb.frame = (MB_8 + ((next_pcb->pid - 1)  * MB_4)) >> 22; //shift by 22 to offset 4MB pages

        // FLUSH TLB
        asm volatile (
            "movl %%cr3, %%eax ;"
            "movl %%eax, %%cr3 ;"
            :
            :
            :"eax","cc"
        );


        curr_pcb->tss_esp0 = tss.esp0;

        //context
        tss.ss0 = KERNEL_DS;
        tss.esp0 = next_pcb->tss_esp0; //now has to go back to the scheduled task USING NEXT PID

        asm volatile (
            // Save ESP
            "movl %%esp, %0"
            : "=r" (curr_pcb->esp)
            :
            : "cc"

        );

        asm volatile (
            // Save EBP
            "movl %%ebp, %0"
            : "=r" (curr_pcb->ebp)
            :
            : "cc"
        );

        asm volatile (
            "movl %0, %%ebp ;"
            "movl %1, %%esp ;"

            :   
            :   "r" (next_pcb->ebp), "r" (next_pcb->esp)
            :   "esp", "ebp"
        );

    }

    return;
}


