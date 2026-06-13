#include "main.h"

void mostrarJugador(const void *jugadores);
void mostrarPartida(const void *partida);
void mostrarIdx(const void *jug);
int cmpJugadoresIdx(const void *jugador1, const void *jugador2);
void CopiarIndice(void *dest, const void *orig, const void *reg);

int main()
{
    tJugador jugadores[] =
    {
        {"LunaGamer", "12/05/2026", 3},
        {"MaxPower", "15/05/2026", 2},
        {"NicoPro", "18/05/2026", 1},
        {"LilaPlay", "20/05/2026", 4},
        {"SofiWin", "22/05/2026", 2},
        {"LeoCrack", "25/05/2026", 1},
        {"MatiZ", "28/05/2026", 3},
        {"ValeX", "30/05/2026", 1},
        {"Tomi99", "01/06/2026", 2},
        {"FlorGame", "03/06/2026", 1}
    };

    tPartida partidas[] =
    {
        {1, "LunaGamer", 4, 20},
        {2, "MaxPower", 2, 25},
        {3, "NicoPro", 3, 15},
        {4, "LilaPlay", 5, 10},
        {5, "SofiWin", 1, 13},
        {6, "LeoCrack", 2, 12},
        {7, "MatiZ", 3, 21},
        {8, "ValeX", 5, 22},
        {9, "Tomi99", 3, 23},
        {10, "FlorGame", 1, 25},
        {11, "LunaGamer", 1, 24},
        {12, "LilaPlay", 6, 9},
        {13, "MatiZ", 5, 18},
        {14, "MaxPower", 3, 17},
        {15, "LilaPlay", 2, 20}
    };
       /**Creacion de lote de pruebas*/

    int cantJug = 0, cantPart = 0;
    int opcion, errores;
    /** DECLARACION DE VARIABLES DE ESTRUCTURAS*/
    tConfiguracion config;
    tArbol idxJugador;

    srand(time(NULL)); /*/srand le pasa la hora actual como punto de arranque para que el resultado del dado (rand) no siga siempre la misma secuencia*/
    cantJug = sizeof(jugadores)/sizeof(tJugador);
    cantPart = sizeof(partidas)/sizeof(tPartida);


        GuardarArchivo(ARCHIVO_JUGADORES,jugadores,sizeof(tJugador),cantJug);
        GuardarArchivo(ARCHIVO_PARTIDAS,partidas,sizeof(tPartida),cantPart);

        MostrarArchivo(ARCHIVO_JUGADORES, sizeof(tJugador),mostrarJugador);
        MostrarArchivo(ARCHIVO_PARTIDAS, sizeof(tPartida), mostrarPartida);

    Arbol_Crear(&idxJugador);
    GenerarIndiceJugadores(&idxJugador, ARCHIVO_JUGADORES, ARCHIVO_INDICE, cmpJugadoresIdx, CopiarIndice);

    /*
    Arbol_RecorrerInOrden(&idxJugador, mostrarIdx);
    MostrarArchivo(ARCHIVO_INDICE, sizeof(tJugadorIdx),mostrarIdx);*/

    errores = leerConfig(&config,ARCHIVO_CONFIG);
    if (errores != TODO_OK)
    {
        ManejoErrores(errores,ARCHIVO_CONFIG);
        return errores; /*/ Terminamos si no hay archivo de configuración*/
    }



    do
    {
        Menu();
        printf("Opcion: ");
        scanf("%d", &opcion);
        fflush(stdin);
        switch (opcion)
        {
            case 1:
                {
                    system("cls");
                    jugarPartida();
                }
                break;
            case 2:
                {
                    /*/mostrarRanking();*/
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
                    system("pause");
                    system("cls");
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
