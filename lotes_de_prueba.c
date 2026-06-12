#include "lotes_de_prueba.h"

int GuardarArchivo(const char *arch, const void *vec, size_t tamElem,size_t cant )
{
    FILE *pf = fopen(arch, "wb");
    if(!pf)
        return ERR_ARCH;

    fwrite(vec,tamElem, cant, pf);
    fclose(pf);
    return TODO_OK;
}

int MostrarArchivo(const char *arch, size_t tamElem, tMostrar mostrar)
{
    FILE *pf = fopen(arch, "rb");
    if(!pf)
        return ERR_ARCH;
    void *elem = malloc(tamElem);
    if(!elem)
    {
        fclose(pf);
        return SIN_MEM;
    }

    while(fread(elem,tamElem,1,pf))
        mostrar(elem);

    fclose(pf);
    free(elem);
    return TODO_OK;
}



