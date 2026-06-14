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

int GenerarIndiceJugadores(tArbol *arbolIdx,const char *archJugadores, const char *archIdx, tCmp cmp, tAccion Copiar)
{
    int resu;

    resu = Arbol_CargarIndiceDesdeArchivo(arbolIdx, archJugadores,sizeof(tJugador),sizeof(tJugadorIdx),cmp,Copiar);
    if(resu != TODO_OK)
    {
        Arbol_Destruir(arbolIdx);
        return resu;
    }

    resu = Arbol_GuardarEnArchivo(arbolIdx, archIdx, IN_ORDEN);
    if(resu != TODO_OK)
    {
        Arbol_Destruir(arbolIdx);
        return resu;
    }

    Arbol_Destruir(arbolIdx);/**Una vez cargado el indice ordenado
    Destruimos el arbol para volver a cargarlo        */

    resu = Arbol_GenerarIndiceBalanceado(arbolIdx,archIdx,sizeof(tJugadorIdx));
    if(resu != TODO_OK)
    {
        Arbol_Destruir(arbolIdx);
        return resu;
    }
    resu = Arbol_GuardarEnArchivo(arbolIdx, archIdx, PRE_ORDEN);
    if(resu != TODO_OK)
    {
        Arbol_Destruir(arbolIdx);
        return resu;
    }

    return TODO_OK;
}

int AltaJugadores(tJugador *jugador, tJugadorIdx *nuevoJugador, const char *archJugadores)
{
    FILE *pf;
    unsigned reg;
    pf = fopen(archJugadores,"ab");
    if(!pf)
        return ERR_ARCH;

    strcpy(jugador->nickname, nuevoJugador->nickname);
    jugador->partidas_jugadas = 0;
    ObtenerFechaActual(jugador->fechaIngreso);
    fwrite(jugador, sizeof(tJugador),1,pf);
    reg = ftell(pf)/sizeof(tJugador) - 1;/*Despues de escribir está en EOF  */
    nuevoJugador->registro = reg;

    fclose(pf);
    return TODO_OK;
}

void ObtenerFechaActual(char *fechaDest)
{
    time_t tiempoActual = time(NULL);
    struct tm *tiempo_local = localtime(&tiempoActual);
    strftime(fechaDest, 11, "%d/%m/%Y",tiempo_local);
}

int BuscarDatosJugadores(tJugador *jugador, tJugadorIdx *nuevoJugador, const char *archJugadores)
{
    FILE *pf;

    pf = fopen(archJugadores,"r+b");
    if(!pf)
        return ERR_ARCH;
    fread(jugador, sizeof(tJugador), nuevoJugador->registro, pf);
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



