#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "types.h"

#define KEYBOARD_DATA 0x60 // data port
#define CHAR_LIMIT 0x59 //limit of my character table

#define TAB_SIZE 4

#define BUFFER_LIMIT 128

    // scan codes //
#define CAPS_PRESSED 0X3A
#define CTRL_PRESSED 0x1D
#define CTRL_RELEASED 0x9D
#define R_SHIFT_PRESSED 0x2A
#define L_SHIFT_PRESSED 0x36
#define R_SHIFT_RELEASED 0xAA
#define L_SHIFT_RELEASED 0xB6
#define TAB_PRESSED 0xF
#define TAB_RELEASED 0x8F
#define ENTER_PRESSED 0x1C
#define ENTER_RELEASED 0x9C
#define L_KEY_PRESS 0x26
#define C_KEY_PRESS 0x2E
#define BACKSPACE_PRESS 0x0E
#define F_1_P 0x3B
#define F_2_P 0x3C                                                // F1 Key Pressed
#define F_3_P 0x3D
#define F_12_P 0x58 // F12press
#define F_1_R 0xBB //F1 release TO (used as ranges to check)
#define F_12_R 0xD8 //F12 release
#define ESCAPE_PRESSED 0x01
#define ESCAPE_RELEASED 0x81
#define LEFT_ALT_PRESSED 0x38
#define LEFT_ALT_RELEASED 0xB8
#define Q_Pressed 0x10
#define P_Pressed 0x19
#define A_Pressed 0x1E
#define L_Pressed 0x26
#define Z_Pressed 0x2C
#define M_Pressed 0x32


    // scan codes //

extern char term_buffer[BUFFER_LIMIT];
extern int buffer_index;

extern void keyboard_init(void);

extern void keyboard_handler(void);

extern int32_t terminal_read (int32_t fd, void* buf, int32_t nbytes);
extern int32_t terminal_write (int32_t fd, const void* buf, int32_t nbytes);
extern int32_t terminal_open (const uint8_t* filename);
extern int32_t terminal_close (int32_t fd);

#endif /* KEYBOARD_H */

