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

// Estructura de cada posición en la ruta
typedef struct {
    int numero_posicion;
    char elemento;       // Elementos fijos: 'P', 'V', 'O', 'T', 'S', 'I', '.'
    int tiene_jugador;   // 1 si está el jugador, 0 si no
    int tiene_bandido;   // Contador: 0, 1, o más bandidos en esta casilla
} tPosicion;

int cargarConfiguracion(const char* ruta, tConfig* config);
void generarEscenario(tConfig* config, tListaDoble* ruta_desierto);
void mostrarMenu();
// Estructura de un Movimiento
typedef struct {
    char entidad;    // 'J' para Jugador, 'B' para Bandido
    int pos_origen;  // Casillero desde donde inicia el movimiento
    char direccion;  // 'F' o 'B'
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
void jugarTurno(tConfig *config, tListaDoble *ruta, tCola *colaMovimientos, int vidas, int puntos, int *turnos_perdidos);
int tirarDado();
void enviarJugadorAlInicio(tListaDoble *ruta);
char obtenerDireccionBandido(int posB, int posJ, int totalPos);
void mostrarMapa(tListaDoble *ruta);
int ejecutarMovimientos(tListaDoble *ruta, tCola *colaMovimientos, int *vidas, int *puntos, int *turnos_perdidos, int *protegido);
void iniciarPartida(tConfig *config, tListaDoble *ruta);
#endif // FUNCIONES_H
