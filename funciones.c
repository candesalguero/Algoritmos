#include "funciones.h"

int leerConfig(tConfiguracion* config)
{
    FILE *pf=fopen(ARCHIVO_CONFIG, "rt");
    if(!pf)
    {
        printf("No se pudo abrir el archivo %s\n", ARCHIVO_CONFIG);
        return ERR_ARCH;
    }

    char clave[50];
    int  valor;

    while(fscanf(pf, "%49[^=]=%d\n", clave, &valor) == 2) //49 es el maximo de caracteres a leer si no encontro el '='
    {
        if      (strcmp(clave, "cantidad_posiciones") == 0) config->cantidad_posiciones = valor;
        else if (strcmp(clave, "vidas_inicio")        == 0) config->vidas_inicio        = valor;
        else if (strcmp(clave, "maximo_bandidos")     == 0) config->maximo_bandidos     = valor;
        else if (strcmp(clave, "maximo_premios")      == 0) config->maximo_premios      = valor;
        else if (strcmp(clave, "maximo_vidas_extra")  == 0) config->maximo_vidas_extra  = valor;
        else if (strcmp(clave, "maximo_oasis")        == 0) config->maximo_oasis        = valor;
        else if (strcmp(clave, "maximo_tormentas")    == 0) config->maximo_tormentas    = valor;
    }
    fclose(pf);
    return TODO_OK;
}

void Menu()
{
    int opcion=0;
    do
    {
        printf("\n-----------------------------\n");
        printf("    CARAVANA DEL DESIERTO    \n");
        printf("-------------------------------\n");
        printf("[1] Jugar nueva partida\n");
        printf("[2] Ver ranking\n");
        printf("[3] Salir\n");
        printf("-------------------------------\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
            jugarPartida();
            break;
        case 2:
            //mostrarRanking();
            break;
        case 3:
            printf("\nSaliendo...\n\n");
            break;
        default:
            printf("\nOpcion invalida. Intente nuevamente.\n");
            break;
        }
    }
    while (opcion != 3);
}


int tirarDado()
{
    return rand() % 6 + 1;
}

void jugarPartida()
{
    int valor = tirarDado();
    printf("Sacaste: %d\n", valor);
}

