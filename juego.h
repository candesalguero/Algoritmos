#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include "config.h"
#include "funciones.h"

#define POS_OCUPADA 20

/* Estructura de cada posicion en la ruta */
typedef struct {
    int numero_posicion;
    char elemento;       /* 'P', 'V', 'O', 'T', 'S', 'I', '.' */
    int tiene_jugador;   /* 1 si esta el jugador, 0 si no */
    int tiene_bandido;   /* contador: 0, 1, o mas bandidos en la casilla */
} tPosicion;

/* Estructura de un Movimiento */
typedef struct {
    char entidad;    /* 'J' para Jugador, 'B' para Bandido */
    int pos_origen;  /* casillero desde donde inicia el movimiento */
    char direccion;  /* 'F' o 'B' */
    int casillas;    /* cantidad a mover (1 a 6) */
} tMovimiento;

/* Estado del jugador durante la partida */
typedef struct {
    int posicion_actual;
    int vidas;
    int puntos;
    int turnos_perdidos;
    int protegido; /* 0 = sin escudo, 1 = activo, 2 = recien adquirido en oasis */
} tEstadoJugador;

/* Contexto para el recorrido de bandidos */
typedef struct {
    int posJugador;
    int totalPosiciones;
    tCola *colaMovimientos;
    int hay_bandidos;
} tContextoBandidos;

/* --- Generacion del escenario --- */
int cmpPosicionPorNumero(const void *a, const void *b);
int inicializarRutaDesierto(tListaDoble *ruta_desierto, int cantidad_posiciones);
int colocarElementoEnRuta(tListaDoble *ruta_desierto, int numero_posicion, char simbolo);
int colocarElementosAleatorios(tListaDoble *ruta_desierto, int cantidad_posiciones, int cantidad, char simbolo);
FILE *abrirArchivo(const char *nombre, const char *modo);
int escribirRutaDesiertoEnArchivo(const char *nombre_archivo, const tListaDoble *ruta_desierto);
int generarEscenario(tConfiguracion *config, tListaDoble *ruta_desierto);
void CargarCantidades(int *vec, const tConfiguracion *config);

/* --- Motor del juego --- */
int tirarDado();
char obtenerDireccionBandido(int posB, int posJ, int totalPos);
void enviarJugadorAlInicio(tListaDoble *ruta);
int accionMandarInicio(void *dato, void *ctx);
int turnoJugador(tListaDoble *ruta, tCola *colaMovimientos, tCola *colaHistorial, tEstadoJugador *jugador);
int obtenerPosJugador(tListaDoble *ruta);
int accionBuscarJugador(void *dato, void *ctx);
void turnoBandidos(tConfiguracion *config, tListaDoble *ruta, tCola *colaMovimientos, tEstadoJugador *jugador);
int accionEncolarBandidos(void *ruta, void *ctx);
void accionSacarEntidad(void *dato, void *param);
void accionPonerEntidad(void *dato, void *param);
int ejecutarMovimientos(tListaDoble *ruta, tCola *colaMovimientos, tEstadoJugador *jugador, int totalPosiciones);
void mostrarMapa(tListaDoble *ruta, tEstadoJugador *jugador);
int accionImprimirCasillero(void *dato, void *ctx);
int accionEscribirCasillero(void *dato, void *ctx);
int mostrarHistorial(tCola *historial);
int iniciarPartida(tConfiguracion *config, tListaDoble *ruta, tPartida *partidaActual);
void InicializarJugador(tEstadoJugador *jugador, tConfiguracion *config);
void mostrarMenu();

#endif /* JUEGO_H_INCLUDED */
