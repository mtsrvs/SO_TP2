#ifndef __RTC_H__
#define __RTC_H__

#define CURRENT_YEAR        2014                            // Change this each year!
 

enum {
      cmos_address = 0x70,
      cmos_data    = 0x71
};

int get_update_in_progress_flag();

unsigned char get_RTC_register(int reg);

void write_rtc(unsigned char hour, unsigned char minute, unsigned char second, unsigned char day,
                  unsigned char month, unsigned char year, unsigned char century);

void read_rtc(unsigned char *hour, unsigned char *minute, unsigned char *second, unsigned char *day,
                  unsigned char *month, unsigned char *year, unsigned char *century_r);

int full_year(unsigned char century, unsigned char year);
#endif