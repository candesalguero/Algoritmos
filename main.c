#include "main.h"

void mostrarJugador(const tJugador jugadores[]);
void mostrarPartida(const tPartida partidas[]);

int main()
{
    int cantJug = 0, cantPart = 0;

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

    cantJug = sizeof(jugadores)/sizeof(tJugador);
    cantPart = sizeof(partidas)/sizeof(tPartida);

    guardarJugadoresArchivo(ARCHIVO_JUGADORES, jugadores, cantJug);
    guardarPartidasArchivo(ARCHIVO_PARTIDAS, partidas, cantPart);

    printf("\n%-20s %-12s %-10s\n", "Nickname", "Fecha", "Partidas");

    if(!recorrerArchivoJugadores(ARCHIVO_JUGADORES, mostrarJugador))
        puts("\nNo se pudo abrir el archivo jugadores.dat\n");

    printf("\n%-10s %-20s %-10s %-15s\n",
           "Nro", "Nickname", "Puntos", "Movimientos");

    if(!recorrerArchivoPartidas(ARCHIVO_PARTIDAS, mostrarPartida))
        puts("\nNo se pudo abrir el archivo partidas.dat\n"); ///Creacion de lote de pruebas

    tConfiguracion config;

    if (!leerConfig(&config))
    {
        return ERR_ARCH; // Terminamos si no hay archivo de configuración
    }

    srand((unsigned int)time(NULL)); //srand le pasa la hora actual como punto de arranque para que el resultado del dado (rand) no siga siempre la misma secuencia
    Menu();

    return 0;
}

void mostrarJugador(const tJugador *jugador)
{
    printf("%-20s %-12s %-10d\n",
           jugador->nickname,
           jugador->fecha,
           jugador->cantPartidasJugadas);
}

void mostrarPartida(const tPartida *partida)
{
    printf("%-10d %-20s %-10d %-15d\n",
           partida->nroPartida,
           partida->nickname,
           partida->puntos,
           partida->cantMovimientos);
}
