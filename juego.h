#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include "config.h"

/*/ Estructura para almacenar la configuracion del juego*/
typedef struct {
    int cantidad_posiciones;
    int vidas_inicio;
    int maximo_bandidos;
    int maximo_premios;
    int maximo_vidas_extra;
    int maximo_oasis;
    int maximo_tormentas;
} tConfig;

/*/ Estructura de cada posición en la ruta*/
typedef struct {
    int numero_posicion;
    char elemento;       /*/ Elementos fijos: 'P', 'V', 'O', 'T', 'S', 'I', '.'*/
    int tiene_jugador;   /*/ 1 si está el jugador, 0 si no*/
    int tiene_bandido;   /*/ Contador: 0, 1, o más bandidos en esta casilla*/
} tPosicion;

/*/ Estructura de un Movimiento*/
typedef struct {
    char entidad;    /*/ 'J' para Jugador, 'B' para Bandido*/
    int pos_origen;  /*/ Casillero desde donde inicia el movimiento*/
    char direccion;  /*/ 'F' o 'B'*/
    int casillas;    /*/ Cantidad a mover (1 a 6)*/
} tMovimiento;


/*/ Funciones del Motor del Juego*/
int cargarConfiguracion(const char* ruta, tConfig* config);
void generarEscenario(tConfig* config, tListaDoble* ruta_desierto);
int tirarDado();
char obtenerDireccionBandido(int posB, int posJ, int totalPos);
void enviarJugadorAlInicio(tListaDoble *ruta);
int turnoJugador(tListaDoble *ruta, tCola *colaMovimientos, tCola *colaHistorial, int vidas, int puntos, int *turnos_perdidos, int protegido);
int obtenerPosJugador(tListaDoble *ruta);
void turnoBandidos(tConfig *config, tListaDoble *ruta, tCola *colaMovimientos);
int ejecutarMovimientos(tListaDoble *ruta, tCola *colaMovimientos, int *vidas, int *puntos, int *turnos_perdidos, int *protegido);
void mostrarMapa(tListaDoble *ruta, int protegido);
int mostrarHistorial(tCola *historial);
void Pausar();
void LimpiarPantalla();
int iniciarPartida(tConfig *config, tListaDoble *ruta, tPartida *partidaActual);

void limpiarBuffer();
void mostrarMenu();


#endif
