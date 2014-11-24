SO_TP2
======

Sistemas Operativos - ITBA - 2014Q2

Los programas agregados a mtask son:
	time
	time set
	calendar
	cron

# Time

    MT1> time

`time` mostrara la fecha y hora actual del sistema.


# Time set

    MT1> time set hh mm ss dd mm yyyy
    
`time set` permite setear la fecha y hora.
Los nuevos valores a ingresar son los siguiente: `hh mm ss dd mm yyyy`.


# Calendar

      MT1> calendar

Muestra el calendario y las tareas encoladas mediante `cron`.
El calendario mostrado esta en formato gregoriano.

# Cron

      MT1> cron

Permite que se agenden tareas para que sean ejecutadas cierto dia y hora varias veces (repeticiones)
con intervalos medidios en milisegundos (ms).Tambien brinda la posibilidad de ejecutar programas en 
el momento con repeticiones.

# Ejemplo de uso agendando programa

    MT1> cron
    Ingresar la funcion y sus parametros:
    time
    Desea agendar esta funcion?
    1 - Si
    2 - No
    1
    Ingrese fecha y hora en la que correra la funcion con formato dd/mm/yyyy hh:mm
    24/11/2014 23:00
    Ingrese cantidad de repeticiones
    10
    Ingrese cada cuantos milisegundos sean los intervalos
    10
    Se encolo la funcion:time para el 24/11 a las 23:00
    
# Ejemplo de uso sin agendar programa
Al final se muestra la fecha y hora actual dado que se ejecuta en el momento.

    MT1> cron
    Ingresar la funcion y sus parametros:
    time
    Desea agendar esta funcion?
    1 - Si
    2 - No
    2
    Ingrese cantidad de repeticiones
    2
    Ingrese cada cuantos milisegundos sean los intervalos
    10
    Se encolo la funcion:time para el 24/11 a las 23:00
    Time: 23:00:00 Date:24/11/2014
    Time: 23:00:00 Date:24/11/2014
    MT1>
