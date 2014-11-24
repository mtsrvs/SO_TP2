#ifndef __CRON_H__
#define __CRON_H__

#include <lib.h>

//typedef int (*TaskFunc_t)(void *arg);

#define BUFSIZE 200
#define NAMESIZE 30
#define NARGS 20
#define NHIST 10

typedef int (*main_func)(int argc, char *argv[]);
typedef int (*TaskFunc_t)(void *arg);

// typedef struct
// {
// 	int minute;
// 	int hour;
// 	int date;
// 	int month;
// }date_t;

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

// SIN USO
// typedef struct 
// {
// 	//TaskFunc_t func;
// 	//char *args[10];
// 	date_t date;
// 	// taskcron_t *head;
// 	// taskcron_t *tail;
// }taskcron_t;

void getCronInfo(void);
#endif
