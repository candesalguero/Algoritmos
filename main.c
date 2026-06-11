#include "main.h"

int main()
{
    tConfiguracion config;
    tArbol arbol;
    int opcion, errores;

    CrearArbol(&arbol);
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

