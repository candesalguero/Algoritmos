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
    FILE *pf ;
    void *elem ;

    pf = fopen(arch, "rb");
    elem = malloc(tamElem);

    if(!pf)
        return ERR_ARCH;
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

int GenerarLotePrueba()
{
    tJugador jugadores[] =
    {
        {"LunaGamer", "12/05/2026", 3},
        {"MaxPower", "15/05/2026", 2},
        {"NicoPro", "18/05/2026", 1},
        {"LilaPlay", "20/05/2026", 4},
        {"SofiWin", "22/05/2026", 2},
        {"LeoCrack", "25/05/2026", 1},
        {"MatiZ", "28/05/2026", 3},
        {"ValeX", "30/05/2026", 1},
        {"Tomi99", "01/06/2026", 2},
        {"FlorGame", "03/06/2026", 1}
    };

    tPartida partidas[] =
    {
        {1, "LunaGamer", 4, 20},
        {2, "MaxPower", 2, 25},
        {3, "NicoPro", 3, 15},
        {4, "LilaPlay", 5, 10},
        {5, "SofiWin", 1, 13},
        {6, "LeoCrack", 2, 12},
        {7, "MatiZ", 3, 21},
        {8, "ValeX", 5, 22},
        {9, "Tomi99", 3, 23},
        {10, "FlorGame", 1, 25},
        {11, "LunaGamer", 1, 24},
        {12, "LilaPlay", 6, 9},
        {13, "MatiZ", 5, 18},
        {14, "MaxPower", 3, 17},
        {15, "LilaPlay", 2, 20}
    };
       /**Creacion de lote de pruebas*/
    int cantJug = 0, cantPart = 0;
    cantJug = sizeof(jugadores)/sizeof(tJugador);
    cantPart = sizeof(partidas)/sizeof(tPartida);


    GuardarArchivo(ARCHIVO_JUGADORES,jugadores,sizeof(tJugador),cantJug);
    GuardarArchivo(ARCHIVO_PARTIDAS,partidas,sizeof(tPartida),cantPart);

    return TODO_OK;
}


