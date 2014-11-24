#include <kernel.h>
#include <calendar.h>
#include <cron.h>
#include <rtc.h>

static const char *title  = 
    "                       Calendario y Tareas Programadas                          ";

static const char *tasks  = 
    "   Tarea        Fecha        Hora         Repeticiones         Intervalo [ms]   ";


static const char *foot  = 
    "       ENTER, ESPACIO, BS: scroll      ESC: reset scroll      S: salir          ";

static bool
getuser(unsigned *skip)
{
    int c;

    switch ( (c  = getch_timed(200)) )
    {
        case 'S':
        case 's':
            return false;
        case ' ':
        case '\r':
            ++*skip;
            return true;
        case 0x08:      // BS
            if ( *skip )
                --*skip;
            return true;
        case 0x1B:      // ESC
            *skip = 0;
            return true;
    }
    return true;
}

int
cal_and_ts_main(int argc, char *argv[])
{

    unsigned char second;
    unsigned char minute;
    unsigned char hour;
    unsigned char day;
    unsigned char month;
    unsigned char year;
    unsigned char century;
    int fullyear = 0;
    
    unsigned n, skip;
    bool cursor = mt_cons_cursor(false);

    mt_cons_clear();

    mt_cons_gotoxy(0, 0); // me ubica en un lugar en pantalla (X e Y) para deespues escribir.
    cprintk(WHITE, BLUE, "%s", title); // permite escribir con letra y fondo particular
    mt_cons_gotoxy(0, 24);
    cprintk(WHITE, BLUE, "%s", foot);

    skip = 0;

    read_rtc(&hour, &minute, &second, &day, &month, &year, &century);

    fullyear = full_year(century, year);

    mt_cons_gotoxy(0, 1);
    print_calendar(fullyear, determinedaycode(fullyear), month);
    
    mt_cons_gotoxy(0, 9); // me ubica en un lugar en pantalla (X e Y) para deespues escribir.
    cprintk(WHITE, BLUE, "%s", tasks);

    int i, ntasks;
    CronTask_t *c_task, *next;

    do
    {
        ntasks = getCountCronList();
        next = c_task = get_cron_tasks();

        for ( n = 0, i = 10; i < 24 && ntasks-- && next; )
        {
            if ( n++ < skip ){
                next = next->list_next;    
                continue;
            }
            mt_cons_gotoxy(0, i++);
            printk("   %.16s       %02d/%02d/%04d    %02d:%02d    %12d   %17d\n", next->name, 
                (next->date)->day, (next->date)->month, (next->date)->year, 
                (next->date)->hr, (next->date)->min, next->ticks, next->msecs);
            next = next->list_next; 
        }
        while ( i < 24 )
        {
            mt_cons_gotoxy(0, i++);
            mt_cons_clreol();
        }
    }
    while ( getuser(&skip) );
    mt_cons_clear();
    mt_cons_cursor(cursor); 

    return 0;
}