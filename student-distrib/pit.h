#ifndef _PIT_H
#define _PIT_H

#include "i8259.h"
#include "lib.h"
#include "terminal.h"

void pit_init(void);
void pit_handler(void);


#endif
