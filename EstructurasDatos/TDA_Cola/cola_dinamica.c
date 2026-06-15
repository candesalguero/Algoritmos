#include "cola_dinamica.h"

void crear_cola(tCola * pc)
{
    pc->pri = pc->ult = NULL;
}

int poner_en_cola(tCola * pc, const void * pd, unsigned tam)
{
    tNodoCola * nue = (tNodoCola *) malloc(sizeof(tNodoCola));
    if(!nue)
    {
        return SIN_MEM;
    }
    nue->info = malloc(tam);
    if(!nue->info)
    {
        free(nue);
        return SIN_MEM;
    }
    nue->tamInfo = tam;
    memcpy(nue->info, pd, tam);
    nue->sig = NULL;
    if(pc->ult)
    {
        pc->ult->sig = nue;
    }
    else
    {
        pc->pri = nue;
    }
    pc->ult = nue;
    return TODO_OK;
}

int sacar_de_cola(tCola * pc, void * pd, unsigned tam)
{
    tNodoCola * elim = pc->pri;
    if(!elim)
    {
        return COLA_VACIA;
    }
    memcpy(pd, elim->info, MIN(tam, elim->tamInfo));
    pc->pri = elim->sig;
    if(pc->pri == NULL)
    {
        pc->ult = NULL;
    }
    free(elim->info);
    free(elim);
    return TODO_OK;
}

int frente_de_cola(const tCola * pc, void * dato, unsigned tam)
{
    if(pc->pri == NULL)
        return COLA_VACIA;
    memcpy(dato, pc->pri->info, MIN((pc->pri->tamInfo), tam));
    return TODO_OK;
}

int cola_llena(const tCola * pc, unsigned tam)
{
    (void)pc; /* para solucionar el warning de la firma */
    (void)tam; /* para solucionar el warning de la firma */
    return TODO_OK;
}

int cola_vacia(const tCola * pc)
{
    return pc->pri== NULL;
}

void vaciar_cola(tCola * pc)
{
    tNodoCola *aux;
    while(pc->pri)
    {
        aux=pc->pri;
        pc->pri=aux->sig;
        free(aux->info);
        free(aux);
    }
    pc->ult=NULL;
}
