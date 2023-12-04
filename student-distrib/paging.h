#ifndef _PAGING_H
#define _PAGING_H

#ifndef ASM

#include "x86_desc.h" 

#define MAX_ENTRIES 1024
#define BYTES_TO_ALIGN_TO_4kb 4096 //size of page
#define KERNEL_ADD 0x400000
#define USER_ADD 0x8000000
#define VIDEO_MEM_ADDR 0xB8 // Place where vid mem address exists in physical memory!
// Function to initialize paging
extern void paging_init();

#define MB_8 0x800000
#define MB_4 0x400000

#define TERM1_VRAM      0xB9
#define TERM2_VRAM      0xBA
#define TERM3_VRAM      0xBB




//Page directory and page table entries
typedef struct __attribute__((packed)) page_directory_entry_4kb{
    uint32_t present      : 1;  // Bit 0: Page is present in memory
    uint32_t rw           : 1;  // Bit 1: Read/Write permission
    uint32_t user         : 1;  // Bit 2: User/Supervisor mode
    uint32_t pwt          : 1;  // Bit 3: Page-level write-through
    uint32_t pcd          : 1;  // Bit 4: Page-level cache disable
    uint32_t accessed     : 1;  // Bit 5: Page has been accessed
    uint32_t avl          : 1;  // Bit 6: available
    uint32_t ps           : 1;  // Bit 7: ps should be 0
    uint32_t available    : 4;  // Bits 8-11: Available for system programmer's use
    uint32_t frame        : 20; // Bits 12-31: Page base address (physical address)
} page_directory_entry_4kb;

typedef struct __attribute__((packed)) page_directory_entry_4mb{
    uint32_t present      : 1;  // Bit 0: Page is present in memory
    uint32_t rw           : 1;  // Bit 1: Read/Write permission
    uint32_t user         : 1;  // Bit 2: User/Supervisor mode
    uint32_t pwt          : 1;  // Bit 3: Page-level write-through
    uint32_t pcd          : 1;  // Bit 4: Page-level cache disable
    uint32_t accessed     : 1;  // Bit 5: Page has been accessed
    uint32_t dirty        : 1;  // Bit 6: Dirty
    uint32_t ps           : 1;  // Bit 7: Page Size, 1 indicates 4mb
    uint32_t global       : 1;  // Bit 8: Global page (Ignored)
    uint32_t available    : 3;  // Bits 9-11: Available for system programmer's use
    uint32_t pat          : 1;  // Bit 12: PAT 
    uint32_t addr         : 8;  // Bits 13-20
    uint32_t rsvd         : 1;  // Bit 21 Reserved to be 0
    uint32_t frame        : 10; // Bits 22-31: Page base address (physical address)
} page_directory_entry_4mb;

typedef struct __attribute__((packed)) page_table_entry_t{
    uint32_t present      : 1;  // Bit 0: Page is present in memory
    uint32_t rw           : 1;  // Bit 1: Read/Write permission
    uint32_t user         : 1;  // Bit 2: User/Supervisor mode
    uint32_t pwt          : 1;  // Bit 3: Page-level write-through
    uint32_t pcd          : 1;  // Bit 4: Page-level cache disable
    uint32_t accessed     : 1;  // Bit 5: Page has been accessed
    uint32_t dirty        : 1;  // Bit 6: Dirty
    uint32_t pat          : 1;  // Bit 7: Page Table Attribute Index
    uint32_t global       : 1;  // Bit 8: Global Page
    uint32_t available    : 3;  // Bits 9-11: Available for system programmer's use
    uint32_t frame        : 20; // Bits 12-31: Page base address (physical address)
} page_table_entry_t;

typedef union page_directory_t{
    uint32_t bs;
    page_directory_entry_4kb pde_4kb;
    page_directory_entry_4mb pde_4mb;
}page_directory_t;



page_directory_t  page_directory[MAX_ENTRIES] __attribute__((aligned(BYTES_TO_ALIGN_TO_4kb)));
page_table_entry_t  page_table[MAX_ENTRIES] __attribute__((aligned(BYTES_TO_ALIGN_TO_4kb)));
page_table_entry_t  page_table_vidmap[MAX_ENTRIES] __attribute__((aligned(BYTES_TO_ALIGN_TO_4kb)));

extern void loadPageDirectory(unsigned int*);
extern void otherenable();

#endif /* ASM */
#endif /* PAGING_H */
