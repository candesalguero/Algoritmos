#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Headers/funciones.h"
#include "Headers/ListaDoble.h"

int main() {
    tConfig config;
    tListaDoble ruta_desierto;
    int opcion;
    srand(time(NULL));
    // 1. Cargar la configuración principal del juego
    if(!cargarConfiguracion("config.txt", &config)) {
        return 1; // Terminamos si no hay archivo de configuración
    }

    // Bucle principal del menú
    do {
        mostrarMenu();
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                generarEscenario(&config, &ruta_desierto);
                iniciarPartida(&config, &ruta_desierto);
                break;
            case 2:
                // Ver árbol binario de ranking
                break;
            case 3:
                printf("Saliendo del desierto... Hasta pronto!\n");
                break;
            default:
                printf("Opcion invalida.\n");
        }
    } while(opcion != 3);

    return 0;
}
