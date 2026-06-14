#include "main.h"

void mostrarJugador(const void *jugadores);
void mostrarPartida(const void *partida);
void mostrarIdx(const void *jug);
int cmpJugadoresIdx(const void *jugador1, const void *jugador2);
void CopiarIndice(void *dest, const void *orig, const void *reg);

int main()
{
    /** --- VARIABLES  --- */
    int opcion, errores, resultado;
    /** Variables de tu juego */
    tConfig config;
    tListaDoble ruta_desierto;
    /** DECLARACION DE VARIABLES DE ESTRUCTURAS*/
    tConfiguracion config;
    tArbol idxJugador;
    tJugadorIdx nuevoJugador;
    tJugador jugador;

    srand(time(NULL)); /*/srand le pasa la hora actual como punto de arranque para que el resultado del dado (rand) no siga siempre la misma secuencia*/

    /** --- 1. INICIALIZACIÓN DEL EQUIPO (ÁRBOL) --- */
    GenerarLotePrueba(mostrarJugador,mostrarPartida);
    Arbol_Crear(&idxJugador);
    GenerarIndiceJugadores(&idxJugador, ARCHIVO_JUGADORES, ARCHIVO_INDICE, cmpJugadoresIdx, CopiarIndice);

    /*
    Arbol_RecorrerInOrden(&idxJugador, mostrarIdx);
    MostrarArchivo(ARCHIVO_INDICE, sizeof(tJugadorIdx),mostrarIdx);
    */
    /* --- 2. INICIALIZACIÓN DE TU JUEGO --- */
    errores = leerConfig(&config,ARCHIVO_CONFIG);
    if (errores != TODO_OK)
    {
        ManejoErrores(errores,ARCHIVO_CONFIG);
        return errores; /*/ Terminamos si no hay archivo de configuración*/
    }



    do
    {
        mostrarMenu();
        // Verificamos si scanf logró leer exactamente 1 elemento
        if (scanf("%d", &opcion) != 1) {
            opcion = -1; // Le asignamos un valor inválido para que caiga en el 'default'
        }

        limpiarBuffer(); // Limpiamos la basura o el 'Enter' sobrante

        switch (opcion)
        {
            case 1:
                {
                    LimpiarPantalla();
                    puts("Ingrese su nickname para comenzar a jugar!");
                    scanf("%s",nuevoJugador.nickname);
                    limpiarBuffer();
                    resultado = Arbol_BusquedaBinaria(&idxJugador, &nuevoJugador, &nuevoJugador, sizeof(tJugadorIdx),cmpJugadoresIdx);
                    if(resultado == NOT_FOUND_ELEM)
                    {
                        /**Dar de alta al jugador*/
                        AltaJugadores(&jugador, &nuevoJugador, ARCHIVO_JUGADORES);
                        Arbol_Insertar(&idxJugador, &nuevoJugador, sizeof(tJugadorIdx), cmpJugadoresIdx);
                        /*Arbol_RecorrerInOrden(&idxJugador, mostrarIdx);*/
                    }
                    else /*si el jugador está, me traigo del archivo sus datos */
                    {
                        BuscarDatosJugador(&jugador, &nuevoJugador, ARCHIVO_JUGADORES);
                    }


                    Pausar();
                    LimpiarPantalla();

                    /* --- ACÁ ARRANCA TU JUEGO REAL --- */
                    generarEscenario(&config, &ruta_desierto);
                    iniciarPartida(&config, &ruta_desierto);

                    /* TODO PARA EL PASO 2: Necesitamos que iniciarPartida nos devuelva
                       los puntos para guardarlos en el archivo del jugador. */
                }
                break;
            case 2:
                {
                    mostrarRanking();
                    Pausar();
                    LimpiarPantalla();
                }
                break;
            case 3:
                {
                    printf("\nSaliendo...\n\n");
                }
                break;
            default:
                {
                    printf("\nOpcion invalida. Intente nuevamente.\n");
                    Pausar();
                    LimpiarPantalla();
                }
                break;
        }
    }while(opcion != 3);

    return 0;
}

int cmpJugadoresIdx(const void *jugador1, const void *jugador2)
{
    tJugadorIdx *jug1 = (tJugadorIdx*) jugador1;
    tJugadorIdx *jug2 = (tJugadorIdx*) jugador2;

    return strcmpi(jug1->nickname,jug2->nickname);
}

void CopiarIndice(void *dest, const void *orig, const void *reg)
{
    tJugadorIdx *idx = (tJugadorIdx*)dest;
    tJugador *jug = (tJugador*)orig;
    unsigned *registro = (unsigned*)reg;

    strcpy(idx->nickname, jug->nickname);
    idx->registro = *registro;
}

void mostrarJugador(const void *jug)
{
    tJugador * jugador = (tJugador*)jug;
    printf("%-20s %-12s %-10d\n",
           jugador->nickname,
           jugador->fechaIngreso,
           jugador->partidas_jugadas);
}

void mostrarPartida(const void *part)
{
    tPartida *partida = (tPartida*)part;

    printf("%-10d %-20s %-10d %-15d\n",
           partida->nro_partida,
           partida->nickname,
           partida->puntos,
           partida->cant_movimientos);
}

void mostrarIdx(const void *jug)
{
    tJugadorIdx *idx = (tJugadorIdx*)jug;
    printf("[%u]%s\n",idx->registro,idx->nickname);
}
