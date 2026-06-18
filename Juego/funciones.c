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
    if(*arbolIdx)/*nos debemos asegurar que el arbol esté vacio antes de cargarlo*/
        Arbol_Destruir(arbolIdx);
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

int InicializarIndice(tArbol *arbolIdx,const char *archJugadores, const char *archIdx, tCmp cmp, tAccion Copiar)
{
    FILE *idx, *jug;
    tJugadorIdx jugIdx;
    idx = fopen(archIdx, "rb");
    if(idx)/*el indice existe, lo bajamos a el arbol*/
    {
        while(fread(&jugIdx, sizeof(tJugadorIdx),1,idx))
            Arbol_Insertar(arbolIdx, &jugIdx, sizeof(tJugadorIdx), cmp);
        fclose(idx);
        if(!Arbol_DetectarBalanceado(arbolIdx))
            {
                Arbol_Destruir(arbolIdx);
                GenerarIndiceJugadores(arbolIdx, archJugadores, archIdx, cmp, Copiar);
                /*si el arbol no está balanceado, lo volvemos a generar de 0*/
            }
        return TODO_OK;
    }
    /* Si el indice no existe hay que revisar si existe el de jugadores*/
    jug = fopen(archJugadores, "rb");
    if(!jug)/*si no existe ninguno es la PRIMERA VEZ que se inicia el juego*/
        return TODO_OK;
    /* si el de jugadores existe, significa que se corrompi� el de idx, lo volvemos a generar */
    fclose(jug);
    GenerarIndiceJugadores(arbolIdx, archJugadores, archIdx, cmp, Copiar);
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
    /*strcpy(nuevoJugador->nickname,jugador->nickname );*/
    jugador->partidas_jugadas = 0;
    ObtenerFechaActual(jugador->fechaIngreso);
    fwrite(jugador, sizeof(tJugador),1,pf);
    reg = ftell(pf)/sizeof(tJugador) - 1;/*Despues de escribir est� en EOF  */
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

    fseek(pf,sizeof(tJugador) * nuevoJugador->registro,SEEK_SET);
    fread(jugador, sizeof(tJugador), 1 , pf);
    fclose(pf);
    return TODO_OK;
}

void LeerCadena(char *dest, size_t longCad)
{
    char *p;
    if(fgets(dest, longCad, stdin) == NULL)
    {
        *dest = '\0';
        return;
    }
    p = strchr(dest, '\n');
    if(p)
        *p = '\0';
    else
        limpiarBuffer();

}

void limpiarBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void Pausar()
{
    system("pause");
}

void LimpiarPantalla()
{
    system("cls");
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
            puts("Error Desconocido");
        }break;
    }
    printf("\ncodError:%d",codError);
}

void mostrarMenu()
{
    printf("\n==============================\n");
    printf("   CARAVANA DEL DESIERTO\n");
    printf("==============================\n");
    printf("1. Nueva Partida\n");
    printf("2. Ver Ranking de Jugadores\n");
    printf("3. Salir\n");
    printf("------------------------------\n");
    printf("Seleccione una opcion: ");
}

