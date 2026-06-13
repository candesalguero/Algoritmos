#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


/*/ CONSTANTES GENERALES /////////////////////*/
#define MIN(x,y) x > y ? y : x
#define MAX(x,y) x < y ? y : x
#define MAX_NOMBRE 50
#define MAX_MOVIMIENTO 10

/*/ ARCHIVOS ///////////////////////////////*/
#define ARCHIVO_CONFIG "config.txt"
#define ARCHIVO_TABLERO "caravana.txt"
#define ARCHIVO_JUGADORES "jugadores.dat"
#define ARCHIVO_PARTIDAS "partidas.dat"
#define ARCHIVO_INDICE "indiceJugadores.idx"



/*/ ETIQUETAS DE RETURN /////////////////*/
#define TODO_OK 1
#define DISPONIBLE 0
#define ERR_ARCH -1
#define SIN_MEM -2
#define NOT_FOUND_ELEM -3
#define ELEM_REPETIDO -4


/*/ TIPOS DE CELDAS//////////////////////////*/
#define TIPO_VACIO    '.'
#define TIPO_INICIO   'I'
#define TIPO_SALIDA   'S'
#define TIPO_PREMIO   'P'
#define TIPO_VIDA     'V'
#define TIPO_OASIS    'O'
#define TIPO_TORMENTA 'T'
#define TIPO_BANDIDO  'B'


typedef int(*tCmp)(const void*, const void*);
typedef void(*tAccion)(void*, const void*, const void*);
typedef void(*tMostrar)(const void*);
typedef void(*tCopiar)(void*, const void*);
/** se colocan acá para que cuando el compilador entre a los headers, conozca las funciones de tCmp, etc.*/
#include "EstructurasDatos/TDA_Arbol/Arbol_Header.h"
#include "EstructurasDatos/TDA_Cola/cola_dinamica.h"
#include "EstructurasDatos/TDA_Lista/Lista_Header.h"

/*/ ESTRUCTURAS ////////////////////////*/
typedef struct {
    int cantidad_posiciones;
    int vidas_inicio;
    int maximo_bandidos;
    int maximo_premios;
    int maximo_vidas_extra;
    int maximo_oasis;
    int maximo_tormentas;
} tConfiguracion;

typedef struct
{
    char nickname[20];
    unsigned puntos_totales;
    unsigned mov_totales;
}tRankig;

typedef struct
{
    unsigned nro_partida;
    char nickname[20];
    unsigned puntos;
    unsigned cant_movimientos;
}tPartida;

typedef struct
{
    char nickname[20];
    char fechaIngreso[11];
    unsigned partidas_jugadas;
}tJugador;

typedef struct
{
    unsigned registro;
    char nickname[20];
}tJugadorIdx;




#endif /*/ CONFIG_H_INCLUDED*/
