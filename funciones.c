#include "funciones.h"

int leerConfig(tConfiguracion* config, const char *arch)
{
    FILE *pf=fopen(arch, "rt");
    char clave[50];
    int  valor;

    if(!pf)
        return ERR_ARCH;

    while(fscanf(pf, "%49[^=]=%d\n", clave, &valor) == 2) /*/49 es el maximo de caracteres a leer si no encontro el '='*/
    {
        if(strcmp(clave, "cantidad_posiciones")       == 0)
            config->cantidad_posiciones = valor;
        else if (strcmp(clave, "vidas_inicio")        == 0)
            config->vidas_inicio        = valor;
        else if (strcmp(clave, "maximo_bandidos")     == 0)
            config->maximo_bandidos     = valor;
        else if (strcmp(clave, "maximo_premios")      == 0)
            config->maximo_premios      = valor;
        else if (strcmp(clave, "maximo_vidas_extra")  == 0)
            config->maximo_vidas_extra  = valor;
        else if (strcmp(clave, "maximo_oasis")        == 0)
            config->maximo_oasis        = valor;
        else if (strcmp(clave, "maximo_tormentas")    == 0)
            config->maximo_tormentas    = valor;
    }
    fclose(pf);
    return TODO_OK;
}

void ManejoErrores(int codError, const char *arch)
{
    switch(codError)
    {
    case SIN_MEM:
        {
            puts("\nEl sistema se encuentra con problemas de memoria");
        }break;
    case ERR_ARCH:
        {
            printf("\nError al abrir el archivo '%s'", arch);
        }break;
    case LISTA_VACIA:
        {
            puts("\nLa lista se encuentra vacia");
        }break;
    case NOT_FOUND_ELEM:
        {
            puts("El elemento buscado no se ha encontrado");
        }break;
    case ELEM_REPETIDO:
        {
            puts("El elemento ingresado ya está en la estructura");
        }break;
    case COLA_LLENA:
        {
            puts("La cola esta llena");
        }break;
    case COLA_VACIA:
        {
            puts("La cola esta vacia");
        }break;
    default:
        {

        }break;
    }
    printf("\ncodError:%d",codError);
}

void Menu()
{
    printf("\n-----------------------------\n");
    printf("    CARAVANA DEL DESIERTO    \n");
    printf("-------------------------------\n");
    printf("[1] Jugar nueva partida\n");
    printf("[2] Ver ranking\n");
    printf("[3] Salir\n");
    printf("-------------------------------\n");
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

