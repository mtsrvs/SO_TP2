// SOURCE
// http://wiki.osdev.org/CMOS#Getting_Current_Date_and_Time_from_RTC
#include <kernel.h> //para utilizar outb e inb

#define CURRENT_YEAR        2014                            // Change this each year!
 
int century_register = 0x00;                                // Set by ACPI table parsing code if possible


unsigned char second;
unsigned char minute;
unsigned char hour;
unsigned char day;
unsigned char month;
unsigned int year;
 
//TLS

// typedef struct
// {
//       unsigned char century;
//       unsigned char last_second;
//       unsigned char last_minute;
//       unsigned char last_hour;
//       unsigned char last_day;
//       unsigned char last_month;
//       unsigned char last_year;
//       unsigned char last_century;
//       unsigned char registerB;
// }
// data; 

// #define century         TLS(data)->century
// #define last_second     TLS(data)->last_second
// #define last_minute     TLS(data)->last_minute
// #define last_hour       TLS(data)->last_hour
// #define last_day        TLS(data)->last_day
// #define last_month      TLS(data)->last_month
// #define last_year       TLS(data)->last_year
// #define last_century    TLS(data)->last_century
// #define registerB       TLS(data)->registerB

// void outb(int port, int value);
// int inb(int port);
 
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
      // if(hour<0 || hour>23 || minute<0 || minute >59 || second<0 || second>59){
      //       printk("datos incorrectos");
      //       return;
      // }

      outb(cmos_address, 0x00);   // select Status Register A, and disable NMI (by setting the 0x80 bit)
      outb(cmos_data, 0x08);
      
      outb(cmos_address, 0x02);   // select Status Register A, and disable NMI (by setting the 0x80 bit)
      outb(cmos_data, 0x08);

      outb(cmos_address, 0x04);   // select Status Register A, and disable NMI (by setting the 0x80 bit)
      outb(cmos_data, 0x08);
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
            read_rtc(); //toma los datos y los guarda en el TLS

            printk("Valores de RTC= hora: %d minutos:%d segundos: %d\n", hour, minute, second);

            return 0;
      }else{
            write_rtc(*argv[0], *argv[1], *argv[2]);
            return 0;
      }

}

// time

// time set h m s