SO_TP2
======

Sistemas Operativos - ITBA - 2014Q2

# Programa time

    MT1> time

`time` mostrara la fecha y hora actual del sistema.


# Programa time set

    MT1> time set hh mm ss dd mm yyyy
    
`time set` permite setear la fecha y hora.
Los nuevos valores a ingresar son los siguiente: `hh mm ss dd mm yyyy`.


# Programa calendar

      MT1> calendar

Muestra el calendario y las tareas encoladas mediante `cron`.
El calendario mostrado esta en formato gregoriano.

# Programa cron

      MT1> cron

Permite que se agenden tareas para que sean ejecutadas cierto dia y hora varias veces (repeticiones)
con intervalos medidios en milisegundos (ms).Tambien brinda la posibilidad de ejecutar programas en 
el momento con repeticiones.

ejemplo de uso agendando programa

    MT1> cron
    Ingresar la funcion y sus parametros:
    time
    Ingrese fecha y hora en la que correra la funcion con formato dd/mm/yyyy hh:mm
    24/11/2014 23:00
    Ingrese cantidad de repeticiones
    10
    Ingrese cada cuantos milisegundos sean los intervalos
    10
    Se encolo la funcion:%s para el 24/11 a las 23:00
    
