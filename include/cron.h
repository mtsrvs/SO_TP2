#ifndef __CRON_H__
#define __CRON_H__

//typedef int (*TaskFunc_t)(void *arg);

typedef struct
{
	int minute;
	int hour;
	int date;
	int month;
}date_t;

typedef struct 
{
	//TaskFunc_t func;
	//char *args[10];
	date_t date;
	// taskcron_t *head;
	// taskcron_t *tail;
}taskcron_t;

#endif
