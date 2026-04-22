#include "config.h"
#include "funciones.h"

int main()
{
    tConfiguracion config;
    if (!leerConfig(&config))
    {
        return 1; // Terminamos si no hay archivo de configuración
    }

    srand((unsigned int)time(NULL)); //srand le pasa la hora actual como punto de arranque para que el resultado del dado (rand) no siga siempre la misma secuencia
    Menu();

    return 0;
}
