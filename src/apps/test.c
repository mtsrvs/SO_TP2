#include <kernel.h>
extern int suma;

int
test_main(int argc, char *argv[])
{
	printk("Hola mundo\n");
	// printk("valor de suma:%d",suma);
	return 0;
}
