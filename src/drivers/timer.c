#include <kernel.h>
extern int suma;

void
mt_setup_timer(unsigned msecs)
{
    unsigned count = 1193182 * msecs;
    count /= 1000;

    outb(0x43, 0x36);
    outb(0x40, count);
    outb(0x40, count >> 8);
}

static void write_cmos(unsigned char reg, unsigned data)
{
    //DisableInts();
    outb(CMOS_IN_PORT, reg);
    outb(CMOS_OUT_PORT, data);
    //RestoreInts();
}

static unsigned read_cmos(unsigned char reg)
{
    unsigned d = 0;
    
    //DisableInts();
    outb(CMOS_IN_PORT, reg);
    d = inb(CMOS_OUT_PORT);
    //RestoreInts();
    
    return d;
}

void alarm_handler(unsigned irq_number) {

    int register_c;         
    // Leo el registro C para aceptar futuras interrupciones
    outb(0x70, 0x0C);
    register_c = inb(0x71);
    register_c += register_c; // Linea de codigo inutil. Evita mostrar el warning.
    suma += kernel_rtc_int_handler();
}

void rtc_setup() {
    unsigned reg_a = 0, reg_b = 0;
 
    mt_set_int_handler(8, alarm_handler);
    mt_enable_irq(8);

    /* Configurar registro A para recibir 1024 interrupciones por segundo. */
    reg_a = read_cmos(CMOS_REG_A);
    reg_a &= 0xF0;
    reg_a |= 0x06;
    write_cmos(CMOS_REG_A, reg_a);

    /* Habilitar Periodic interrupt, para lograr esto se habilita el bit 6 del registro B del CMOS. */
    reg_b = read_cmos(CMOS_REG_B);
    reg_b |= (0x01 << 6);
    write_cmos(CMOS_REG_B, reg_b);
}
