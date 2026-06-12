#include "Lista_Header.h"

void CrearLista(tLista *lista)
{
    *lista = NULL;
}

int InsertarAlPrincipio(tLista *lista, const void *elem, size_t tamElem)
{
    tNodo *nue = malloc(sizeof(tNodo));
    if(!nue)
        return SIN_MEM;

    nue->info = malloc(tamElem);
    if(!nue->info)
    {
        free(nue);
        return SIN_MEM;
    }

    memcpy(nue->info, elem, tamElem);
    nue->tamElem = tamElem;
    nue->sig = *lista;

    *lista = nue;

    return TODO_OK;
}

int InsertarAlFinal(tLista *lista, const void *elem, size_t tamElem)
{
    tNodo *nue = malloc(sizeof(tNodo));
    if(!nue)
        return SIN_MEM;
    nue->info = malloc(tamElem);
    if(!nue->info)
    {
        free(nue);
        return SIN_MEM;
    }
    memcpy(nue->info, elem, tamElem);
    nue->tamElem = tamElem;
    nue->sig = NULL;
    if(*lista)
    {
        while( (*lista)->sig )//si sig es NULL, entonces estoy en el ultimo
            lista = &(*lista)->sig;

        (*lista)->sig = nue;//le digo al ult que lo sigue alguien
        lista = &(*lista)->sig;//me muevo a la DIRECCION DE MEMORIA del nuevo que voy a poner
        *lista = nue;//lo pongo
    }
    else
        *lista = nue;

    return TODO_OK;
}

int InsertarOrdenado(tLista *lista, const void *elem, size_t tamElem,tCmp cmp, int conDup, tAccion accion)
{
    tNodo *aux;
    aux = malloc(sizeof(tNodo));
    if(!aux)
        return SIN_MEM;

    aux->info = malloc(tamElem);
    if(!aux->info)
    {
        free(aux);
        return SIN_MEM;
    }
    memcpy(aux->info, elem, tamElem );
    aux->tamElem = tamElem;

    while(*lista && cmp(aux->info,(*lista)->info) > 0 )
        lista = &(*lista)->sig;

    if(!conDup && *lista && cmp(aux->info,(*lista)->info) == 0)
    {
        accion((*lista)->info, elem, NULL);
        free(aux->info);
        free(aux);
    }
    else
    {
        aux->sig = *lista;
        *lista = aux;
    }

    return TODO_OK;
}

int MostrarLista(tLista *lista, void Mostrar(const void*))
{
    if(!*lista)
        return LISTA_VACIA;

    while(*lista)
    {
        Mostrar( (*lista)->info);
        lista = &(*lista)->sig;
    }
    return TODO_OK;
}

int EliminarPorClaveOrdenada(tLista *lista, const void *clave, void* elem, size_t tamElem, tCmp cmp )
{
    tNodo *elim;
    if(!*lista)
        return LISTA_VACIA;

    while(*lista && cmp(clave, (*lista)->info)>0)
    {
        lista = &(*lista)->sig;
    }

    if(!*lista)
        return NOT_FOUND_ELEM;//no lo encontro
    else
    {
        elim = *lista;
        *lista = elim->sig;

        memcpy(elem,elim,MIN(tamElem, elim->tamElem));
        free(elim->info);
        free(elim);
    }

    return TODO_OK;
}

int EliminarPorClaveDesOrdenada(tLista *lista, const void *clave, tCmp cmp)
{
    tNodo *elim;

    if(!*lista)
        return LISTA_VACIA;

    while(*lista )
    {
        if(cmp(clave, (*lista)->info)==0)
        {
            elim = *lista;
            *lista = elim->sig;
            free(elim->info);
            free(elim);
        }
        lista = &(*lista)->sig;
    }
    return 1;
}

int VaciarLista(tLista *lista)
{
    tNodo *elim;
    while(*lista)
    {
        elim = *lista;
        *lista = (*lista)->sig;

        free(elim->info);
        free(elim);
    }
    return TODO_OK;
}

void OrdenarLista(tLista *lista, tCmp cmp)
{
    tNodo **ini, **min, **plis, *aux;
    ini = (tNodo**)lista;

    while(*ini)
    {
        plis = ini;
        min = ini;
        while(*plis)
        {
            if(cmp( (*plis)->info, (*min)->info ) < 0 )
            {
                min = plis;
            }
            plis = &(*plis)->sig;
        }
        aux = *min;
        if(ini != min)
        {
            *min = aux->sig;
            aux->sig = *ini;
            *ini = aux;

        }
        ini = &aux->sig;
    }
}


