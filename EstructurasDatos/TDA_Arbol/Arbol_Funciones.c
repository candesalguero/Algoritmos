#include "Arbol_Header.h"

void CrearArbol(tArbol *arbol)
{
    *arbol = NULL;
}
int InsertarOrdenadoEnArbol(tArbol *arbol, const void* elem, size_t tamElem, tCmp cmp)
{
    int resultado = 0;
    while(*arbol)//recorremos hasta el final del arbol
    {
        resultado = cmp(elem, (*arbol)->info);
        if(resultado < 0  )
            arbol = &(*arbol)->izq;

        else if(resultado > 0  )
            arbol = &(*arbol)->der;

        else//si es 0, significa que está repetido
            return ELEM_REPETIDO;
    }
    //*arbol ya es nulo, si falla el malloc, no se pierde info
    (*arbol) = malloc(sizeof(tNodoArbol));
    if(!(*arbol))
        return SIN_MEM;
    (*arbol)->info = malloc(tamElem);
    if(!(*arbol)->info)
    {
        free((*arbol)->info);
        return SIN_MEM;
    }
    memcpy((*arbol)->info, elem, tamElem);
    (*arbol)->tamElem = tamElem;
    (*arbol)->izq = NULL;
    (*arbol)->der = NULL;

    return TODO_OK;
}

int AlturaArbol(tArbol *arbol)
{
    if(!*arbol)
        return 0;
    int AlturaIzq = AlturaArbol(&(*arbol)->izq);
    int AlturaDer = AlturaArbol(&(*arbol)->der);

    return MAX(AlturaIzq, AlturaDer) +1;
}

void RecorrerInOrden(const tArbol *arbol, tMostrar mostrar)
{
    if(!*arbol)
        return;
    RecorrerInOrden(&(*arbol)->izq, mostrar);
    mostrar((*arbol)->info);
    RecorrerInOrden(&(*arbol)->der, mostrar);
}
void RecorrerPreOrden(const tArbol *arbol, tMostrar mostrar)
{
    if(!*arbol)
        return;
    mostrar( (*arbol)->info);
    RecorrerPreOrden(&(*arbol)->izq, mostrar);
    RecorrerPreOrden(&(*arbol)->der, mostrar);

}

void TalarArbol(tArbol *arbol)
{
    if(!*arbol)
        return;

    TalarArbol(&(*arbol)->izq);
    TalarArbol(&(*arbol)->der);
    if(! (*arbol)->izq && !(*arbol)->der )//si es una hoja, la corto
    {
        free((*arbol)->info);
        free(*arbol);
        *arbol = NULL;
    }
}


int CargarArchivo_fromArbol(const tArbol *arbol, const char *arch, int metodo)
{
    FILE *pf = fopen(arch,"wb");
    if(!pf)
        return ERR_ARCH;

    if(metodo == IN_ORDEN)
        _CargarArchInOrden(arbol, pf);
    else if(metodo == PRE_ORDEN)
        _CargarArchPreOrden(arbol, pf);

    fclose(pf);
    return TODO_OK;
}

void _CargarArchInOrden(const tArbol *arbol, FILE *pf)
{
    if(!*arbol)
        return;
    _CargarArchInOrden(&(*arbol)->izq, pf);
    fwrite( (*arbol)->info, (*arbol)->tamElem, 1, pf );
    _CargarArchInOrden(&(*arbol)->der,pf);
}

void _CargarArchPreOrden(const tArbol *arbol, FILE *pf)
{
    if(!*arbol)
        return;

    fwrite((*arbol)->info, (*arbol)->tamElem, 1, pf);
    _CargarArchPreOrden(&(*arbol)->izq, pf);
    _CargarArchPreOrden(&(*arbol)->der, pf);

}


int ArbolBalanceado_fromArchOrdenado(tArbol *arbolIdx, const char *arch, size_t tamElemOr, size_t tamElemIdx ,tAccion accion )
{
    FILE *pf = fopen(arch, "r+b");
    if(!pf)
        return ERR_ARCH;
    fseek(pf, 0,SEEK_END);

    void * elemArch = malloc (tamElemOr);
    if(!elemArch)
    {
        fclose(pf);
        return SIN_MEM;
    }
    int cantReg = (ftell(pf)/tamElemOr) - 1;//menos 1 para manejarnos con [0-regTot]
    int tope = cantReg, base = 0;
    //usamos envoltorio para la recursividad
    _InsersionBalanceada(arbolIdx,elemArch, tamElemOr,tamElemIdx,base,tope,pf,accion);

    fclose(pf);
    free(elemArch);
    return TODO_OK;

}

void _InsersionBalanceada(tArbol *arbol, void *elemArch ,size_t tamElemOr, size_t tamElemIdx ,int base, int tope, FILE *pf, tAccion CopiarIndice )
{
    if(base > tope)///condicion de fin
        return;
    int medio = (base + tope) / 2; //el medio es derivado

    fseek(pf,tamElemOr * medio,SEEK_SET);///SEEK_SET porque es desde el principio
    fread( elemArch, tamElemOr,1,pf );
    if(!*arbol)
    {
        *arbol = malloc(sizeof(tNodoArbol));
        if(!*arbol)
            return ;
        (*arbol)->info = malloc(tamElemIdx);
        if(!(*arbol)->info)
        {
            free(*arbol);
            return ;///falta el sinMem para manejo de errores
        }
        (*arbol)->tamElem = tamElemIdx;
        (*arbol)->izq = NULL;
        (*arbol)->der = NULL;
        CopiarIndice((*arbol)->info, elemArch, &medio);//medio es el n° de registro
    }
    ///para anidar el SIN_MEM, tendría que poner return Inser... ???
    _InsersionBalanceada( &(*arbol)->izq, elemArch, tamElemOr, tamElemIdx, base, medio-1, pf, CopiarIndice);

    _InsersionBalanceada( &(*arbol)->der, elemArch, tamElemOr, tamElemIdx, medio+1, tope, pf, CopiarIndice);
}

int BajarArchivoArbol(tArbol *arbol, const char *arch, size_t tamElem ,tCmp cmp)
{
    FILE *pf = fopen(arch,"rb");
    if(!pf)
        return ERR_ARCH;
    void *elemArch = malloc(tamElem);
    if(!elemArch)
    {
        fclose(pf);
        return SIN_MEM;
    }

    int resu;
    while(fread(elemArch,tamElem,1,pf))
    {
        resu = InsertarOrdenadoEnArbol(arbol,elemArch,tamElem,cmp);
        if(resu != TODO_OK)
        {
            fclose(pf);
            free(elemArch);
            return resu;
        }
    }
    fclose(pf);
    free(elemArch);
    return TODO_OK;
}


int EliminarNodo(tArbol *arbol, const void *aElim, void *elem, size_t tamElem, tCmp cmp)
{
    while(*arbol)
    {
        if(cmp(aElim, (*arbol)->info) > 0)
            arbol = &(*arbol)->der;
        else if(cmp(aElim, (*arbol)->info)< 0)
            arbol = &(*arbol)->izq;
        else
        {
            //nos tenemos que fijar si es raiz o hoja
            if( !(*arbol)->izq && !(*arbol)->der )//SI ES HOJA
            {
                memcpy(elem, (*arbol)->info, MIN(tamElem, (*arbol)->tamElem));
                CortarHoja(arbol);
                return TODO_OK;
            }

            else //es RAIZ
            {
                memcpy(elem, (*arbol)->info, MIN(tamElem, (*arbol)->tamElem));
                EliminarRaiz(arbol);//aca busco el mayor de los menores
                return TODO_OK;
            }
        }
    }
    return NOT_FOUND_ELEM;
}

void CortarHoja(tArbol *arbol)
{
    free((*arbol)->info);
    free(*arbol);
    *arbol = NULL;///SIEMPRE QUE BORRO, LO PONGO EN NULL
}

void EliminarRaiz(tArbol *arbol)
{
    int AlturaIzq, AlturaDer;
    tNodoArbol **aRemplazar, *elim;

    AlturaIzq = AlturaArbol(&(*arbol)->izq);
    AlturaDer = AlturaArbol(&(*arbol)->der);


    if(AlturaIzq >= AlturaDer)
    {
        aRemplazar = _BuscarMayorClave(&(*arbol)->izq);//ya tenemos al MA ME
        elim = *aRemplazar;
        *aRemplazar = elim->izq;
    }
    else
    {
        aRemplazar =_BuscarMenorClave(&(*arbol)->der);//ya tenemos al MENOR MAYOR
        elim = *aRemplazar;
        *aRemplazar = elim->der;
    }

    memcpy((*arbol)->info, elim->info, elim->tamElem);
    (*arbol)->tamElem = elim->tamElem;
    free(elim->info);
    free(elim);
}

tNodoArbol **_BuscarMayorClave(tArbol *arbol)
{
    if(!(*arbol)->der)
        return arbol;
    return _BuscarMayorClave(&(*arbol)->der);
}
tNodoArbol **_BuscarMenorClave(tArbol *arbol)
{
    if(!(*arbol)->izq)
        return arbol;
    return _BuscarMenorClave(&(*arbol)->izq);
}

