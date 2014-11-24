// SOURCE
// http://wiki.osdev.org/CMOS#Getting_Current_Date_and_Time_from_RTC

#include <kernel.h> //para utilizar outb e inb
#include <lib.h>
#include <rtc.h>

unsigned char second;
unsigned char minute;
unsigned char hour;
unsigned char day;
unsigned char month;
unsigned int year;
unsigned char century_r = 0;

int century_register = 0x32;                                // Set by ACPI table parsing code if possible

 
int get_update_in_progress_flag() {
      outb(cmos_address, 0x0A);
      return (inb(cmos_data) & 0x80);
}
  
unsigned char get_RTC_register(int reg) {
      outb(cmos_address, reg);
      return inb(cmos_data);
}

unsigned toBCD(unsigned char value){
      char hex2[10] = {0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90};
      char hex1[10] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
      int uni, deci;

      uni = value%10;
      deci = value/10;

      return hex2[deci] | hex1[uni];
}

void write_rtc(unsigned char hour, unsigned char minute, unsigned char second, unsigned char day,
                  unsigned char month, unsigned char year, unsigned char century) {

      outb(cmos_address, 0x00);   // second
      outb(cmos_data, toBCD(second));
      
      outb(cmos_address, 0x02);   // minute
      outb(cmos_data, toBCD(minute));

      outb(cmos_address, 0x04);   // hour
      outb(cmos_data, toBCD(hour));

      outb(cmos_address, 0x07);   // day
      outb(cmos_data, toBCD(day));

      outb(cmos_address, 0x08);   // month
      outb(cmos_data, toBCD(month));

      outb(cmos_address, 0x09);   // year
      outb(cmos_data, toBCD(year));

      outb(cmos_address, century_register); // century
      outb(cmos_data, toBCD(century));


}
 
void read_rtc(unsigned char *h, unsigned char *m, unsigned char *s, unsigned char *dy,
                  unsigned char *mo, unsigned char *yr, unsigned char *cr) {
      unsigned char century;
      unsigned char last_second;
      unsigned char last_minute;
      unsigned char last_hour;
      unsigned char last_day;
      unsigned char last_month;
      unsigned char last_year;
      unsigned char last_century;
      unsigned char registerB;
 
      // Note: This uses the "read registers until you get the same values twice in a row" technique
      //       to avoid getting dodgy/inconsistent values due to RTC updates
 
      while (get_update_in_progress_flag());                // Make sure an update isn't in progress
      second = get_RTC_register(0x00);
      minute = get_RTC_register(0x02);
      hour = get_RTC_register(0x04);
      day = get_RTC_register(0x07);
      month = get_RTC_register(0x08);
      year = get_RTC_register(0x09);
      if(century_register != 0) {
            century = get_RTC_register(century_register);
      }
 
      do {
            last_second = second;
            last_minute = minute;
            last_hour = hour;
            last_day = day;
            last_month = month;
            last_year = year;
            last_century = century;
 
            while (get_update_in_progress_flag());           // Make sure an update isn't in progress
            second = get_RTC_register(0x00);
            minute = get_RTC_register(0x02);
            hour = get_RTC_register(0x04);
            day = get_RTC_register(0x07);
            month = get_RTC_register(0x08);
            year = get_RTC_register(0x09);
            if(century_register != 0) {
                  century = get_RTC_register(century_register);
            }
      } while( (last_second != second) || (last_minute != minute) || (last_hour != hour) ||
               (last_day != day) || (last_month != month) || (last_year != year) ||
               (last_century != century) );
 
      registerB = get_RTC_register(0x0B);
 
      // Convert BCD to binary values if necessary
 
      if (!(registerB & 0x04)) {
            second = (second & 0x0F) + ((second / 16) * 10);
            minute = (minute & 0x0F) + ((minute / 16) * 10);
            hour = ( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80);
            day = (day & 0x0F) + ((day / 16) * 10);
            month = (month & 0x0F) + ((month / 16) * 10);
            year = (year & 0x0F) + ((year / 16) * 10);
            if(century_register != 0) {
                  century = (century & 0x0F) + ((century / 16) * 10);
                  century_r = century; // guardo el valor del century
            }
      }
 
      // Convert 12 hour clock to 24 hour clock if necessary
 
      if (!(registerB & 0x02) && (hour & 0x80)) {
            hour = ((hour & 0x7F) + 12) % 24;
      }
 
      // Calculate the full (4-digit) year
 
      // if(century_register != 0) {
      //       year += century * 100;
      // } else {
      //       year += (CURRENT_YEAR / 100) * 100;
      //       if(year < CURRENT_YEAR) year += 100;
      // }

      *h = hour;
      *m = minute;
      *s = second;
      *mo = month;
      *dy = day;
      *yr = year;
      *cr = century_r;
}

int full_year(unsigned char century, unsigned char year){
      // Calculate the full (4-digit) year
      int y = year;
      if(century != 0) {
            y += century * 100;
      } else {
            y += (CURRENT_YEAR / 100) * 100;
            if(y < CURRENT_YEAR) y += 100;
      }

      return y;
}