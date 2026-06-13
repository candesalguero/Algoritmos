#include "Arbol_Header.h"

void Arbol_Crear(tArbol *arbol)
{
    *arbol = NULL;
}
int Arbol_Insertar(tArbol *arbol, const void* elem, size_t tamElem, tCmp cmp)
{
    int resultado = 0;
    while(*arbol)/*/recorremos hasta el final del arbol*/
    {
        resultado = cmp(elem, (*arbol)->info);
        if(resultado < 0  )
            arbol = &(*arbol)->izq;

        else if(resultado > 0  )
            arbol = &(*arbol)->der;

        else/*/si es 0, significa que está repetido*/
            return ELEM_REPETIDO;
    }
    /* arbol ya es nulo, si falla el malloc, no se pierde info*/
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

int Arbol_BusquedaBinaria(const tArbol *arbol, const void *aBuscar, void *encontrado, size_t tamEncontrado, tCmp cmp)
{
    int res;
    while(*arbol)
    {
        res = cmp(aBuscar, (*arbol)->info);
        if(res < 0)
            arbol = &(*arbol)->izq;
        else if(res > 0)
            arbol = &(*arbol)->der;
        else
        {
            memcpy(encontrado, (*arbol)->info, MIN(tamEncontrado, (*arbol)->tamElem));
            return TODO_OK;
        }
    }
    return NOT_FOUND_ELEM;
}

int Arbol_ObtenerAltura(tArbol *arbol)
{
    int AlturaIzq, AlturaDer;
    if(!*arbol)
        return 0;
    AlturaIzq = Arbol_ObtenerAltura(&(*arbol)->izq);
    AlturaDer = Arbol_ObtenerAltura(&(*arbol)->der);

    return MAX(AlturaIzq, AlturaDer) +1;
}

void Arbol_RecorrerInOrden(const tArbol *arbol, tMostrar mostrar)
{
    if(!*arbol)
        return;
    Arbol_RecorrerInOrden(&(*arbol)->izq, mostrar);
    mostrar((*arbol)->info);
    Arbol_RecorrerInOrden(&(*arbol)->der, mostrar);
}
void Arbol_RecorrerPreOrden(const tArbol *arbol, tMostrar mostrar)
{
    if(!*arbol)
        return;
    mostrar( (*arbol)->info);
    Arbol_RecorrerPreOrden(&(*arbol)->izq, mostrar);
    Arbol_RecorrerPreOrden(&(*arbol)->der, mostrar);

}

void Arbol_Destruir(tArbol *arbol)
{
    if(!*arbol)
        return;

    Arbol_Destruir(&(*arbol)->izq);
    Arbol_Destruir(&(*arbol)->der);
    if(! (*arbol)->izq && !(*arbol)->der )/*/si es una hoja, la corto*/
    {
        free((*arbol)->info);
        free(*arbol);
        *arbol = NULL;
    }
}


int Arbol_GuardarEnArchivo(const tArbol *arbol, const char *arch, int metodo)
{
    FILE *pf = fopen(arch,"wb");
    if(!pf)
        return ERR_ARCH;

    if(metodo == IN_ORDEN)
        _Arbol_GuardarArchInOrden(arbol, pf);
    else if(metodo == PRE_ORDEN)
        _Arbol_GuardarArchPreOrden(arbol, pf);

    fclose(pf);
    return TODO_OK;
}

void _Arbol_GuardarArchInOrden(const tArbol *arbol, FILE *pf)
{
    if(!*arbol)
        return;
    _Arbol_GuardarArchInOrden(&(*arbol)->izq, pf);
    fwrite( (*arbol)->info, (*arbol)->tamElem, 1, pf );
    _Arbol_GuardarArchInOrden(&(*arbol)->der,pf);
}

void _Arbol_GuardarArchPreOrden(const tArbol *arbol, FILE *pf)
{
    if(!*arbol)
        return;

    fwrite((*arbol)->info, (*arbol)->tamElem, 1, pf);
    _Arbol_GuardarArchPreOrden(&(*arbol)->izq, pf);
    _Arbol_GuardarArchPreOrden(&(*arbol)->der, pf);

}


int Arbol_GenerarIndiceBalanceado(tArbol *arbolIdx, const char *archIdx, size_t tamElemIdx)
{
    FILE *pf ;
    void *elemArch ;
    int cantReg, resu;
    int tope, base = 0;

    pf = fopen(archIdx, "r+b");
    elemArch = malloc (tamElemIdx);

    if(!pf)
        return ERR_ARCH;

    if(!elemArch)
    {
        fclose(pf);
        return SIN_MEM;
    }
    fseek(pf, 0,SEEK_END);
    cantReg = (ftell(pf)/tamElemIdx) - 1;/*/menos 1 para manejarnos con [0-regTot]*/
    tope = cantReg;
    /*/usamos envoltorio para la recursividad*/
    resu = _Arbol_InsertarBalanceado(arbolIdx,elemArch,tamElemIdx,base,tope,pf);

    fclose(pf);
    free(elemArch);

    return resu;

}

int _Arbol_InsertarBalanceado(tArbol *arbol, void *elemArch, size_t tamElemIdx ,int base, int tope, FILE *pf)
{
    int medio, resu;
    if(base > tope)/*//condicion de fin*/
        return TODO_OK;

    medio = (base + tope) / 2; /*/el medio es derivado*/
    fseek(pf,tamElemIdx * medio,SEEK_SET);/*SEEK_SET porque es desde el principio*/
    fread( elemArch, tamElemIdx,1,pf );
    if(!*arbol)
    {
        *arbol = malloc(sizeof(tNodoArbol));
        if(!*arbol)
            return SIN_MEM;
        (*arbol)->info = malloc(tamElemIdx);
        if(!(*arbol)->info)
        {
            free(*arbol);
            return SIN_MEM;
        }
        (*arbol)->tamElem = tamElemIdx;
        (*arbol)->izq = NULL;
        (*arbol)->der = NULL;
        memcpy((*arbol)->info, elemArch, tamElemIdx);
    }

    resu = _Arbol_InsertarBalanceado( &(*arbol)->izq, elemArch,tamElemIdx, base, medio-1, pf);
    if (resu != TODO_OK)
        return resu;
    resu = _Arbol_InsertarBalanceado( &(*arbol)->der, elemArch, tamElemIdx, medio+1, tope, pf);

    if(resu != TODO_OK)
        return resu;

    return TODO_OK;
}

int Arbol_CargarIndiceDesdeArchivo(tArbol *arbolIdx, const char *arch, size_t tamElemArch, size_t tamElemIdx,tCmp cmp, tAccion Copiar)
{
    FILE *pf;
    void *elemArch, *elemIdx;
    int resu;
    unsigned reg=0;
    pf  = fopen(arch,"rb");
    elemArch = malloc(tamElemArch);
    elemIdx = malloc(tamElemIdx);
    if(!pf)
        return ERR_ARCH;
    if(!elemArch)
    {
        fclose(pf);
        return SIN_MEM;
    }

    while(fread(elemArch,tamElemArch,1,pf))
    {
        Copiar(elemIdx, elemArch, &reg);
        resu = Arbol_Insertar(arbolIdx,elemIdx,tamElemIdx,cmp);
        if(resu != TODO_OK)
        {
            fclose(pf);
            free(elemArch);
            free(elemIdx);
            return resu;
        }
        reg++;
    }
    fclose(pf);
    free(elemArch);
    free(elemIdx);
    return TODO_OK;
}


int Arbol_EliminarNodo(tArbol *arbol, const void *aElim, void *elem, size_t tamElem, tCmp cmp)
{
    while(*arbol)
    {
        if(cmp(aElim, (*arbol)->info) > 0)
            arbol = &(*arbol)->der;
        else if(cmp(aElim, (*arbol)->info)< 0)
            arbol = &(*arbol)->izq;
        else
        {
            /*/nos tenemos que fijar si es raiz o hoja*/
            if( !(*arbol)->izq && !(*arbol)->der )/*/SI ES HOJA*/
            {
                memcpy(elem, (*arbol)->info, MIN(tamElem, (*arbol)->tamElem));
                _Arbol_EliminarHoja(arbol);
                return TODO_OK;
            }

            else /*/es RAIZ*/
            {
                memcpy(elem, (*arbol)->info, MIN(tamElem, (*arbol)->tamElem));
                _Arbol_EliminarRaiz(arbol);/*/aca busco el mayor de los menores*/
                return TODO_OK;
            }
        }
    }
    return NOT_FOUND_ELEM;
}

void _Arbol_EliminarHoja(tArbol *arbol)
{
    free((*arbol)->info);
    free(*arbol);
    *arbol = NULL;/*//SIEMPRE QUE BORRO, LO PONGO EN NULL*/
}

void _Arbol_EliminarRaiz(tArbol *arbol)
{
    int AlturaIzq, AlturaDer;
    tNodoArbol **aRemplazar, *elim;

    AlturaIzq = Arbol_ObtenerAltura(&(*arbol)->izq);
    AlturaDer = Arbol_ObtenerAltura(&(*arbol)->der);


    if(AlturaIzq >= AlturaDer)
    {
        aRemplazar = _Arbol_BuscarMayorClave(&(*arbol)->izq);/*/ya tenemos al MA ME*/
        elim = *aRemplazar;
        *aRemplazar = elim->izq;
    }
    else
    {
        aRemplazar =_Arbol_BuscarMenorClave(&(*arbol)->der);/*/ya tenemos al MENOR MAYOR*/
        elim = *aRemplazar;
        *aRemplazar = elim->der;
    }

    memcpy((*arbol)->info, elim->info, elim->tamElem);
    (*arbol)->tamElem = elim->tamElem;
    free(elim->info);
    free(elim);
}

tNodoArbol **_Arbol_BuscarMayorClave(tArbol *arbol)
{
    if(!(*arbol)->der)
        return arbol;
    return _Arbol_BuscarMayorClave(&(*arbol)->der);
}
tNodoArbol **_Arbol_BuscarMenorClave(tArbol *arbol)
{
    if(!(*arbol)->izq)
        return arbol;
    return _Arbol_BuscarMenorClave(&(*arbol)->izq);
}

