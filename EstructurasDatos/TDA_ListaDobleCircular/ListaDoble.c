#include "ListaDoble.h"


void crearLista(tListaDoble *pl) {
    *pl = NULL;
}


int vaciarLista(tListaDoble *pl) {
    int cant = 0;
    tNodoDoble *actual = *pl;
    tNodoDoble *ultimo;
    tNodoDoble *aux;
    if (actual) {
        ultimo = actual->anterior;
        ultimo->siguiente = NULL;
        while (actual) {
            aux = actual->siguiente;
            free(actual->info);
            free(actual);
            actual = aux;
            cant++;
        }
    }
    *pl = NULL;
    return cant;
}


int listaVacia(const tListaDoble *pl) {
    return *pl == NULL;
}



int insertarAlFinal(tListaDoble *pl, const void *d, unsigned cantBytes) {
    tNodoDoble *nuevo;
    tNodoDoble *primero;
    tNodoDoble *ultimo;

    if ((nuevo = (tNodoDoble *)malloc(sizeof(tNodoDoble))) == NULL ||
        (nuevo->info = malloc(cantBytes)) == NULL) {
        free(nuevo);
        return SIN_MEM;
    }
    memcpy(nuevo->info, d, cantBytes);
    nuevo->tamInfo = cantBytes;
    if (*pl == NULL) {
        nuevo->siguiente = nuevo;
        nuevo->anterior = nuevo;
        *pl = nuevo;
    } else {
        primero = *pl;
        ultimo = primero->anterior;
        nuevo->siguiente = primero;
        nuevo->anterior = ultimo;
        ultimo->siguiente = nuevo;
        primero->anterior = nuevo;
    }
    return TODO_OK;
}


int insertarAlComienzo(tListaDoble *pl, const void *d, unsigned cantBytes) {
    if (!insertarAlFinal(pl, d, cantBytes))
        return SIN_MEM;
    *pl = (*pl)->anterior;
    return TODO_OK;
}

int BuscarElementoListaDC(const tListaDoble *lista, const void *aBuscar, void *elem, size_t tamElem, tCmp cmp)
{
    tNodoDoble *act;
    act = *lista;

    if(!act)
        return LISTA_VACIA;
    do
    {
        if(cmp(act->info, aBuscar)==0)
        {
            memcpy(elem,act->info,MIN(tamElem, act->tamInfo));
            return TODO_OK;
        }
        act = act->siguiente;
    }while(act != *lista);

    return NOT_FOUND_ELEM;
}


int recorrerListaDobleCircular(tListaDoble *pl, void *ctx, void (*func)(void *dato, void *ctx))
{
    tNodoDoble *act = *pl;
    int continuar = 1;

    if (act == NULL) {
        return LISTA_VACIA;

    }

    do {
        /* La función ahora nos responde si debemos seguir o frenar */
        continuar = func(act->info, ctx);

        if (continuar != 0) {
            act = act->siguiente;
        }

    } while (act != *pl && continuar != 0); /* El bucle frena naturalmente si 'continuar' se vuelve 0 */

    return 1;
}


