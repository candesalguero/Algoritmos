#include "main.h"

void mostrarJugador(const void *jugadores);

int main()
{
    tConfiguracion config;
    tArbol idxJugador;
    int opcion, errores, cantJug;

    tJugador jugador;
    tJugador vecJugador[]={
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
    cantJug = sizeof(vecJugador)/sizeof(tJugador);



    GuardarArchivo(ARCHIVO_JUGADORES,vecJugador,sizeof(tJugador),cantJug);
    MostrarArchivo(ARCHIVO_JUGADORES, sizeof(tJugador),mostrarJugador);
    Arbol_Crear(&idxJugador);


    srand(time(NULL)); //srand le pasa la hora actual como punto de arranque para que el resultado del dado (rand) no siga siempre la misma secuencia
    errores = leerConfig(&config,ARCHIVO_CONFIG);
    if (errores != TODO_OK)
    {
        ManejoErrores(errores,ARCHIVO_CONFIG);
        return errores; // Terminamos si no hay archivo de configuración
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
                    //mostrarRanking();
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
