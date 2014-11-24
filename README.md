SO_TP2
======

Sistemas Operativos - ITBA - 2014Q2

# Programa time (ejecucion)

    MT1> time

`time` mostrara la fecha y hora actual del sistema.


# Programa time set (ejecucion)

   MT1> time set hh mm ss dd mm yyyy

`time set` permite setear la fecha y hora.
Los nuevos valores a ingresar son los siguiente: `hh mm ss dd mm yyyy`.


# Programa calendar (

  MT1> calendar

`<dirección ipc>` es:

- `FILESIG`, `FIFOS`: path de un directorio para archivos temporales (ej `tmp`)
- `SOCKETS`: número de puerto
- `SHMEM` y `MQUEUES`: archivo cualquiera, legible


# Cliente

    bin/client <dirección ipc> <comando> [argumentos...]

`<dirección ipc>` es:

- `FILESIG`, `FIFOS`: `<tmp>/<pid server>`, siendo `<tmp>` el mismo del server
- `SOCKETS`: una dirección en formato `<ip>:<puerto>` (ej 127.0.0.1:5000)
- `SHMEM` y `MQUEUES`: un archivo cualquiera, legible (el mismo)


`<comando>` es uno de:

1. `list`: muestra las películas
2. `buy <número de película>`: compra una entrada
3. `get <código de entrada>`: consulta una entrada
