#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "file_system.h"
#include "keyboard.h"
#include "rtc.h"
#include "system_calls.h"

// make clean && make 2> output.txt

#define PASS 1
#define FAIL 0

#define KERNEL 0x400000
#define VIDEO 0xB8000
#define KB 0x1000
#define MB 0x100000

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $30");
}


/* Checkpoint 1 tests */

/* IDT Test - Example
 * 
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int idt_test(){
	TEST_HEADER;

	int i;
	int result = PASS;
	for (i = 0; i < 31; ++i){
		if(i == 9 || i == 15 || (i >= 22 && i <= 27)){}
		else if ((idt[i].offset_15_00 == NULL) && 
			(idt[i].offset_31_16 == NULL)){
				printf("%d <----", i);
			result = FAIL;
		}
	}

	// Cause an exception by hard coding the int
		// Uncomment one line to check specific exception

	// asm volatile("int $0");
	// asm volatile("int $1");
	// asm volatile("int $2");
	// asm volatile("int $3");
	// asm volatile("int $4");
	// asm volatile("int $5");
	// asm volatile("int $6");
	// asm volatile("int $7");
	// asm volatile("int $8");
	// asm volatile("int $9");
	// asm volatile("int $10");
	// asm volatile("int $11");
	// asm volatile("int $12");
	// asm volatile("int $13");
	// asm volatile("int $14");
	// asm volatile("int $16");
	// asm volatile("int $17");
	// asm volatile("int $18");
	// asm volatile("int $19");
	// asm volatile("int $20");
	// asm volatile("int $21");
	// asm volatile("int $28");
	// asm volatile("int $29");
	// asm volatile("int $30");

	return result;
}

/* 
 * div0_test
 *   DESCRIPTION: Cause a divide by 0 exception
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: 0 -- shouldn't ever be reached
 *   SIDE EFFECTS: cause exception in kernel
 */
int div0_test(){
	TEST_HEADER;
	int y = 0;
	int x = 1;
	int z = x/y;
	printf("%d", z);
	return 0;
}

/* 
 * assertionCheck
 *   DESCRIPTION: Cause an exception by hard coding the int
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: 0 -- shouldn't ever be reached
 *   SIDE EFFECTS: cause desired exception in kernel
 */
int assertionCheck(){
	TEST_HEADER;
	assertion_failure();
	return 0;
}

/* 
 * page_fault_test
 *   DESCRIPTION: Cause a page fault exception
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: 0 -- shouldn't ever be reached
 *   SIDE EFFECTS: cause page fault exception in kernel
 */
int page_fault_test(){
	TEST_HEADER;

	// Access an invalid memory address to trigger a page fault
	int* ptr = (int*)(KERNEL-1);
	int x = *ptr;

	printf("%x", x);

	return 0;
}

/* 
 * pageTesting
 *   DESCRIPTION: Dereferences kernel memory and video memory
 *   INPUTS: none
 *   OUTPUTS: PASS/FAIL
 *   RETURN VALUE: 1 - PASS
 *   SIDE EFFECTS: Prints addressses and indexes being checked
 */
int pageTesting(){
	TEST_HEADER;

	int i;
	int j;

	// KERNEL Paging test:
	// KERNAL + 4 * MB = 8MB
	for(i = KERNEL; i < (KERNEL + 4*MB); i++) {
		char* ptr = (char*)i;
		int x = *ptr;
		printf("deref i: %x %d\n", i, x); // last index should fail
	}

	// VIDEO Memory Test:

	for(j = VIDEO; j < (VIDEO + KB); j++) {
		char* ptr = (char*)j;
		int x = *ptr;
		printf("deref j: %x %d\n", j, x); // last index should fail
	}

	return PASS;
}

/* 
 * syscall_test
 *   DESCRIPTION: Accesses and calles interrupt 0x80
 *   INPUTS: none
 *   OUTPUTS: PASS/FAIL
 *   RETURN VALUE: 1 - shouldn't be reached
 *   SIDE EFFECTS: Prints message and stops program
 */
int syscall_test() {
    asm volatile("int $0x80");
    return PASS;
}

/* 
 * null_dereferenctTest
 *   DESCRIPTION: Dereferences a null pointer
 *   INPUTS: none
 *   OUTPUTS: PASS/FAIL
 *   RETURN VALUE: 1 - shouldn't be reached
 *   SIDE EFFECTS: Segmentation Fault
 */
int null_dereferenceTest(){
	TEST_HEADER;
	int* ptr = NULL;
	int test = *ptr;
	if(test != -1){
		return PASS;
	}
	return PASS;
}

/* Checkpoint 2 tests */

/* 
 * text_open_and_read_test
 *   DESCRIPTION: Test reading from a file and printing to screen.
 *   INPUTS: none
 *   OUTPUTS: Writes data to local buffer.
 *   RETURN VALUE: PASS/FAIL
 *   SIDE EFFECTS: Prints data to screen.
 */
int text_open_and_read_test(){
	TEST_HEADER;
	// int8_t buf[10000];			// test with 10000 bytes
	// // int8_t buf[40];			// CHECK SHORT BUFFER

	// int8_t * filename = "verylargetextwithverylongname.txt";
	// int32_t fd;
	// int32_t nbytes = 10000;		// test with 10000 bytes
	// // int32_t nbytes = 40;		// CHECK SHORT BUFFER

	// int32_t ret;
	// clear(); // clear screen

	// fd = file_open((uint8_t*)filename); // open file

	// if (fd == -1) {
    //     // handle error when file_open fails
    //     printf("Error opening the file\n");
    //     return -1;
    // }

	// // print some info
	// printf("num open descs: %d\n", num_open_descriptors);
	// printf("file name: ");
	// printf(file_desc_arr[fd].fname);
	// printf("\n");

	// printf("Data:\n");

    // ret = file_desc_arr[fd].fops->read(fd, (uint8_t*)buf, nbytes);

    // if (ret == -1) {
    //     // handle error when file_read fails
    //     printf("Error reading the file\n");
    //     return -1;
    // }

	// terminal_write(0, (uint8_t*) buf, ret);

	// printf("Num bytes read: %d\n", ret);

	return PASS;
	
}

int text_read_with_offset_test(){
	TEST_HEADER;
	// int8_t buf[200];			// test with 10000 bytes

	// int8_t * filename = "verylargetextwithverylongname.txt";
	// int32_t fd;
	// int32_t nbytes = 200;		// test with 10000 bytes

	// int32_t ret;
	// clear(); // clear screen

	// fd = file_open((uint8_t*)filename); // open file

	// if (fd == -1) {
    //     // handle error when file_open fails
    //     printf("Error opening the file\n");
    //     return -1;
    // }

	// // print some info
	// printf("num open descs: %d\n", num_open_descriptors);
	// printf("file name: ");
	// printf(file_desc_arr[fd].fname);
	// printf("\n");

	// printf("Data:\n");

    // ret = file_desc_arr[fd].fops->read(fd, (uint8_t*)buf, nbytes);

    // if (ret == -1) {
    //     // handle error when file_read fails
    //     printf("Error reading the file\n");
    //     return -1;
    // }

	// terminal_write(0, (uint8_t*) buf, ret);

	// printf("Num bytes read: %d\n", ret);

	// printf("Part 2:\n");

	// ret = file_desc_arr[fd].fops->read(fd, (uint8_t*)buf, nbytes);

    // if (ret == -1) {
    //     // handle error when file_read fails
    //     printf("Error reading the file\n");
    //     return -1;
    // }

	// terminal_write(0, (uint8_t*) buf, ret);

	// printf("Num bytes read: %d\n", ret);

	return PASS;
	
}

/* 
 * exec_open_and_read_test
 *   DESCRIPTION: Test reading from an executable file and printing to screen.
 *   INPUTS: none
 *   OUTPUTS: Writes data to local buffer.
 *   RETURN VALUE: PASS/FAIL
 *   SIDE EFFECTS: Prints data to screen.
 */
int exec_open_and_read_test(){
	TEST_HEADER;
	// int8_t buf[10000];			// test with 10000 bytes
	// int8_t * filename = "shell";
	// int32_t fd;
	// int32_t nbytes = 10000;		// test with 10000 bytes
	// int32_t ret;
	// clear(); // clear screen

	// fd = file_open((uint8_t*)filename); // open file

	// if (fd == -1) {
    //     // handle error when file_open fails
    //     printf("Error opening the file\n");
    //     return -1;
    // }

	// // print some info
	// printf("num open descs: %d\n", num_open_descriptors);
	// printf("file name: ");
	// printf(file_desc_arr[fd].fname);
	// printf("\n");

	// printf("Data:\n");

    // ret = file_desc_arr[fd].fops->read(fd, (uint8_t*)buf, nbytes);

    // if (ret == -1) {
    //     // handle error when file_read fails
    //     printf("Error reading the file\n");
    //     return -1;
    // }

	// terminal_write_executable(0, (uint8_t*) buf, ret); // write executable data to screen

	// printf("Num bytes read: %d\n", ret);

	return PASS;
	
}

/* 
 * dir_open_and_read_test
 *   DESCRIPTION: Test reading from the directory and printing to screen.
 *   INPUTS: none
 *   OUTPUTS: Writes filenames and types to local buffer.
 *   RETURN VALUE: PASS/FAIL
 *   SIDE EFFECTS: Prints filenames and types to screen.
 */
int dir_open_and_read_test(){
	TEST_HEADER;
	// int8_t buf[80]; 		// 80 chars = one line on kernel screen
	// int8_t * filename = "."; // dir
	// int32_t fd;
	// int32_t nbytes = 80;	// 80 chars = one line on kernel screen
	// clear(); // clear screen

	// fd = file_open((uint8_t*)filename); // open

	// if (fd == -1) {
    //     // handle error when file_open fails
    //     printf("Error opening the file\n");
    //     return -1;
    // }

	// // print info
	// printf("num open descs: %d\n", num_open_descriptors);
	// printf("file name: ");
	// printf(file_desc_arr[fd].fname);
	// printf("\n");

	// // FOR DIRS TESTING
	// while(file_desc_arr[fd].fops->read(fd, (uint8_t*)buf, nbytes)){ // while we haven't reached end of files
	// 	printf("%s\n", buf);
	// }

	return PASS;
	
}

/* 
 * terminal_test
 *   DESCRIPTION: Copies user input text and prints to screen to test terminal drivers.
 *   INPUTS: none
 *   OUTPUTS: Writes user input to local buffer.
 *   RETURN VALUE: PASS/FAIL
 *   SIDE EFFECTS: Prints input to screen.
 */
int terminal_test() {
	int chars_read;
	uint8_t buffff[128]; // buffer
	while(1){
		chars_read = terminal_read(0, buffff, sizeof(buffff)); // read and write
		terminal_write(0, buffff, chars_read);
	}

	return PASS;
}

/* 
 * rtc_customTest
 *   DESCRIPTION: Test RTC driver.
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: PASS/FAIL
 *   SIDE EFFECTS: Reads and writes from rtc file.
 */
int rtc_customTest(){
	TEST_HEADER;
	int32_t desiredFreq = 512;

	rtc_write(0, &desiredFreq, 4);

	while (1) {
		// printf("4444444444\n");
		rtc_read(0, NULL, 0);
		printf("1\n");
	}

	return PASS;
}


/* Checkpoint 3 tests */

int file_ops_w_pcb(){
	TEST_HEADER;
// 	int ret;
// 	int i;
// 	pid_counter++;

// 	build_pcb(get_active_pcb(pid_counter), 100);

// 	ret = open("ls");

// 	if (ret == -1) return FAIL;

// 	uint8_t * mybuf[100000];

// 	ret = read(2, mybuf, 100000);
// 	if (ret == -1) return FAIL;

// 	terminal_write(0, mybuf, ret);
// 	printf("\n%d", ret);
	
// 	for (i = 0; i < 7; i++){
// 		ret = open(".");
// 		if (ret == -1) return FAIL;
// 		printf("\nopened . for the %d time", ret);
// 		printf("num open: %d\n", get_active_pcb(pid_counter)->num_open_descriptors);
// 	}

	return PASS;
}

int test_dentry(){
	TEST_HEADER;
// 	dentry_t curr;
// 	read_dentry_by_name("ls", &curr);
// 	uint8_t buf[1000];
// 	read_data(curr.inode_num,0,buf,1000);

// 	terminal_write(0, buf, 1000);

	return PASS;
}

/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests() {

	// TEST_OUTPUT("idt_test", idt_test());
	// launch your tests here

	// __CHECKPOINT 1__

	// TEST_OUTPUT("divide zero test", div0_test());
	// TEST_OUTPUT("pageTesting test", pageTesting());
	// TEST_OUTPUT("manual exception", assertionCheck());
	// TEST_OUTPUT("System Call Test", syscall_test());
	// TEST_OUTPUT("Null Deref Test", null_dereferenceTest());
	// TEST_OUTPUT("PageFault Test - Kernel", page_fault_test());

	// Checkpoint 2
	// TEST_OUTPUT("TERMINAL TEST", terminal_test());
	// TEST_OUTPUT("text read test", text_open_and_read_test());
	// TEST_OUTPUT("text read with offset test", text_read_with_offset_test());
	// TEST_OUTPUT("executable read test", exec_open_and_read_test());
	// TEST_OUTPUT("dir read test", dir_open_and_read_test());
	// TEST_OUTPUT("rtc_customTest TEST", rtc_customTest());

	// Checkpoint 3

	// TEST_OUTPUT("testing pcb and sys calls", file_ops_w_pcb());
	// TEST_OUTPUT("test ls dentry", test_dentry());

}
