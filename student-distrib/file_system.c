#include "file_system.h"

static int32_t fs_start; // start address of file system module
static int32_t num_inodes;
static int32_t num_direntries;
static int32_t D;
static int32_t N;
static boot_block_t* myBoot;

static pcb_t * file_pcb = NULL;

int pid_arr[MAX_PCBS] = {0,0,0,0,0,0};

void update_file_pcb(pcb_t * curr){
    file_pcb = curr;
}

/**
 * filesys_init
 *   DESCRIPTION : This function initializes the file system and assigns global variable data.
 *                 Also opens the first two fds, stdin and stdout.
 *   INPUTS :  start_addr -- Starting address of the filesystem in memory
 *   OUTPUTS :  writes data to the global vars of this file including boot block
 *   RETURN VALUE :  None
 *   SIDE EFFECTS : Initializes stdin and stdout.
 */
void filesys_init(int32_t start_addr){ // save start addr
    fs_start = start_addr;
    myBoot = (boot_block_t*)fs_start; // extract boot block
    num_inodes = myBoot->inode_count; // save info
    num_direntries = myBoot->dir_count;
    D = myBoot->data_count;
    N = num_inodes + 1;

}

/**
 * read_dentry_by_name
 *   DESCRIPTION : Finds the corresponding dentry to the filename query and writes data to dentry parameter.
 *   INPUTS :  fname -- name of file
 *             dentry -- dentry struct to write to
 *   OUTPUTS :  Writes data to the dentry struct
 *   RETURN VALUE :  0 -- success
 *                   1 -- fail (filename doesn't exist)
 *   SIDE EFFECTS : None
 */
int32_t read_dentry_by_name(const int8_t* fname, dentry_t* dentry){
    int i;
    for (i = 0; i < num_direntries; i++){
        dentry_t * curr = &(myBoot->direntries[i]); // get dentry
        int32_t dentry_len = strlen(curr->filename);
        int32_t fname_len = strlen(fname);

        if (dentry_len > FILENAME_LEN) dentry_len = FILENAME_LEN; // if name len more than 32 truncate to 32
        if (fname_len > FILENAME_LEN) fname_len = FILENAME_LEN;

        if (fname_len == dentry_len && strncmp(fname, curr->filename, fname_len) == 0){
            *dentry = *curr; // if same name copy to dentry param
            return dentry_len;
        }
    }

    return -1; // file doesn't exist
}

/**
 * read_dentry_by_index
 *   DESCRIPTION : Finds the corresponding dentry to the index query and writes data to dentry parameter.
 *   INPUTS :  index -- file index
 *             dentry -- dentry struct to write to
 *   OUTPUTS :  Writes data to the dentry struct
 *   RETURN VALUE :  0 -- success
 *                   1 -- fail (index out of bounds)
 *   SIDE EFFECTS : None
 */
int32_t read_dentry_by_index(int32_t index, dentry_t* dentry){
    if (index < num_direntries){
        dentry_t * curr = &(myBoot->direntries[index]); // copy index to dentry
        *dentry = *curr;

        int32_t dentry_len = strlen(curr->filename);
        if (dentry_len > FILENAME_LEN) dentry_len = FILENAME_LEN;

        return dentry_len;
    }

    return -1; // file doesn't exist
}

/**
 * read_data
 *   DESCRIPTION : Reads data from data blocks for corresponding inode and write to buffer.
 *                 Writes either length bytes or until end of file.
 *   INPUTS :  inode -- inode number
 *             offset -- offset in file to start at
 *             buf -- data buffer
 *             length -- number of bytes to read
 *   OUTPUTS :  Writes data to the buf
 *   RETURN VALUE :  num_written -- number of bytes written to the buffer
 *   SIDE EFFECTS : None
 */
int32_t read_data (int32_t inode, int32_t offset, int8_t* buf, int32_t length){

    if (inode >= num_inodes) return -1; // out of bounds

    inode_t * myInode = (inode_t*)(fs_start + (inode + 1) * SIZE_4KB); // get to inode start
    int num_bytes_in_file = myInode->length; // max bytes in file
    int num_written = 0;
    
    while ((num_written + offset) < num_bytes_in_file && num_written < length){
        int block_idx = (num_written + offset)/SIZE_4KB;
        if (block_idx > (num_bytes_in_file/SIZE_4KB)) return -1; // maybe we don't need this check?
        int curr_block = myInode->data_block_num[block_idx]; // get curr_block
        if (curr_block > D) return -1; // out of bounds
        
        int8_t * curr_byte = (int8_t*)fs_start;
        curr_byte += (SIZE_4KB * (N + curr_block)); // get to data block start
        curr_byte += (num_written + offset) % SIZE_4KB; // get to byte

        buf[num_written] = *curr_byte; // write curr byte

        num_written++;
    }

    return num_written; // wrote length bytes

}

/**
 * file_open
 *   DESCRIPTION : Opens a file by name and builds fd_t struct intsance which is put in the global fd array.
 *   INPUTS :  filename -- name of file to open
 *   OUTPUTS :  Builds fd_t struct instance and puts it in the gloabal fd array.
 *              Increments number of files open.
 *   RETURN VALUE :  fd -- file opened
 *                   -1 -- fail (either filename not found or out of fd spots)
 *   SIDE EFFECTS : Initializes offset to 0.
 */
int32_t file_open(const uint8_t* filename) {
    return 0;
}

/**
 * file_close
 *   DESCRIPTION : Closes file and clears corresponding file instance
 *   INPUTS :  fd -- file number to close
 *   OUTPUTS :  Closes file instance in fd_array.
 *              Decrements number of global fds open.
 *   RETURN VALUE :   0 -- success
 *                   -1 -- fail (fd out of bounds)
 *   SIDE EFFECTS : None
 */
int32_t file_close(int32_t fd){
    return 0;
}

/**
 * file_write
 *   DESCRIPTION : Supposed to write to file. Not supported in our kernel.
 *   INPUTS :  fd -- file number to write
 *             buf -- data buffer
 *             nbytes -- number of bytes to write
 *   OUTPUTS :  None
 *   RETURN VALUE : -1 -- fail (not supported)
 *   SIDE EFFECTS : None
 */
int32_t file_write(int32_t fd, const uint8_t* buf, int32_t nbytes){
    return -1; // no write in our kernel
}

/**
 * file_read
 *   DESCRIPTION : Read nbytes bytes from file into data buffer. Calls read_data to do so.
 *   INPUTS :  fd -- file number to read from
 *             buf -- data buffer
 *             nbytes -- number of bytes to read
 *   OUTPUTS :  Reads data from files and writes into buffer
 *   RETURN VALUE : ret_val -- number of bytes read
 *                  -1 -- fail (fd out of bounds)
 *   SIDE EFFECTS : Updates file offset. Calls read_data.
 */
int32_t file_read (int32_t fd, uint8_t* buf, int32_t nbytes){

    if (fd < 0 || fd >= file_pcb->num_open_descriptors) return -1;

    fd_t * curr = &(file_pcb->file_desc_arr[fd]);

    if (curr->inode_num == 0) return -1; // make sure its a file

    int32_t inode = curr->inode_num; // get data from file
    int32_t offset = curr->file_offset;
    
    int ret_val = read_data(inode, offset, (int8_t*)buf, nbytes); // call read data

    if (ret_val == -1) return -1; // error checking

    curr->file_offset += ret_val;

    return ret_val;

}

/**
 * dir_write
 *   DESCRIPTION : Supposed to write to dir. Not supported in our kernel.
 *   INPUTS :  fd -- file number to write
 *             buf -- data buffer
 *             nbytes -- number of bytes to write
 *   OUTPUTS :  None
 *   RETURN VALUE : -1 -- fail (not supported)
 *   SIDE EFFECTS : None
 */
int32_t dir_write(int32_t fd, const uint8_t* buf, int32_t nbytes){
    return -1; // no write in our kernel
}

/**
 * file_read
 *   DESCRIPTION : Read 1 file in the base directory's info to the data buffer.
 *   INPUTS :  fd -- ignored
 *             buf -- data buffer
 *             nbytes -- ignored
 *   OUTPUTS :  Reads directory names and file types and writes to buffer.
 *   RETURN VALUE : 0 -- done reading all file info. Reached end of dir.
 *                  -1 -- fail (fd out of bounds)
 *                  1 -- success
 *   SIDE EFFECTS : Updates file offset to go to next file in dir.
 */
int32_t dir_read(int32_t fd, uint8_t* buf, int32_t nbytes){

    // TO CAUSE EXCEPTION HANDLING UNCOMMENT BELOW

    // int * ptr = 0;

    // int tmp = *ptr;

    if (fd < 0 || fd >= file_pcb->num_open_descriptors) return -1;

    fd_t * curr = &(file_pcb->file_desc_arr[fd]);

    if (curr->inode_num != 0) return -1; // make sure its a dir

    if (curr->file_offset >= num_direntries){ // done reading return 0
        return 0;
    }

    dentry_t myDentry;
    
    int ret = read_dentry_by_index(curr->file_offset, &myDentry); // get curr dentry

    if (ret == -1) return -1;

    strncpy((int8_t*) buf, myDentry.filename, FILENAME_LEN);

    curr->file_offset++; // increment offset to go to next file

    return ret; // return 0 when done reading, 1 otherwise

}

// int32_t null_read(int32_t fd, uint8_t* buf, int32_t nbytes) {
//     return -1;
// }

// int32_t null_write(int32_t fd, const uint8_t* buf, int32_t nbytes) {
//     return -1;
// }

// int32_t null_open(const uint8_t* filename) {
//     return -1;
// }

// int32_t null_close(int32_t fd) {
//     return -1;
// }


// int32_t read_data (int32_t inode, int32_t offset, int8_t* buf, int32_t length){
    // if (inode > num_inodes) return -1; // out of bounds

    // inode_t * myInode = (inode_t*)(fs_start + (inode + 1) * SIZE_4KB); // get to inode start
    // int num_bytes_in_file = myInode->length; // max bytes in file
    // int num_written = 0;
    // int curr_offset = offset;
    // int num_left_to_write; 
    // if ((num_bytes_in_file - offset) < length){
    //     num_left_to_write = (num_bytes_in_file - offset);
    // } else{
    //     num_left_to_write = length;
    // }


    // while ((num_written < length) && ((num_written + offset) < num_bytes_in_file)){
    //     int inode_block_idx = (num_written + offset)/SIZE_4KB;
    //     if (inode_block_idx > (num_bytes_in_file/SIZE_4KB)) return -1; // maybe we don't need this check?

    //     int curr_block_num = myInode->data_block_num[inode_block_idx]; // get curr_block
    //     if (curr_block_num > D) return -1; // out of bounds if more than num data blocks

    //     int8_t * curr_start = (int8_t*)fs_start;
    //     curr_start += (SIZE_4KB * (N + curr_block_num)); // get to node start
    //     curr_start += curr_offset;
        
    //     if (curr_offset == 0 && num_left_to_write > SIZE_4KB){
    //         memcpy(buf, curr_start, SIZE_4KB);
    //         num_left_to_write -= SIZE_4KB;
    //         num_written += SIZE_4KB;
    //     } else if (num_left_to_write > (SIZE_4KB - curr_offset)){
    //         memcpy(buf, curr_start, SIZE_4KB - curr_offset);
    //         num_written += (SIZE_4KB - curr_offset);
    //         num_left_to_write -= (SIZE_4KB - curr_offset);
    //         curr_offset = 0;
    //     } else {
    //         memcpy(buf, curr_start, num_left_to_write);
    //         num_written += num_left_to_write;
    //         num_left_to_write = 0;
    //     }

    // }

    //     return num_written;

// }


