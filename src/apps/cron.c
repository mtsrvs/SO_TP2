#include <cron.h>
#include <kernel.h>
#include <apps.h>


typedef int (*main_func)(int argc, char *argv[]);

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
	{	"time",			rtc_main,			""					}, //RTC
	{															}
};

taskcron_t*
create_a_taskcron(main_func func, int date, int month, int hour, int min){
	taskcron_t *task = malloc(sizeof(taskcron_t));
	task->func = func;
	// task->head = NULL;
	// task->tail = NULL;
	task->date = (date_t){min, hour, date, month};
	return task;
}

int
cron_main(){

	printk("Ingresar funcion, hora, minutos, dia y mes");
	
	struct cmdentry *cp;
	for ( cp = cmdtab ; cp->name ; cp++ ){
			if ( strcmp(/*ex.args[0]*/"time", cp->name) == 0 )
			{
				// found = true;
				//ex.func = cp->func;
				
				int status;

				taskcron_t *t = create_a_taskcron(cp->func, 10, 10, 10, 10);
				break;
			}
		}

//		if ( !found )
//cprintk(LIGHTRED, BLACK, "Comando %s desconocido\n", ex.args[0]);
		return 0;
}