// SOURCE
// http://wiki.osdev.org/CMOS#Getting_Current_Date_and_Time_from_RTC
#include <kernel.h> //para utilizar outb e inb
#include <lib.h>

#define CURRENT_YEAR        2014                            // Change this each year!
 
int century_register = 0x00;                                // Set by ACPI table parsing code if possible


unsigned char second;
unsigned char minute;
unsigned char hour;
unsigned char day;
unsigned char month;
unsigned int year;
 
enum {
      cmos_address = 0x70,
      cmos_data    = 0x71
};

 
int get_update_in_progress_flag() {
      outb(cmos_address, 0x0A);
      return (inb(cmos_data) & 0x80);
}
  
unsigned char get_RTC_register(int reg) {
      outb(cmos_address, reg);
      return inb(cmos_data);
}
 

void write_rtc(unsigned char hour, unsigned char minute, unsigned char second) {
      char hex2[6] = {0x00, 0x10, 0x20, 0x30, 0x40, 0x50};
      char hex1[10] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
      int uni, deci;

      if(hour<0 || hour>23 || minute<0 || minute >59 || second<0 || second>59){
            printk("datos incorrectos");
            return;
      }

      uni = second%10;
      deci = second/10;

      outb(cmos_address, 0x00);   // second
      outb(cmos_data, hex2[deci] | hex1[uni]);

      uni = minute%10;
      deci = minute/10;
      
      outb(cmos_address, 0x02);   // minute
      outb(cmos_data, hex2[deci] | hex1[uni]);

      uni = hour%10;
      deci = hour/10;

      outb(cmos_address, 0x04);   // hour
      outb(cmos_data, hex2[deci] | hex1[uni]);
}
 
void read_rtc() {
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
            }
      }
 
      // Convert 12 hour clock to 24 hour clock if necessary
 
      if (!(registerB & 0x02) && (hour & 0x80)) {
            hour = ((hour & 0x7F) + 12) % 24;
      }
 
      // Calculate the full (4-digit) year
 
      if(century_register != 0) {
            year += century * 100;
      } else {
            year += (CURRENT_YEAR / 100) * 100;
            if(year < CURRENT_YEAR) year += 100;
      }
}



int
rtc_main(int argc, char *argv[])
{

      if(argc  == 1){
            read_rtc();

            printk("Time: %d:%d:%d  Date:%d/%d/%d \n", hour, minute, second, day, month, year);

            return 0;
      }else{
            if(strcmp(argv[1], "set") == 0)
                  printk("es set %s\n", argv[1]);

            write_rtc(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
            return 0;
      }

}