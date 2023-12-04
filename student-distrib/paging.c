#include "paging.h"

/* 
 * paging_init
 *   DESCRIPTION: Initializes the paging system for a kernel, sets up page directory
 *                and page table entries.
 *   INPUTS: None
 *   OUTPUTS: None
 *   RETURN VALUE: None
 *   SIDE EFFECTS: Initializes the paging system and sets up page directory and page tables.
 */

void paging_init() {

    unsigned int i; // Loop variable
    for(i = 0; i<MAX_ENTRIES; i++){
        if(i == 0){
            page_directory[i].pde_4kb.present = 1;
            page_directory[i].pde_4kb.rw = 1;
            page_directory[i].pde_4kb.user = 0;
            page_directory[i].pde_4kb.pwt = 0;
            page_directory[i].pde_4kb.pcd = 0;
            page_directory[i].pde_4kb.accessed = 0;
            page_directory[i].pde_4kb.avl = 0;
            page_directory[i].pde_4kb.ps = 0; // ps should be 0
            page_directory[i].pde_4kb.available = 0; // Set to the desired value
            page_directory[i].pde_4kb.frame  = (uint32_t)page_table >> 12; //shifted coz of the structre of where the address is! add starts at bit 12 
        }
        else if(i == 1){
            page_directory[i].pde_4mb.present = 1;
            page_directory[i].pde_4mb.rw = 1;
            page_directory[i].pde_4mb.user = 0;
            page_directory[i].pde_4mb.pwt = 0;
            page_directory[i].pde_4mb.pcd = 0;
            page_directory[i].pde_4mb.accessed = 0;
            page_directory[i].pde_4mb.dirty = 0;
            page_directory[i].pde_4mb.ps = 1;
            page_directory[i].pde_4mb.global = 0;
            page_directory[i].pde_4mb.available = 0;
            page_directory[i].pde_4mb.pat = 0;
            page_directory[i].pde_4mb.addr = 0;
            page_directory[i].pde_4mb.rsvd = 0;
            page_directory[i].pde_4mb.frame = (uint32_t)KERNEL_ADD >> 22;     // shifted coz of the structre of where the address is! add starts at bit 22
        } 
        else if(i == 32){ //confirm this indexing for user memory...
            page_directory[i].pde_4mb.present = 1;
            page_directory[i].pde_4mb.rw = 1;
            page_directory[i].pde_4mb.user = 1;
            page_directory[i].pde_4mb.pwt = 0;
            page_directory[i].pde_4mb.pcd = 0;
            page_directory[i].pde_4mb.accessed = 0;
            page_directory[i].pde_4mb.dirty = 0;
            page_directory[i].pde_4mb.ps = 1;
            page_directory[i].pde_4mb.global = 1;
            page_directory[i].pde_4mb.available = 0;
            page_directory[i].pde_4mb.pat = 0;
            page_directory[i].pde_4mb.addr = 0;
            page_directory[i].pde_4mb.rsvd = 0;
            page_directory[i].pde_4mb.frame = (uint32_t)USER_ADD >> 22;  // this is the new user add, shifted by 22 as they are 4mb 
        }
        
        else if (i == 33) {
            page_directory[i].pde_4kb.present = 1;
            page_directory[i].pde_4kb.rw = 1;
            page_directory[i].pde_4kb.user = 1;
            page_directory[i].pde_4kb.pwt = 0;
            page_directory[i].pde_4kb.pcd = 1;
            page_directory[i].pde_4kb.accessed = 0;
            page_directory[i].pde_4kb.avl = 0;
            page_directory[i].pde_4kb.ps = 0; // ps should be 0
            page_directory[i].pde_4kb.available = 0; // Set to the desired value
            page_directory[i].pde_4kb.frame = (uint32_t)page_table_vidmap >> 12;
        }
        
        else{
            page_directory[i].bs = 0;
        }
    }


    for(i = 0; i < MAX_ENTRIES; i++){
        page_table[i].rw = 0;
        page_table[i].user = 0;
        page_table[i].pwt = 0;
        page_table[i].pcd = 0;
        page_table[i].accessed = 0;
        page_table[i].dirty = 0;
        page_table[i].pat = 0;
        page_table[i].global = 0;
        page_table[i].available = 0;
        page_table[i].frame = 0;
        switch(i){
            case VIDEO_MEM_ADDR:
                page_table[i].present = 1;
                page_table[i].rw = 1;
                page_table[i].frame = VIDEO_MEM_ADDR;
                break;
            case TERM1_VRAM:
                page_table[i].present = 1;
                page_table[i].rw = 1;
                page_table[i].frame = TERM1_VRAM;
                break;
            case TERM2_VRAM:
                page_table[i].present = 1;
                page_table[i].rw = 1;
                page_table[i].frame = TERM2_VRAM;
                break;
            case TERM3_VRAM:
                page_table[i].present = 1;
                page_table[i].rw = 1;
                page_table[i].frame = TERM3_VRAM;
                break;
            default:
                page_table[i].present = 0;
        }
    }

// new for VID-MAP
    for (i = 0; i < MAX_ENTRIES; i++) {
            page_table_vidmap[i].present = 0;
            page_table_vidmap[i].rw = 1;
            page_table_vidmap[i].user = 1;
            page_table_vidmap[i].pwt = 0;
            page_table_vidmap[i].pcd = 1; 
            page_table_vidmap[i].accessed = 0;
            page_table_vidmap[i].dirty = 0;
            page_table_vidmap[i].pat = 0;
            page_table_vidmap[i].global = 0; 
            page_table_vidmap[i].available = 0; 
            page_table_vidmap[i].frame = 0;

            switch(i){
                case 0:
                    page_table_vidmap[0].present = 1;
                    page_table_vidmap[0].frame = VIDEO_MEM_ADDR; //---B8000 >> 12
                    break;
                case 1:
                    page_table_vidmap[1].present = 1;
                    page_table_vidmap[1].frame = TERM1_VRAM; //---B8000 >> 12
                    break;
                case 2:
                    page_table_vidmap[2].present = 1;
                    page_table_vidmap[2].frame = TERM2_VRAM; //---B8000 >> 12
                    break;
                case 3:
                    page_table_vidmap[3].present = 1;
                    page_table_vidmap[3].frame = TERM3_VRAM; //---B8000 >> 12
                    break;
                default:
                    break;
            }

    }

    loadPageDirectory((unsigned int*)page_directory);
    otherenable();
    page_table[0xBC].present = 1;
    page_table[0xBC].rw = 1;
    page_table[0xBC].frame = VIDEO_MEM_ADDR;

     asm volatile (
            "movl %%cr3, %%eax ;"
            "movl %%eax, %%cr3 ;"
            :
            :
            :"eax","cc"
        );

}
