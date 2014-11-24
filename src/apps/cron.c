#include <cron.h>
#include <calendar.h>
#include <rtc.h>
#include <kernel.h>
#include <apps.h>
#include <lib.h>

#define MSPERTICK 		10						/* 100 Hz */

typedef int (*main_func)(int argc, char *argv[]);

typedef struct
{
	main_func func;
	int nargs;
	char *args[NARGS+1];
	char buf[BUFSIZE];
}
execpars;


static struct cmdentry
{
	char *name;
	main_func func;
	char *params;
}
cmdtab[] =
{
	{	"setkb",		setkb_main, 		"[distrib]"			},
	{	"shell",		shell_main,			""					},
	{	"sfilo",		simple_phil_main,	""					},
	{	"filo",			phil_main,			""					},
	{	"xfilo",		extra_phil_main,	""					},
	{	"afilo",		atomic_phil_main,	""					},
	{	"camino",		camino_main,		""					},
	{	"camino_ns",	camino_ns_main,		"[cantidad]"		},
	{	"prodcons",		prodcons_main,		""					},
	{	"divz",			divz_main,			"dividendo divisor"	},
	{	"pelu",			peluqueria_main,	""					},
	{	"events",		events_main,		""					},
	{	"disk",			disk_main,			""					},
	{	"ts", 			ts_main,			"[consola...]"		},
	{	"kill",			kill_main,			"tarea [status]"	},
	{	"test",			test_main,			""					},
	{	"time",			time_main,			""					}, //RTC
	{	"time set",		time_main,			"[hh mm ss dd mm yyyy]"}, //RTC	
	{	"calendar",		cal_and_ts_main,	""					},
	{   "cron", 		cron_main, 			""					}, // TAREA CRON
	{															}
};


static int
attached_app(void *arg)
{
	execpars *ep = arg;

	return ep->func(ep->nargs, ep->args);
}

static unsigned 
msecs_to_ticks(unsigned msecs)
{
	return (msecs + MSPERTICK - 1) / MSPERTICK;
}

char line[BUFSIZE];
  
bool
correAhora()
{
	printk("Desea agendar esta funcion?\n");
	printk("1 - Si\n");
	printk("2 - No\n");

	line[0]=0;
	int pos = getline(line, sizeof line);
	while ( --pos >= 0 )
	{
		char c = line[pos];
		switch ( c )
		{
			case ' ':
			case '\t':
			case '\r':
			case '\n':
				line[pos] = 0;
				continue;
		}
		break;
	}

	if(line[0] == '1')
	{
		return false;
	}
	return true;
}

bool
fechaHrOk(int day, int month, int year, int hr, int min)
{
	if(day<1 || day>31)
		return false;
	if(month<1 || month>12)
		return false;
	if(hr<0 || hr>23)
		return false;
	if(min<0 || min>59)
		return false;
	if(day>30 && (month == 2 || month == 4 || month == 6 || month == 9 || month == 11))
		return false;

	return true;
}

int
cron_main(int argc, char *argv[]){ 

	printk("Ingresar la funcion y sus parametros:\n");

	execpars ex;
	char line[BUFSIZE];
	struct cmdentry *cp;
	int pos;
	bool found;
	int repeat_mode = 0;
	int msecs = 1;

	char year[5];
	char month[3];
	char day[3];
	char hr[3];
	char min[3];
	
	year[4] = month[2] = day[2] = hr[2] = min[2] = '\0';
	
	// Sacar espacios al final y detectar comando en background
	line[0]=0;
	pos = getline(line, sizeof line);
	while ( --pos >= 0 )
	{
		char c = line[pos];
		switch ( c )
		{
			case ' ':
			case '\t':
			case '\r':
			case '\n':
				line[pos] = 0;
				continue;
		}
		break;
	}

	// Separar en argumentos
	strcpy(ex.buf, line);

	ex.nargs = separate(ex.buf, ex.args, NARGS);
	if ( ex.nargs )
	{
		ex.args[ex.nargs] = NULL;
		
		/* Aplicaciones */
		found = false;
		int rpt = 0;
		for ( cp = cmdtab ; cp->name ; cp++ )
		{	
			if ( strcmp(ex.args[0], cp->name) == 0 )
			{
				found = true;
				ex.func = cp->func;

				struct Date_t date;

				if(correAhora())
				{
					unsigned char hr, min, day, month, year, no_use_I, no_use_II;
					read_rtc(&hr, &min, &no_use_I, &day, &month, &year, &no_use_II);

					date.year = year;
					date.month = month;
					date.day = day;
					date.hr = hr;
					date.min = min;

				}else{
					printk("Ingrese fecha y hora en la que correra la funcion con formato dd/mm/yyyy hh:mm\n");
					line[0]=0;
					pos = getline(line, sizeof line);
					while ( --pos >= 0 )
					{
						char c = line[pos];
						switch ( c )
						{
							case ' ':
							case '\t':
							case '\r':
							case '\n':
								line[pos] = 0;
								continue;
						}
						break;
					}

					day[0] = line[0];
					day[1] = line[1];
					month[0] = line[3];
					month[1] = line[4];
					year[0] = line[6];
					year[1] = line[7];
					year[2] = line[8];
					year[3] = line[9];
					hr[0] = line[11];
					hr[1] = line[12];
					min[0] = line[14];
					min[1] = line[15];	
				
					if(check_time(atoi(hr), atoi(min), 0) && 
						check_date(atoi(day), atoi(month),atoi(year))){
						return 1;
					}

					date.day = atoi(day);
					date.month =  atoi(month);
					date.year = atoi(year);
					date.hr = atoi(hr);
					date.min = atoi(min);	
				}
                		
				printk("Ingrese cantidad de repeticiones\n");
				line[0]=0;
				pos = getline(line, sizeof line );
				while ( --pos >= 0 )
				{
					char c = line[pos];
					switch ( c )
					{
						case ' ':
						case '\t':
						case '\r':
						case '\n':
							line[pos] = 0;
							continue;
					}
					break;
				}

				repeat_mode = atoi(line);
				if(repeat_mode > 0)
				{
					printk("Ingrese cada cuantos milisegundos sean los intervalos\n");
					line[0]=0;
					pos = getline(line, sizeof line);
					while ( --pos >= 0 )
					{
						char c = line[pos];
						switch ( c )
						{
							case ' ':
							case '\t':
							case '\r':
							case '\n':
								line[pos] = 0;
								continue;
						}
						break;
					}

					msecs = atoi(line);
				}

				for(rpt = 0; rpt < repeat_mode ; rpt++)
				{
					struct Task_t *t = CreateTask(attached_app, MAIN_STKSIZE, &ex, ex.args[0], DEFAULT_PRIO);			
					struct CronTask_t *ct = Malloc(sizeof(CronTask_t));
					ct->task = t;
					strcpy(ct->name, cp->name);
					ct->date = Malloc(sizeof(Date_t));
					memcpy(ct->date, &date, sizeof(Date_t));
					ct->msecs = msecs;
					ct->ticks = msecs_to_ticks((msecs)*(rpt+1));
					cronTask_list_add(ct);
				}
				printk("Se encolo la funcion:%s para el %02d/%d a las %02d:%02d\n", 
					cp->name, date.day, date.month, date.hr, date.min);
				break;
			}
		}

		if ( !found ){
			cprintk(LIGHTRED, BLACK, "Comando %s desconocido\n", ex.args[0]);
		}
		return 0;
	}
	printk("Error de parametros\n");
	return 0;
}
