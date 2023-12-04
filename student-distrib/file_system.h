#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_

#include "rtc.h"
#include "lib.h"
#include "types.h"
#include "keyboard.h"

#define FILENAME_LEN    32
#define DATA_BLOCK_NUM_INODE  1023
#define NUM_DENTRIES_BOOT  63
#define NUM_FDS     8
#define SIZE_4KB     4096
#define FILE_FILETYPE   2
#define DIR_FILETYPE   1
#define USER_FILETYPE   0
#define CHARS_ON_LINE   79

#define MAX_PCBS        6

extern int pid_arr[MAX_PCBS];

// dentry struct
typedef struct dentry {
    int8_t filename[FILENAME_LEN]; // name
    int32_t filetype; // type 0-2
    int32_t inode_num; // inode num
    int8_t reserved[24]; // 24 reserved chars in dentry
} dentry_t;

// inode struct
typedef struct inode {
    int32_t length; // num bytes
    int32_t data_block_num[DATA_BLOCK_NUM_INODE]; // data block arr
} inode_t;

// boot block struct
typedef struct boot_block {
    int32_t dir_count; // num dirs
    int32_t inode_count; // num inode
    int32_t data_count; // num data blocks
    int8_t reserved[52]; // 52 reserved chars in boot block
    dentry_t direntries[NUM_DENTRIES_BOOT]; // dentry addresses
} boot_block_t;

// file ops struct -- jumptable
struct file_ops {
    int32_t (*open)(const uint8_t* filename);
    int32_t (*close)(int32_t fd);
    int32_t (*write)(int32_t fd, const uint8_t* buf, int32_t nbytes);
    int32_t (*read)(int32_t fd, uint8_t* buf, int32_t nbytes);
};

// file descriptor struct
typedef struct file_descriptor {
    struct file_ops* fops;  // file operations jump table
    int32_t inode_num;      // inode number for data files (0 for directories and RTC)
    int32_t file_offset;  // current position in the file
    int32_t flags;         // flags to mark the status of the file descriptor
    int8_t * fname;
} fd_t;

typedef struct pcb {
    uint32_t pid;
    uint32_t parent_pid;
    
    uint32_t user_prog_ep;

    uint8_t args[FILENAME_LEN];

    uint32_t ESP_prev;
    uint32_t EBP_prev;

    uint32_t prev_tss_ESP0;
    uint32_t tss_esp0;          // Running Process' tss esp0;
    uint32_t prev_tss_SS;

    uint32_t ebp;
    uint32_t esp;

    fd_t file_desc_arr[NUM_FDS];
    int num_open_descriptors;

} pcb_t;

// initialize file system
void filesys_init(int32_t start_addr);

// load dentry data to dentry parameter by name query
int32_t read_dentry_by_name(const int8_t* fname, dentry_t* dentry);

// load dentry data to dentry parameter by index query
int32_t read_dentry_by_index(int32_t index, dentry_t* dentry);

// read data into the buffer byte by byte
int32_t read_data (int32_t inode, int32_t offset, int8_t* buf, int32_t length);

// open a file and put in fd array
int32_t file_open(const uint8_t* filename);

// close a file and remove from fd array
int32_t file_close(int32_t fd);

// write to file (not supported in our kernel)
int32_t file_write(int32_t fd, const uint8_t* buf, int32_t nbytes);

// read data from file
int32_t file_read (int32_t fd, uint8_t* buf, int32_t nbytes);    

// write to dir (not supported in our kernel)
int32_t dir_write(int32_t fd, const uint8_t* buf, int32_t nbytes);

// read file names in dir
int32_t dir_read(int32_t fd, uint8_t* buf, int32_t nbytes);

int32_t null_read(int32_t fd, uint8_t* buf, int32_t nbytes);

int32_t null_write(int32_t fd, const uint8_t* buf, int32_t nbytes);

void update_file_pcb(pcb_t * curr);

#endif  // FILESYSTEM_H
