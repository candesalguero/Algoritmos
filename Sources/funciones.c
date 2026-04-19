#include "../Headers/funciones.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Lee config.txt y lo vuelca en la estructura tConfig
int cargarConfiguracion(const char* ruta, tConfig* config) {
    FILE *archivo = fopen(ruta, "r");
    if (!archivo) {
        printf("Error: No se pudo abrir %s\n", ruta);
        return 0;
    }

    // Aquí implementaremos el parseo línea por línea
    // Por simplicidad en este esqueleto, asignamos valores por defecto o leemos el archivo con fscanf.
    char linea[150];
    char clave[50];
    int valor;

    // 1. Inicializamos con valores por defecto por seguridad
    config->cantidad_posiciones = 0;
    config->vidas_inicio = 0;
    config->maximo_bandidos = 0;
    config->maximo_premios = 0;
    config->maximo_vidas_extra = 0;
    config->maximo_oasis = 0;
    config->maximo_tormentas = 0;

    // 2. Leemos línea por línea hasta el final del archivo
    while (fgets(linea, sizeof(linea), archivo)) {

        // 3. Parseamos la línea: ignoramos espacios iniciales, leemos todo
        // hasta los dos puntos (la clave), leemos los dos puntos, y luego el entero.
        if (sscanf(linea, " %[^:]:%d", clave, &valor) == 2) {

            // 4. Comparamos la clave leída y asignamos el valor donde corresponde
            if (strcmp(clave, "cantidad_posiciones") == 0) {
                config->cantidad_posiciones = valor;
            }
            else if (strcmp(clave, "vidas_inicio") == 0) {
                config->vidas_inicio = valor;
            }
            else if (strcmp(clave, "maximo_bandidos") == 0) {
                config->maximo_bandidos = valor;
            }
            else if (strcmp(clave, "maximo_premios") == 0) {
                config->maximo_premios = valor;
            }
            else if (strcmp(clave, "maximo_vidas_extra") == 0) {
                config->maximo_vidas_extra = valor;
            }
            else if (strcmp(clave, "maximo_oasis") == 0) {
                config->maximo_oasis = valor;
            }
            else if (strcmp(clave, "maximo_tormentas") == 0) {
                config->maximo_tormentas = valor;
            }
        }
    }

    fclose(archivo);
    // Validación extra: Verificar que los datos mínimos se cargaron
    if (config->cantidad_posiciones <= 0 || config->vidas_inicio <= 0) {
        printf("Error: Archivo de configuracion corrupto o incompleto.\n");
        return 0;
    }

    return 1; // Verdadero, carga exitosa
}

// Lógica para inicializar la caravana.txt y la lista doble
void generarEscenario(tConfig* config, tListaDoble* ruta_desierto) {
    int i, pos_aleatoria;
    FILE *archivo;

    // 1. Nos aseguramos de empezar con una lista vacía
    crearLista(ruta_desierto);

    // 2. Usamos un arreglo temporal para planificar el mapa antes de armar la lista
    char* mapa = (char*)malloc(config->cantidad_posiciones * sizeof(char));
    if (!mapa) return;

    for(i = 0; i < config->cantidad_posiciones; i++) {
        mapa[i] = '.'; // Llenamos todo con ruta despejada
    }

    // 3. Ubicamos el Inicio (I) y la Salida (S)
    mapa[0] = 'I';
    // Ubicamos la salida en el punto medio del mapa circular
    mapa[config->cantidad_posiciones / 2] = 'S';

    // 4. Distribuimos los elementos aleatoriamente
    int cantidades[] = {config->maximo_bandidos, config->maximo_premios,
                        config->maximo_vidas_extra, config->maximo_oasis,
                        config->maximo_tormentas};
    char simbolos[] = {'B', 'P', 'V', 'O', 'T'};

    for (int tipo = 0; tipo < 5; tipo++) {
        int colocados = 0;
        // Seguimos intentando hasta colocar todos los elementos del tipo actual
        while(colocados < cantidades[tipo]) {
            pos_aleatoria = rand() % config->cantidad_posiciones;
            // Solo colocamos si el casillero está vacío
            if(mapa[pos_aleatoria] == '.') {
                mapa[pos_aleatoria] = simbolos[tipo];
                colocados++;
            }
        }
    }

    // 5. Abrimos el archivo para guardar el tablero visual
    archivo = fopen("caravana.txt", "w");
    if (!archivo) {
        printf("Error: No se pudo crear caravana.txt\n");
        free(mapa);
        return;
    }

    // 6. Volcamos el mapa planificado a la Lista Circular y al archivo
    for (i = 0; i < config->cantidad_posiciones; i++) {
        tPosicion pos;
        pos.numero_posicion = i + 1;
        pos.elemento = mapa[i];
        pos.tiene_jugador = (i == 0) ? 1 : 0; // El jugador arranca en la posición 1 (índice 0)

        // Insertamos el nodo en nuestra lista circular
        insertarAlFinal(ruta_desierto, &pos, sizeof(tPosicion));

        // Escribimos en el archivo con el formato requerido
        if (pos.tiene_jugador) {
            fprintf(archivo, "%02d:[%c J]\n", pos.numero_posicion, pos.elemento);
        } else {
            if (pos.elemento == '.') {
                fprintf(archivo, "%02d:.\n", pos.numero_posicion);
            } else {
                fprintf(archivo, "%02d:%c\n", pos.numero_posicion, pos.elemento);
            }
        }
    }

    fclose(archivo);
    free(mapa); // Liberamos el arreglo temporal
    printf("\n--> El desierto se ha generado! Revisa el archivo 'caravana.txt'.\n");
}
// --- LÓGICA DEL JUEGO ---
int tirarDado() {
    return (rand() % 6) + 1; // Genera un número del 1 al 6
}
void jugarTurno(tConfig *config, tListaDoble *ruta, tCola *colaMovimientos) {
    tMovimiento movActual;
    char eleccion;
    int posJugador = -1;

    // 1. Mostrar el mapa visual para que el jugador pueda armar su estrategia
    mostrarMapa(ruta);

    // 2. Buscar en qué posición exacta se encuentra el jugador
    tNodo *act = *ruta;
    if (act != NULL) {
        do {
            tPosicion *p = (tPosicion *)act->info;
            if (p->tiene_jugador) {
                posJugador = p->numero_posicion;
                break;
            }
            act = act->siguiente;
        } while (act != *ruta);
    }

    printf("\n--- NUEVO TURNO ---\n");
    printf("📍 Te encuentras en el casillero: %d\n", posJugador);

    // 3. Turno del Jugador
    movActual.entidad = 'J';
    movActual.id_entidad = 0;
    movActual.casillas = tirarDado();

    printf("🎲 Jugador, has sacado un %d en el dado.\n", movActual.casillas);

    // 4. Restricción táctica: Si está en el inicio, no puede retroceder
    if (posJugador == 1) {
        printf("⚠️ Como recien comienzas en el Campamento (I), solo puedes avanzar.\n");
        eleccion = 'F';
    } else {
        do {
            printf("¿Deseas moverte hacia (F) Adelante o (B) Atras? : ");
            scanf(" %c", &eleccion);
            if(eleccion >= 'a' && eleccion <= 'z') eleccion -= 32;
        } while (eleccion != 'F' && eleccion != 'B');
    }

    movActual.direccion = eleccion;
    acolar(colaMovimientos, &movActual);

    // 5. Turno de los Bandidos (Automático)
    for(int i = 1; i <= config->maximo_bandidos; i++) {
        movActual.entidad = 'B';
        movActual.id_entidad = i;
        movActual.casillas = tirarDado();
        movActual.direccion = (rand() % 2 == 0) ? 'F' : 'B';

        acolar(colaMovimientos, &movActual);
    }

    printf("\n✅ Movimientos de este turno encolados con exito.\n");
}
// --- VISUALIZADOR DEL TABLERO ---
void mostrarMapa(tListaDoble *ruta) {
    if (*ruta == NULL) return;

    printf("\n============================= MAPA DEL DESIERTO =============================\n");
    tNodo *act = *ruta;
    do {
        tPosicion *p = (tPosicion *)act->info;

        // Imprimimos el casillero actual
        if (p->tiene_jugador) {
            printf("[%c J] ", p->elemento);
        } else if (p->elemento == '.') {
            printf(". ");
        } else {
            printf("%c ", p->elemento);
        }

        act = act->siguiente;
    } while (act != *ruta);
    printf("\n=============================================================================\n");
}
// --- EJECUTOR DE MOVIMIENTOS ---
int ejecutarMovimientos(tListaDoble *ruta, tCola *colaMovimientos, int *vidas, int *puntos) {
    tMovimiento mov;

    // Desencolamos hasta que la cola quede vacía
    while (desencolar(colaMovimientos, &mov)) {

        tNodo *origen = *ruta;
        tPosicion *posOrigen = NULL;

        // 1. Buscar dónde está la entidad que se va a mover
        do {
            tPosicion *p = (tPosicion *)origen->info;
            // Si es el jugador, buscamos el flag. Si es bandido, buscamos la 'B'
            if ((mov.entidad == 'J' && p->tiene_jugador == 1) ||
                (mov.entidad == 'B' && p->elemento == 'B')) {
                posOrigen = p;
                break;
            }
            origen = origen->siguiente;
        } while (origen != *ruta);

        // Si encontramos la entidad, la movemos
        if (posOrigen != NULL) {
            tNodo *destino = origen;

            // 2. Desplazamiento mágico gracias a la Lista Circular
            for (int i = 0; i < mov.casillas; i++) {
                if (mov.direccion == 'F') {
                    destino = destino->siguiente;
                } else {
                    destino = destino->anterior; // Hacia atrás
                }
            }

            tPosicion *posDestino = (tPosicion *)destino->info;

            // 3. Aplicar el cambio en el tablero
            if (mov.entidad == 'J') {
                posOrigen->tiene_jugador = 0; // Borramos rastro viejo
                posDestino->tiene_jugador = 1; // Colocamos en el nuevo

                // Evaluar colisiones del jugador con el mapa
                if (posDestino->elemento == 'P') {
                    (*puntos)++;
                    posDestino->elemento = '.'; // Consumimos el premio
                    printf("¡Atrapaste un premio! Puntos: %d\n", *puntos);
                }
                else if (posDestino->elemento == 'S') {
                    printf("¡HAS LLEGADO A LA CIUDAD REFUGIO!\n");
                    return 0; // 0 significa que el juego termina (Victoria)
                }
            }
            else if (mov.entidad == 'B') {
                posOrigen->elemento = '.'; // El bandido deja su casilla

                // Si el bandido cae donde está el jugador
                if (posDestino->tiene_jugador == 1) {
                    (*vidas)--;
                    printf("¡UN BANDIDO TE HA INTERCEPTADO! Pierdes una vida. Vidas restantes: %d\n", *vidas);
                    // Aquí iría la lógica para devolver al jugador al Campamento Inicial (nodo 1)
                } else {
                    posDestino->elemento = 'B'; // El bandido ocupa la nueva casilla
                }
            }
        }
    }
    return 1; // 1 significa que el juego continúa
}

// --- BUCLE PRINCIPAL DEL JUEGO ---
void iniciarPartida(tConfig *config, tListaDoble *ruta) {
    int vidas = config->vidas_inicio;
    int puntos = 0;
    int jugando = 1;
    tCola colaMovimientos;

    crearCola(&colaMovimientos);

    printf("\n>>> INICIANDO TRAVESIA HACIA LA CIUDAD REFUGIO <<<\n");
    printf("Vidas iniciales: %d\n", vidas);

    // Este es el Bucle que repite los turnos hasta ganar o morir
    while (jugando == 1 && vidas > 0) {
        jugarTurno(config, ruta, &colaMovimientos);

        printf("\nResolviendo movimientos...\n");
        jugando = ejecutarMovimientos(ruta, &colaMovimientos, &vidas, &puntos);
    }

    if (vidas <= 0) {
        printf("\n💀 Has perdido todas tus vidas. El desierto te ha consumido.\n");
    }

    // Limpiamos la ruta al terminar la partida
    vaciarLista(ruta);
}

// UI Básica del juego
void mostrarMenu() {
    printf("\n==============================\n");
    printf("   CARAVANA DEL DESIERTO\n");
    printf("==============================\n");
    printf("1. Nueva Partida\n");
    printf("2. Ver Ranking de Jugadores\n");
    printf("3. Salir\n");
    printf("------------------------------\n");
    printf("Seleccione una opcion: ");
}
