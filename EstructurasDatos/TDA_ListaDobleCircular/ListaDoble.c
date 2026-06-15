#include "ListaDoble.h"


void crearLista(tListaDoble *pl) {
    *pl = NULL;
}


int vaciarListaDobleC(tListaDoble *pl) {
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


int recorrerListaDC_Condicionada(tListaDoble *pl, void *ctx, tAccionCond cond)
{
    tNodoDoble *act = *pl;
    int continuar = 1;

    if (act == NULL) 
        return LISTA_VACIA;


    do {
        /* La función ahora nos responde si debemos seguir o frenar */
        continuar = cond(act->info, ctx);

        if (continuar != 0) {
            act = act->siguiente;
        }

    } while (act != *pl && continuar != 0); /* El bucle frena naturalmente si 'continuar' se vuelve 0 */

    return TODO_OK;
}

/* Se desplaza 'cantidad' nodos desde el inicio de la lista (positivo = hacia
   adelante con 'siguiente', negativo = hacia atras con 'anterior') y al llegar
   al nodo destino ejecuta 'accion' sobre su dato. NO mueve el puntero de la lista. */
void desplazarYAplicar(tListaDoble *pl, int cantidad, void *param, tAccionDesplaz accion)
{
    tNodoDoble *actual = *pl;
    int i;

    if(actual == NULL)
        return;

    if(cantidad >= 0)
    {
        for(i = 0; i < cantidad; i++)
            actual = actual->siguiente;
    }
    else
    {
        for(i = 0; i < -cantidad; i++)
            actual = actual->anterior;
    }

    accion(actual->info, param);
}

/* Busca un elemento por clave y devuelve el puntero al dato real (no una copia).
   Devuelve NULL si la lista esta vacia o si no lo encuentra. */
void *buscarPtrElementoListaDC(const tListaDoble *lista, const void *aBuscar, tCmp cmp)
{
    tNodoDoble *actual = *lista;

    if(!actual)
        return NULL;

    do
    {
        if(cmp(actual->info, aBuscar) == 0)
            return actual->info;

        actual = actual->siguiente;

    }while(actual != *lista);

    return NULL;
}


