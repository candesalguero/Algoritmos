#include "main.h"

int main()
{
    /** Variables de tu juego */
    tConfiguracion config;
    tListaDoble ruta_desierto;
    int opcion, errores, resultado;
    /** --- VARIABLES  --- */
    int puntos_obtenidos;
    tPartida nuevaPartida;
    /** DECLARACION DE VARIABLES DE ESTRUCTURAS*/
    tArbol idxJugador;
    tJugadorIdx nuevoJugador;
    tJugador jugador;

    srand(time(NULL)); /*/srand le pasa la hora actual como punto de arranque para que el resultado del dado (rand) no siga siempre la misma secuencia*/

    /** --- 1. INICIALIZACIÓN DEL EQUIPO (ÁRBOL) --- */
    /*GenerarLotePrueba();*/


    Arbol_Crear(&idxJugador);
    InicializarIndice(&idxJugador, ARCHIVO_JUGADORES, ARCHIVO_INDICE, cmpJugadoresIdx, CopiarIndice);
    /*GenerarIndiceJugadores(&idxJugador, ARCHIVO_JUGADORES, ARCHIVO_INDICE, cmpJugadoresIdx, CopiarIndice);*/
    errores = leerConfig(&config,ARCHIVO_CONFIG);
    if (errores != TODO_OK)
    {
        ManejoErrores(errores,ARCHIVO_CONFIG);
        return errores; /*/ Terminamos si no hay archivo de configuración*/
    }

    do
    {
        mostrarMenu();
        /* Verificamos si scanf logro leer exactamente 1 elemento */
        if (scanf("%d", &opcion) != 1) {
            opcion = -1; /* valor invalido para que caiga en el 'default' */
        }
        limpiarBuffer(); /* limpiamos el Enter sobrante */

        switch (opcion)
        {
            case 1:
                {
                    LimpiarPantalla();
                    puts("Ingrese su nickname para comenzar a jugar!");
                    printf("Tu nombre: ");
                    LeerCadena(nuevoJugador.nickname, 20);
                    resultado = Arbol_BusquedaBinaria(&idxJugador, &nuevoJugador, &nuevoJugador, sizeof(tJugadorIdx),cmpJugadoresIdx);
                    if(resultado == NOT_FOUND_ELEM)
                    {
                        /**Dar de alta al jugador*/
                        puts("Bienvenido Nuevo Jugador!");
                        AltaJugadores(&jugador, &nuevoJugador, ARCHIVO_JUGADORES);
                        Arbol_Insertar(&idxJugador, &nuevoJugador, sizeof(tJugadorIdx), cmpJugadoresIdx);
                    }
                    else /*si el jugador está, me traigo del archivo sus datos */
                    {
                        BuscarDatosJugadores(&jugador, &nuevoJugador, ARCHIVO_JUGADORES);
                    }
                    printf("Nombre Jugador: %s \t Partidas Jugadas: %d \t Fecha de Ingreso: %s\n",
                           jugador.nickname, jugador.partidas_jugadas, jugador.fechaIngreso);
                    Pausar();
                    LimpiarPantalla();

                    /* --- ACÁ ARRANCA EL JUEGO REAL --- */
                    generarEscenario(ARCHIVO_TABLERO, &config, &ruta_desierto);
                    puntos_obtenidos = iniciarPartida(&config, &ruta_desierto, &nuevaPartida);

                    /** --- GUARDADO DE DATOS (Si no abandonó) --- */
                    if(puntos_obtenidos != FIN_PARTIDA)
                    {
                        strcpy(nuevaPartida.nickname, jugador.nickname);
                        ActualizarJugadorGuardado(&jugador, &nuevoJugador, ARCHIVO_JUGADORES);
                        GuardarNuevaPartida(&nuevaPartida, ARCHIVO_PARTIDAS);
                    }
                }break;
            case 2:
                {
                    mostrarRanking();
                    Pausar();
                    LimpiarPantalla();
                }break;
            case 3:
                {
                    Pausar();
                    LimpiarPantalla();
                    printf("\nSaliendo...\n\n");
                }break;
            default:
                {
                    printf("\nOpcion invalida. Intente nuevamente.\n");
                    Pausar();
                    LimpiarPantalla();
                    system("pause");
                    system("cls");
                }break;
        }
    }while(opcion != 3);

    Arbol_GuardarEnArchivo(&idxJugador, ARCHIVO_INDICE, PRE_ORDEN);

    Arbol_Destruir(&idxJugador);

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
/*///////////////////////////////////////////////////////////////////*/
int ActualizarJugadorGuardado(tJugador *jugador, tJugadorIdx *nuevoJugador, const char *archJugadores)
{
    FILE *pf;

    jugador->partidas_jugadas++; /* Le sumamos 1 al historial histórico del jugador */

    pf = fopen(archJugadores, "r+b");
    if(!pf) {
        return ERR_ARCH;
    }

    /* Nos posicionamos en su registro exacto y lo sobreescribimos */
    fseek(pf, nuevoJugador->registro * sizeof(tJugador), SEEK_SET);
    fwrite(jugador, sizeof(tJugador), 1, pf);
    fclose(pf);

    return TODO_OK;
}
int GuardarNuevaPartida(tPartida *nuevaPartida, const char *archPartidas)
{
    FILE *pf;
    tPartida ultimaPartida;

    /* Si el archivo no existe, será la primera partida */
    nuevaPartida->nro_partida = 1;

    pf = fopen(archPartidas, "r+b");

    if (pf != NULL)
    {
        /* Nos posicionamos sobre el último registro */
        fseek(pf, 0L, SEEK_END);
        if( (size_t)ftell(pf)>= sizeof(tPartida) )
        {
            fseek(pf, -(long)sizeof(tPartida), SEEK_END);
            if (fread(&ultimaPartida, sizeof(tPartida), 1, pf) == 1)
                nuevaPartida->nro_partida = ultimaPartida.nro_partida + 1;
        }
        /* Nos posicionamos al final para agregar la nueva partida */
        fseek(pf, 0L, SEEK_END);
    }
    else
    {
        /* El archivo no existe: lo creamos */
        pf = fopen(archPartidas, "wb");

        if (!pf)
            return ERR_ARCH;
    }

    if (fwrite(nuevaPartida, sizeof(tPartida), 1, pf) != 1)
    {
        fclose(pf);
        return ERR_ARCH;
    }

    fclose(pf);

    return TODO_OK;
}
