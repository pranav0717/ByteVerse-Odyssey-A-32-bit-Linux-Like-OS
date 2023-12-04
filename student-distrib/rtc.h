#ifndef _RTC_H_
#define _RTC_H_

#include "types.h"


#define RTC_PORT        0x70
#define RTC_DATA        0x71
#define RTC_IRQ_LINE    8


#define REG_A           0x8A
#define REG_B           0x8B
#define REG_C           0x8C

#define RTC_MAX         1024
#define RTC_MIN         2

extern void rtc_init(void);
void rtc_handler(void);

int32_t rtc_open (const uint8_t* filename);
int32_t rtc_close (const uint8_t* filename);
int32_t rtc_read (int32_t fd, int32_t* buf, int32_t nbytes);
int32_t rtc_write (int32_t fd, const int32_t* buf, int32_t nbytes);


#endif /* RTC_H */

