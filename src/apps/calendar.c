//SOURCE: http://www.tondering.dk/main/index.php/calendar-information/1-information/3-calendar-algorithms

/*
 * CALFAQ version 1.1, 4 April 2008
 *
 * COPYRIGHT:
 *   These functions are Copyright (c) 2008 by Claus Tondering
 *   (claus@tondering.dk).
 *  
 * LICENSE:
 *   The code is distributed under the Boost Software License, which
 *   says:
 *  
 *     Boost Software License - Version 1.0 - August 17th, 2003
 *  
 *     Permission is hereby granted, free of charge, to any person or
 *     organization obtaining a copy of the software and accompanying
 *     documentation covered by this license (the "Software") to use,
 *     reproduce, display, distribute, execute, and transmit the
 *     Software, and to prepare derivative works of the Software, and
 *     to permit third-parties to whom the Software is furnished to do
 *     so, all subject to the following:
 *  
 *     The copyright notices in the Software and this entire
 *     statement, including the above license grant, this restriction
 *     and the following disclaimer, must be included in all copies of
 *     the Software, in whole or in part, and all derivative works of
 *     the Software, unless such copies or derivative works are solely
 *     in the form of machine-executable object code generated by a
 *     source language processor.
 *  
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *     EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *     OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND
 *     NON-INFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR
 *     ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR
 *     OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 *     ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 *     USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * DESCRIPTION:
 *   These functions are an implementation in the C language of the
 *   formulas presented in the Calendar FAQ at
 *   http://www.tondering.dk/claus/calendar.html.
 *
 *   The implementation follows the formulas mentioned in version 2.9
 *   of the FAQ quite closely. The focus of the implementation is on
 *   simplicity and clarity. For this reason, no complex data
 *   structures or classes are used, nor has any attempt been made to
 *   optimize the code. Also, no verification of the input parameters
 *   is performed (except in the function simple_gregorian_easter).
 *
 *   All numbers (including Julian Day Numbers which current have
 *   values of almost 2,500,000) are assumed to be representable as
 *   variables of type 'int'.
 */

#include <calendar.h>
#include <kernel.h>
/*
 * is_leap:
 * Determines if a year is a leap year.
 * Input parameters:
 *     Calendar style (JULIAN or GREGORIAN)
 *     Year (must be >0)
 * Returns:
 *     1 if the year is a leap year, 0 otherwise.
 *
 * Note: The algorithm assumes that AD 4 is a leap year. This may be
 * historically inaccurate. See the FAQ.
 *
 * Reference: Sections 2.1.1 and 2.2.1 of version 2.9 of the FAQ.
 */
int is_leap(int style, int year)
{
    return style==JULIAN
             ? year%4==0
             : (year%4==0 && year%100!=0) || year%400==0;
}


/*
 * days_in_month:
 * Calculates the number of days in a month.
 * Input parameters:
 *     Calendar style (JULIAN or GREGORIAN)
 *     Year (must be >0)
 *     Month (1..12)
 * Returns:
 *     The number of days in the month (28..31)
 */
int days_in_month(int style, int year, int month)
{
    static int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    return month==2 && is_leap(style,year)
             ? 29
             : days[month - 1];
}

//SOURCE: http://www.codingunit.com/how-to-make-a-calendar-in-c

char *months[]=
{
    " ",
    "Enero",
    "Febrero",
    "Marzo",
    "Abril",
    "Mayo",
    "Junio",
    "Julio",
    "Agosto",
    "Septiembre",
    "Octubre",
    "Noviembre",
    "Diciembre"
};

int determinedaycode(int year)
{
    int daycode;
    int d1, d2, d3;
    
    d1 = (year - 1.)/ 4.0;
    d2 = (year - 1.)/ 100.;
    d3 = (year - 1.)/ 400.;
    daycode = (year + d1 - d2 + d3) %7;
    return daycode;
}

void print_calendar(int year, int daycode, int current_month)
{
    int month, day, max_day;

    for ( month = 1; month < current_month; month++ )
    {
        //Set position for next month
        max_day = days_in_month(GREGORIAN, year, month);
        daycode = ( daycode + max_day ) % 7;
    }

    max_day = days_in_month(GREGORIAN, year, month);

    printk("%s - %d\n", months[month],year);
    printk("Dom  Lun  Mar  Mie  jue  Vie  Sab\n" );
    
    // Correct the position for the first date
    for ( day = 1; day <= 1 + daycode * 5; day++ )
    {
        printk(" ");
    }
    
    // Print dates for one month
    for ( day = 1; day <= max_day; day++ )
    {
        printk("%2d", day );
        
        // Is day before Sat? Else start next line Sun.
        if ( ( day + daycode ) % 7 > 0 )
            printk("   " );
        else
            printk("\n " );
    }
}

int check_time(int hour, int minute, int second){
      if(hour<0 || hour>23 || minute<0 || minute >59 || second<0 || second>59){
            return 1;
      }

      return 0;
}

int check_date(int day, int month, int year){
      int daysinmonth = days_in_month(GREGORIAN, year, month);

      if ( day >= 1 && day <= daysinmonth )
            return 0;
      return 1;
}
