#ifndef __CRON_H__
#define __CRON_H__

#include <lib.h>

#define BUFSIZE 200
#define NAMESIZE 30
#define NARGS 20
#define NHIST 10

typedef int (*main_func)(int argc, char *argv[]);
typedef int (*TaskFunc_t)(void *arg);

struct Date_t{
	int day;
	int month;
	int hr;
	int min;
	int year;
};

struct CronTask_t{
	int					ticks;
	struct Task_t 		*task;
	struct Date_t 		*date;
	bool 				repeat;
	int 	 			repeat_mode;
	char				name[20];
	int 				msecs;
	struct CronTask_t 	*list_prev;
	struct CronTask_t 	*list_next;
};

void getCronInfo(void);
#endif
