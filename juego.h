#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include "config.h"

#define POS_OCUPADA 20

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

/*/ Estructura de cada posici�n en la ruta*/
typedef struct {
    int numero_posicion;
    char elemento;       /*/ Elementos fijos: 'P', 'V', 'O', 'T', 'S', 'I', '.'*/
    int tiene_jugador;   /*/ 1 si est� el jugador, 0 si no*/
    int tiene_bandido;   /*/ Contador: 0, 1, o m�s bandidos en esta casilla*/
} tPosicion;

/*/ Estructura de un Movimiento*/
typedef struct {
    char entidad;    /*/ 'J' para Jugador, 'B' para Bandido*/
    int pos_origen;  /*/ Casillero desde donde inicia el movimiento*/
    char direccion;  /*/ 'F' o 'B'*/
    int casillas;    /*/ Cantidad a mover (1 a 6)*/
} tMovimiento;

typedef struct {
    int posicion_actual;
    int vidas;
    int puntos;
    int turnos_perdidos;
    int protegido; // 0 = sin escudo, 1 = activo, 2 = recién adquirido en oasis
} tEstadoJugador;

typedef struct {
    int posJugador;
    int totalPosiciones;
    tCola *colaMovimientos;
    int hay_bandidos;
}tContextoBandidos;

/*/ Funciones del Motor del Juego*/
int cargarConfiguracion(const char* ruta, tConfig* config);
/*void generarEscenario(tConfig* config, tListaDoble* ruta_desierto);*/
int tirarDado();
char obtenerDireccionBandido(int posB, int posJ, int totalPos);
void enviarJugadorAlInicio(tListaDoble *ruta);

int turnoJugador(tListaDoble *ruta, tCola *colaMovimientos, tCola *colaHistorial, tEstadoJugador *jugador);


int obtenerPosJugador(tListaDoble *ruta);
void turnoBandidos(tConfig *config, tListaDoble *ruta, tCola *colaMovimientos, tEstadoJugador *jugador);

int ejecutarMovimientos(tListaDoble *ruta, tCola *colaMovimientos,tEstadoJugador *jugador, tConfig config);

void mostrarMapa(tListaDoble *ruta, tEstadoJugador *jugador);
int mostrarHistorial(tCola *historial);
void Pausar();
void LimpiarPantalla();
int iniciarPartida(tConfig *config, tListaDoble *ruta, tPartida *partidaActual);

void limpiarBuffer();
void mostrarMenu();

int accionMandarInicio(void *dato, void *ctx);
void accionBuscarJugador(void *dato, void *ctx);
int accionImprimirCasillero(void *dato, void *ctx) ;
void InicializarJugador(tEstadoJugador *jugador, tConfig *config);

/*NUEVO*/
void *buscarPtrElementoListaDC(const tListaDoble *lista, const void *aBuscar, tCmp cmp);
int cmpPosicionPorNumero(const void *a, const void *b);
int inicializarRutaDesierto(tListaDoble *ruta_desierto, int cantidad_posiciones);
int colocarElementoEnRuta(tListaDoble *ruta_desierto, int numero_posicion, char simbolo);
int colocarElementosAleatorios(tListaDoble *ruta_desierto, int cantidad_posiciones, int cantidad, char simbolo);
int escribirRutaDesiertoEnArchivo(const char *nombre_archivo, const tListaDoble *ruta_desierto);
FILE * abrirArchivo(const char *nombre, const char *modo);
int generarEscenario(tConfig *config, tListaDoble *ruta_desierto);


#endif
