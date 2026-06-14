#include "../Headers/Cola.h"

void crear_cola(tCola * pc)
{
    pc->pri = pc->ult = NULL;
}

int poner_en_cola(tCola * pc, const void * pd, unsigned tam)
{
    tNodoCola * nue = (tNodoCola *) malloc(sizeof(tNodoCola));
    if(!nue)
    {
        return 0; ///COLA_LLENA
    }
    nue->info = malloc(tam);
    if(!nue->info)
    {
        free(nue);
        return 0; ///COLA_LLENA
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
    return 1; ///OK
}

int sacar_de_cola(tCola * pc, void * pd, unsigned tam)
{
    tNodoCola * elim = pc->pri;
    if(!elim)
    {
        return 0; ///COLA_VACIA
    }
    memcpy(pd, elim->info, MINIMO(tam, elim->tamInfo));
    pc->pri = elim->sig;
    if(!pc->pri)
    {
        pc->ult = NULL;
    }
    free(elim->info);
    free(elim);
    return 1; ///OK
}

int frente_de_cola(const tCola * pc, void * dato, unsigned tam)
{
    if(pc->pri == NULL)
        return 0;
    memcpy(dato, pc->pri->info, MINIMO((pc->pri->tamInfo), tam));
    return 1;
}

int cola_llena(const tCola * pc, unsigned tam)
{
    tNodoCola *aux= (tNodoCola *) malloc(sizeof(tNodoCola));
    void *info= malloc(tam);
    free(aux);
    free(info);
    return aux==NULL || info==NULL;
}

int cola_vacia(const tCola * pc)
{
    return !pc->pri;
}

void vaciar_cola(tCola * pc)
{
    while(pc->pri)
    {
        tNodoCola *aux=pc->pri;
        pc->pri=aux->sig;
        free(aux->info);
        free(aux);
    }
    pc->ult=NULL;
}
