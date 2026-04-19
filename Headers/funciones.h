#ifndef FUNCIONES_H
#define FUNCIONES_H

#include "ListaDoble.h"

// Estructura para almacenar la configuracion del juego
typedef struct {
    int cantidad_posiciones;
    int vidas_inicio;
    int maximo_bandidos;
    int maximo_premios;
    int maximo_vidas_extra;
    int maximo_oasis;
    int maximo_tormentas;
} tConfig;

// Estructura de cada posicion en la ruta
typedef struct {
    int numero_posicion;
    char elemento; // 'P', 'V', 'O', 'T', 'B', '.'
    int tiene_jugador;
} tPosicion;

int cargarConfiguracion(const char* ruta, tConfig* config);
void generarEscenario(tConfig* config, tListaDoble* ruta_desierto);
void mostrarMenu();
// Estructura de un Movimiento
typedef struct {
    char entidad;    // 'J' para Jugador, 'B' para Bandido
    int id_entidad;  // 0 para Jugador, 1, 2... para Bandidos
    char direccion;  // 'F' (Forward - Adelante) o 'B' (Backward - Atrás)
    int casillas;    // Cantidad a mover (1 a 6)
} tMovimiento;

// Nodos para la Cola de Movimientos
typedef struct sNodoCola {
    tMovimiento info;
    struct sNodoCola* sig;
} tNodoCola;

// Estructura de la Cola
typedef struct {
    tNodoCola *frente;
    tNodoCola *fondo;
} tCola;

// Primitivas de la Cola
void crearCola(tCola *pc);
int acolar(tCola *pc, const tMovimiento *d);
int desencolar(tCola *pc, tMovimiento *d);
int colaVacia(const tCola *pc);

// Funciones del Motor del Juego
void jugarTurno(tConfig *config, tListaDoble *ruta, tCola *colaMovimientos);
int tirarDado();
void mostrarMapa(tListaDoble *ruta);
int ejecutarMovimientos(tListaDoble *ruta, tCola *colaMovimientos, int *vidas, int *puntos);
void iniciarPartida(tConfig *config, tListaDoble *ruta);
#endif // FUNCIONES_H
