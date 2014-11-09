#include <kernel.h> //para utilizar outb e inb
#include <rtc.h>
#include <calendar.h>

int check_time(unsigned char hour, unsigned char minute, unsigned char second){
      if(hour<0 || hour>23 || minute<0 || minute >59 || second<0 || second>59){
            return 1;
      }

      return 0;
}

int check_date(unsigned char day, unsigned char month, int year){
      int daysinmonth = days_in_month(GREGORIAN, year, month);

      if ( day >= 1 && day <= daysinmonth )
            return 0;
      return 1;
}

int
time_main(int argc, char *argv[])
{

      unsigned char second;
      unsigned char minute;
      unsigned char hour;
      unsigned char day;
      unsigned char month;
      unsigned char year;
      unsigned char century;

      if(argc  == 1){
      //TIME 
            read_rtc(&hour, &minute, &second, &day, &month, &year, &century);

            printk("Time: %02d:%02d:%02d  Date:%02d/%02d/%d\n", 
                  hour, minute, second, day, month, full_year(century, year), century);

            return 0;
      }else{
      //TIME SET
            if (strcmp(argv[1], "set") != 0){
                  printk("Comando incorrecto.\n");
                  return 1;
            }

            if(argc != 8){
                  printk("Cantidad de parametros incorrecto.\n");
                  return 1;
            }

            hour = atoi(argv[2]);
            minute = atoi(argv[3]);
            second = atoi(argv[4]);

            day = atoi(argv[5]);
            month = atoi(argv[6]);
            year = atoi(argv[7])%100;
            century = atoi(argv[7])/100;

            if(check_time(hour,minute,second) != 0){
                  printk("horario incorrecto.\n");
                  return 1;
            }


            if ( check_date(day, month, full_year(century, year)) != 0){
                  printk("Fecha incorrecta.\n");
                  return 1;
            }

            write_rtc(hour, minute, second, day, month, year, century);
            return 0;
      }
}