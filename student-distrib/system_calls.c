#include "system_calls.h"

// file jumptable
struct file_ops f_ops = {
    .open = &file_open,
    .close = &file_close,
    .write = &file_write,
    .read = &file_read,
};

// dir jumptable
struct file_ops d_ops = {
    .open = &file_open,
    .close = &file_close,
    .write = &dir_write,
    .read = &dir_read,
};

// rtc jumptable
struct file_ops rtc_ops = {
    .open = (int32_t (*)(const uint8_t*)) &rtc_open,
    .close = (int32_t (*)(int32_t)) &rtc_close,
    .write = (int32_t (*)(int32_t, const uint8_t*, int32_t)) &rtc_write,
    .read = (int32_t (*)(int32_t, uint8_t*, int32_t)) &rtc_read,
};

// terminal jumptable
struct file_ops stdin_fops = {
    .open = &terminal_open,
    .close = &terminal_close,
    .write = NULL,
    .read = (int32_t (*)(int32_t, uint8_t*, int32_t)) &terminal_read,
};

// terminal jumptable
struct file_ops stdout_fops = {
    .open = &terminal_open,
    .close = &terminal_close,
    .write = (int32_t (*)(int32_t, const uint8_t*, int32_t)) &terminal_write,
    .read = NULL,
};

/* 
 * int32_t read(int32_t fd, void* buf, int32_t nbytes)
 * 
 * DESCRIPTION: Reads data from the file or device associated with the specified file descriptor.
 * 
 * INPUTS:
 *   - fd: The file descriptor indicating the file or device to read from.
 *   - buf: A pointer to the buffer where the read data should be stored.
 *   - nbytes: The number of bytes to read.
 * 
 * OUTPUTS: None
 * 
 * RETURN VALUE:
 *   - The number of bytes read, which may be less than 'nbytes' if an error occurs.
 *   - -1: An error occurred (e.g., invalid file descriptor, null buffer, or read operation not supported).
 * 
 * SIDE EFFECTS:
 *   - Reads data from the file or device associated with the specified file descriptor and stores it in the provided buffer.
 */ 

int32_t read (int32_t fd, void* buf, int32_t nbytes){
    terminal_t * active_term = get_running_term();

    int pid_num = active_term->active_pid;

    pcb_t * pcb = get_active_pcb(pid_num);
    update_file_pcb(pcb);

    if (fd < 0 || fd >= pcb->num_open_descriptors) return -1; // out of bounds
    if (!buf) return -1;

    fd_t* curr = &(pcb->file_desc_arr[fd]);

    if (curr->fops && curr->fops->read) {
        return curr->fops->read(fd, buf, nbytes);
    }

    return -1;
}

/* 
 * int32_t write(int32_t fd, const void* buf, int32_t nbytes)
 * 
 * DESCRIPTION: Writes data to the file or device associated with the specified file descriptor.
 * 
 * INPUTS:
 *   - fd: The file descriptor indicating the file or device to write to.
 *   - buf: A pointer to the buffer containing the data to be written.
 *   - nbytes: The number of bytes to write.
 * 
 * OUTPUTS: None
 * 
 * RETURN VALUE:
 *   - The number of bytes written, which may be less than 'nbytes' if an error occurs.
 *   - -1: An error occurred (e.g., invalid file descriptor, null buffer, or write operation not supported).
 * 
 * SIDE EFFECTS:
 *   - Writes data to the file or device associated with the specified file descriptor.
 */ 

int32_t write (int32_t fd, const void* buf, int32_t nbytes){

    terminal_t * active_term = get_running_term();

    int pid_num = active_term->active_pid;

    pcb_t * pcb = get_active_pcb(pid_num);
    update_file_pcb(pcb);

    if (fd < 0 || fd >= pcb->num_open_descriptors) return -1; // out of bounds
    if (!buf) return -1;

    fd_t* curr = &(pcb->file_desc_arr[fd]);

    if (curr->fops && curr->fops->write) {
        return curr->fops->write(fd, buf, nbytes);
    }

    return -1;
}

/* 
 * int32_t open(const int8_t* filename)
 * 
 * DESCRIPTION: Opens a file specified by the provided filename and associates it with a file descriptor.
 * 
 * INPUTS:
 *   - filename: A pointer to a null-terminated string containing the name of the file to be opened.
 * 
 * OUTPUTS: None
 * 
 * RETURN VALUE:
 *   - The file descriptor associated with the opened file.
 *   - -1: An error occurred (e.g., maximum number of open file descriptors reached, file not found, or invalid file type).
 * 
 * SIDE EFFECTS:
 *   - Opens the specified file, associates it with a file descriptor, and updates the process's open file descriptors.
 */ 

int32_t open (const int8_t* filename){
    terminal_t * active_term = get_running_term();

    int pid_num = active_term->active_pid;

    pcb_t * pcb = get_active_pcb(pid_num);

    if (pcb->num_open_descriptors >= NUM_FDS) return -1;

    dentry_t myDentry; // local dentry

    if (read_dentry_by_name((int8_t*)filename, &myDentry) == -1) return -1;

    int32_t fd;
    for (fd = 2; fd < NUM_FDS; fd++) { // Start from 2 because 0 and 1 are stdin and stdout
        if (pcb->file_desc_arr[fd].flags == 0) { // check if free
            break;
        }
    }

    if (myDentry.filetype == USER_FILETYPE) { // set functions and inode based on file type
        // rtc_open((uint8_t*)"");
        pcb->file_desc_arr[fd].fops = &(rtc_ops);
        pcb->file_desc_arr[fd].inode_num = 0;
    } else if (myDentry.filetype == DIR_FILETYPE) {
        pcb->file_desc_arr[fd].fops = &(d_ops);
        pcb->file_desc_arr[fd].inode_num = 0;
    } else if (myDentry.filetype == FILE_FILETYPE) { // only files have an inode
        pcb->file_desc_arr[fd].fops = &(f_ops);
        pcb->file_desc_arr[fd].inode_num = myDentry.inode_num;
    } else {
        return -1;
    }

    // pcb->file_desc_arr[fd].fops->open((uint8_t*)filename);
    pcb->file_desc_arr[fd].file_offset = 0; // start at beginning
    pcb->file_desc_arr[fd].flags = 1; // set active
    pcb->file_desc_arr[fd].fname = myDentry.filename; // save file name

    pcb->num_open_descriptors++;

    return fd;
}

/* 
 * int32_t close(int32_t fd)
 * 
 * DESCRIPTION: Closes a file descriptor associated with the current process.
 * 
 * INPUTS:
 *   - fd: The file descriptor to be closed.
 * 
 * OUTPUTS: None
 * 
 * RETURN VALUE:
 *   - 0: The file descriptor was successfully closed.
 *   - -1: An error occurred (e.g., attempting to close stdin or stdout or an invalid file descriptor).
 * 
 * SIDE EFFECTS:
 *   - Closes the specified file descriptor, updates the process's open file descriptors,
 *     and releases associated resources.
 */ 

int32_t close (int32_t fd){

    terminal_t * active_term = get_running_term();

    int pid_num = active_term->active_pid;

    pcb_t * pcb = get_active_pcb(pid_num);

    if (fd < 2 || fd >= pcb->num_open_descriptors) return -1; // cant close stdin and out

    if (pcb->file_desc_arr[fd].flags == 0) return -1;

    pcb->file_desc_arr[fd].fops = NULL; // clear fd struct
    pcb->file_desc_arr[fd].inode_num = 0;
    pcb->file_desc_arr[fd].file_offset = 0;
    pcb->file_desc_arr[fd].flags = 0;
    pcb->file_desc_arr[fd].fname = NULL;

    pcb->num_open_descriptors--; // decrement num open

    return 0;
}

/* 
 * int getCommand(const int8_t* command, int8_t* cmd)
 * 
 * DESCRIPTION: Parses the command to extract and store the main command in a character array.
 * 
 * INPUTS:
 *   - command: A pointer to a null-terminated string containing the full command with arguments.
 *   - cmd: A character array to store the parsed main command.
 * 
 * OUTPUTS: None
 * 
 * RETURN VALUE: The number of characters processed to extract the main command.
 * 
 * SIDE EFFECTS:
 *   - Parses the 'command' string to extract the main command and stores it in the 'cmd' array.
 *   - Null-terminates the 'cmd' array.
 */ 

int getCommand(const uint8_t *command, int8_t *cmd) {
    int i;
    int cmdBuffIndex = 0;

    for (i = 0; i < FILENAME_LEN; i++) {
        if (command[i] != ' ' && command[i] != '\n') {
            cmd[cmdBuffIndex++] = command[i];
        } else {
            while (command[i] == ' ' || command[i] == '\n') {
                i++;
            }
            break;
        }
    }

    cmd[cmdBuffIndex] = '\0'; // null term the cmd string
    return i;
}

/* 
 * void getArguments(const int8_t* command, int8_t args[MAX_ARGS][FILENAME_LEN], int startIndex)
 * 
 * DESCRIPTION: Parses the command to extract and store its arguments in a 2D character array.
 * 
 * INPUTS:
 *   - command: A pointer to a null-terminated string containing the command and its arguments.
 *   - args: A 2D character array to store the parsed arguments.
 *   - startIndex: The starting index in the 'command' string from which to begin parsing.
 * 
 * OUTPUTS: None
 * 
 * RETURN VALUE: None
 * 
 * SIDE EFFECTS:
 *   - Parses the 'command' string to extract and store its arguments in the 'args' array.
 */ 

void getArguments(const uint8_t *command, int8_t* args, int startIndex) {
    int i = startIndex;
    int argIdx = 0;
    while (command[i] != '\0') {
        args[argIdx] = command[i];
        i++;
        argIdx++;
    }

}

/* 
 * void build_pcb(pcb_t* pcb, int32_t prog_eip)
 * 
 * DESCRIPTION: Initializes a Process Control Block (PCB) with the provided program entry point and other default values.
 * 
 * INPUTS:
 *   - pcb: A pointer to the PCB to be initialized.
 *   - prog_eip: The entry point address of the program to be executed.
 * 
 * OUTPUTS: None
 * 
 * RETURN VALUE: None
 * 
 * SIDE EFFECTS:
 *   - Initializes the PCB with process-specific information, such as process ID, parent process ID,
 *     program entry point, file descriptors, and more.
 */ 

void build_pcb(pcb_t * pcb, int32_t prog_eip, int8_t * args, int pid_num) {

    int i;

    // terminal_t * active_term = get_visible_term();

    pcb->pid = pid_num;
    // pcb->parent_pid = pid_num - 1;

    if (pid_num == terminals[sched_term].base_pid) {

        pcb->parent_pid = pid_num;

    } else {

        pcb->parent_pid = terminals[sched_term].active_pid;
        
    }

    // active_term->active_pid = pid_num;
    terminals[sched_term].active_pid = pid_num;
    // active_term->num_pids++;

    // pcb->user_prog_ep = prog_eip;

    memcpy((int8_t*)pcb->args, args, FILENAME_LEN);

    pcb->prev_tss_ESP0 = tss.esp0;
    pcb->prev_tss_SS = tss.ss0;

    // ALL FDS CLEARED AT INIT
    for (i = 2; i < NUM_FDS; i++) {
        pcb->file_desc_arr[i].fops = NULL;
        pcb->file_desc_arr[i].inode_num = 0;
        pcb->file_desc_arr[i].file_offset = 0;
        pcb->file_desc_arr[i].flags = 0;
        pcb->file_desc_arr[i].fname = "";        
    }

    //stdin

	pcb->file_desc_arr[0].fops = &stdin_fops;
	pcb->file_desc_arr[0].inode_num = 0;
	pcb->file_desc_arr[0].file_offset = 0;
	pcb->file_desc_arr[0].flags = 1;
    pcb->file_desc_arr[0].fname = "stdin";

	//stdout
	pcb->file_desc_arr[1].fops = &stdout_fops;
	pcb->file_desc_arr[1].inode_num = 0;
	pcb->file_desc_arr[1].file_offset = 0;
	pcb->file_desc_arr[1].flags = 1;
    pcb->file_desc_arr[1].fname = "stdout";
    
    pcb->num_open_descriptors = 2; // 2 open bc stdin and stdout

    update_file_pcb(pcb);
	

}

/* 
 * pcb_t* get_active_pcb(int32_t pid)
 * 
 * DESCRIPTION: Retrieves a pointer to the Process Control Block (PCB) of the specified process ID (PID).
 * 
 * INPUTS:
 *   - pid: The process ID for which the PCB pointer is to be retrieved.
 * 
 * OUTPUTS: None
 * 
 * RETURN VALUE:
 *   - A pointer to the PCB of the specified process ID.
 * 
 * SIDE EFFECTS: None
 */ 

pcb_t * get_active_pcb(int32_t pid){
    return (pcb_t*)(MB_8 - (pid * KB_8));
}

/* 
 * int get_active_kernel_stack_addr(int32_t pid)
 * 
 * DESCRIPTION: Calculates the address of the active kernel stack for a given process ID (PID).
 * 
 * INPUTS:
 *   - pid: The process ID for which the kernel stack address is to be calculated.
 * 
 * OUTPUTS: None
 * 
 * RETURN VALUE:
 *   - The address of the active kernel stack for the specified process ID.
 * 
 * SIDE EFFECTS: None
 */ 

int get_active_kernel_stack_addr(int32_t pid){
    return (MB_8 - ((pid - 1) * KB_8) - sizeof(uint32_t));
}



/* 
 * int32_t execute(const int8_t* command)
 * 
 * DESCRIPTION: Executes a new process as specified by the given command.
 * 
 * INPUTS:
 *   - command: A pointer to a null-terminated string containing the command to be executed.
 * 
 * OUTPUTS: None
 * 
 * RETURN VALUE:
 *   - 0: Successfully executed the command.
 *   - -1: Error occurred during execution (e.g., invalid command or file).
 * 
 * SIDE EFFECTS:
 *   - Loads the specified program into memory, updates process control blocks (PCBs),
 *     sets up program entry point, and starts executing the new process.
 *   - If successful, control is transferred to the new process. If an error occurs,
 *     no change in control or state.
 */ 

int32_t execute (const uint8_t* command) {

    cli();

    int8_t cmd[FILENAME_LEN];
    int8_t args[FILENAME_LEN];
    int i;

    for (i = 0; i < FILENAME_LEN; i++) {
        args[i] = '\0';
        cmd[i] = '\0';
    }

    int cmdIndex;

    // Clear cmd and args buffers

    if (command == NULL) {
        sti();
        return -1;
    }

    // Get Command
    cmdIndex = getCommand(command, cmd);                                // Random Values here for some reason

    // Get Arguments
    getArguments(command, args, cmdIndex);
    
    // - File exists check
    dentry_t executing_file;
    if (read_dentry_by_name(cmd, &executing_file) == -1) {
        sti();
        return -1;
    }

    // - File executable check
    
    int8_t magic_numbers[NUM_ELF];

    // Read first 4 bytes of file
    if (read_data(executing_file.inode_num, 0, magic_numbers, NUM_ELF) == -1) {return -1;}
    // Compare magic numbers to see if file is valid executable
    if (magic_numbers[0] != MAGICB0 || magic_numbers[1] != MAGICB1
        || magic_numbers[2] != MAGICB2 || magic_numbers[3] != MAGICB3) {
            sti();
            return -1;
        }

    /* NOTE PAGE 21 OF MP3 DOC */
    
    // Get Program Entry Point : Bytes 24-27 of executable file
    uint32_t prog_eip;
    if (read_data(executing_file.inode_num, PROG_EP_OFFSET, (int8_t*)&prog_eip, NUM_ELF) == -1) {
        sti();
        return -1;
    }

    /* When processing the execute system call, your code should make a note of the entry
    point, and then copy the entire file to memory starting at virtual address 0x08048000. 
    It then must jump to the entry point of the program to begin execution. */
    
    // Check if Process Control Block can execute program?

    int pid_num;
    for (pid_num = 0; pid_num < MAX_PCBS; pid_num++){
        if (pid_arr[pid_num] == 0){
            pid_arr[pid_num] = 1;
            pid_num++;
            break;
        }
        if (pid_num == (MAX_PCBS - 1)) {
            printfk("Process Cap Reached: ");
            sti();
            return 256;
        }
    }

    // COMMENT BELOW LINE OUT TO SHOW EXCEPTION HANDLING
    // 32 PAGE for 128 MB Virtual Mem
    page_directory[32].pde_4mb.frame = (MB_8 + ((pid_num-1)  * MB_4)) >> 22; //shift by 22 to offset 4MB pages

    // FLUSH TLB
    asm volatile (
        "movl %%cr3, %%eax ;"
        "movl %%eax, %%cr3 ;"
        :
        :
        :"eax","cc"
    );

    int prog_load_addr = PROG_ADDRESS; // set address for executable image
    int prog_stack_addr = (USER_ADD + MB_4) - sizeof(uint32_t); // 132 MB

    if (read_data(executing_file.inode_num, 0, (int8_t*)prog_load_addr, ALOT) == -1) {
        sti();
        return -1;
    }

    pcb_t * pcb = get_active_pcb(pid_num);

    // BUILD PCB
    build_pcb(pcb, prog_eip, args, pid_num);

    // pcb_t * par_pcb = get_active_pcb(pcb->parent_pid);

    // Save parent process esp and esb

    uint32_t par_ebp, par_esp;

    asm volatile (
        // Save ESP
        "movl %%esp, %0"
        : "=r" (par_esp)
        :
        : "cc"

    );

    asm volatile (
        // Save EBP
        "movl %%ebp, %0"
        : "=r" (par_ebp)
        :
        : "cc"

    );

    pcb->ESP_prev = par_esp;
    pcb->EBP_prev = par_ebp;

    // par_pcb->ebp = par_ebp;         // epb, esp = meant for task switch usage in scheduling.....
    // par_pcb->esp = par_esp;

    uint16_t user_ds = USER_DS;
    uint16_t user_cs = USER_CS;

    // TSS Switch
    tss.ss0 = KERNEL_DS;
    tss.esp0 = get_active_kernel_stack_addr(terminals[sched_term].active_pid);
    pcb->tss_esp0 = get_active_kernel_stack_addr(terminals[sched_term].active_pid);

    
    sti();

    // NOT USING MACROS
     asm volatile(
        "movw %%ax, %%ds ;"
        "pushl %%eax ;"             // push user data seg
        "movl %%ebx, %%eax ;"
        "pushl %%eax ;"             // push user stack pointer

        "pushfl ;"                  // Pushing the flags register (EFLAGS) onto the stack
        "popl %%ebx ;"                // Does IRET auto push op flags? thoughts?? -- ANSWER: NO
        "orl $0x200, %%ebx ;"         // orl $x0200, %%some ... to enable interups
        "pushl %%ebx ;"

        "pushl %%ecx ;"             // push user cs 
        "pushl %%edx ;"             // push return addr

        "iret ;"
        :
        : "d" (prog_eip), "a" (user_ds), "c" (user_cs), "b" (prog_stack_addr)
        : "memory"

    );

   return 0;

}


/* 
 *   int32_t halt (int8_t status)
 *   DESCRIPTION: Halts a process and returns control to the base shell or another process.
 *   INPUTS: 
 *     - status: The exit status of the process to be halted.
 *   OUTPUTS: None
 *   RETURN VALUE: 0 (always, unless there's an error)
 *   SIDE EFFECTS: Closes all files, updates PID data, flushes TLB, and returns control to another process.
 */

int32_t halt (int8_t status) { 

    cli();

    int i;

    int ret_val;
    
    if (status == EXCEPTION_STATUS){
        ret_val = RETURN_EXCEPTION;
    } else {
        ret_val = status;
    }
    
    // terminal_t * active_term = get_running_term();

    int pid_num = terminals[sched_term].active_pid;

    pcb_t * current_pcb = get_active_pcb(pid_num);
    pid_arr[pid_num - 1] = 0;
    pid_num--;

    //now case:1 that I am on the base proccess where PID is = 0 so its base shell and I just return to shell. Also have to set up registers for returning to SHELL
    if(current_pcb->pid == terminals[sched_term].base_pid){ // <-vikram....imooo should be the current_pcb->some pid?? //sasme shit tho

        // IRET Assembly --- maybe outside this if block if needed!
        // return -1;

        execute((uint8_t*)"shell");
    } 
    // Close all files
    //ALSO  FD STUFF THAT EACH EXECUTE OPENS NEEDS TO BE SHUT SOWN
    for (i = 0; i < NUM_FDS; i++) { // its sating online we should start i at 2.... closing stdin and stdout can casue us to bootloop maybes
        close(i);
    }

    //update the now previous closed PID and the mapping of the new PID

    //Update the newe frame of the aressing into the PID
    // 32 PAGE for 128 MB Virtual Mem
    page_directory[32].pde_4mb.frame = ((MB_8 + ((current_pcb->parent_pid - 1) * MB_4)) >> 22); //shift by 22 to offset 4MB pages

    // FLUSH TLB
    asm volatile (
        "movl %%cr3, %%eax ;"
        "movl %%eax, %%cr3 ;"
        :
        :
        :"eax","cc"
    );

    // Do the same thing again:
    tss.ss0 = current_pcb->prev_tss_SS;
    tss.esp0 = current_pcb->prev_tss_ESP0;

    terminals[sched_term].active_pid = current_pcb->parent_pid;
    // active_term->num_pids--;

    // if (active_term->num_pids == 0){
    //     execute((int8_t*)"shell");
    // }

    // return for halt ebp esp staus
    // returnFromHalt(current_pcb->ESP_prev, current_pcb->EBP_prev, ret_val);

    sti();
    asm volatile(
        "movl %0, %%esp ;"
        "movl %1, %%ebp  ;"
        "movl %2, %%eax  ;"
        "leave ;"
        "ret ;"
        :
        : "r" (current_pcb->ESP_prev), "r" (current_pcb->EBP_prev), "r" (ret_val)
        : "memory"
    );

    return 0;
    
}


/*
 * int32_t getargs(int8_t* buf, int32_t nbytes)
 * DESCRIPTION: Copies the command line arguments of the currently executing process into a buffer.
 * INPUTS:
 *   - buf: Pointer to the buffer where command line arguments will be stored.
 *   - nbytes: Maximum number of bytes to copy into the buffer.
 * OUTPUTS: None
 * RETURN VALUE:
 *   - 0 on success, -1 on failure
 * SIDE EFFECTS: None
 */

int32_t getargs (int8_t* buf, int32_t nbytes) {
    if (buf == NULL) return -1;

    int pid_num;
    for (pid_num = 0; pid_num < MAX_PCBS; pid_num++){
        if (pid_arr[pid_num] == 0){
            break;
        }
    }

    pcb_t * pcb = get_active_pcb(pid_num);

    if (pcb->args[0] == '\0') return -1;

    memcpy(buf, (int8_t*)pcb->args, FILENAME_LEN);

    return 0;

}

/*
 * int32_t vidmap (int8_t** screen_start)
 * DESCRIPTION: Maps the video memory to a user-level virtual address.
 * INPUTS:
 *   - screen_start: Pointer to a pointer where the screen start address will be stored.
 * OUTPUTS: None
 * RETURN VALUE:
 *   - 0 on success, -1 on failure
 * SIDE EFFECTS: Modifies the page directory and updates the CR3 register.
 */

int32_t vidmap (int8_t** screen_start){
    cli();

    if (screen_start == NULL){
        return -1;
    }

    int address = (uint32_t)screen_start;

    if (address < USER_ADD || address >= (USER_ADD + MB_4)){
        return -1;
    }
    // VIDEO MEM 33 INDEX IN PD FOR 132 MB
    page_directory[33].pde_4kb.frame = (uint32_t)page_table_vidmap >> 12; //SHIFT BY 12 COZ OF 4 KB PAGES


    asm volatile (
        "movl %%cr3, %%eax ;"
        "movl %%eax, %%cr3 ;"
        :
        :
        :"eax","cc"
    );

    *screen_start = (int8_t*)(0x8400000 + ((KB_4)*(sched_term))); // 0x8400000 = 132 MB VIRTUAL ADDR START
    sti();
    return 0;
}

int32_t set_handler (int32_t signum, void* handler_address) {return 0;}
int32_t sigreturn (void) {return 0;}
