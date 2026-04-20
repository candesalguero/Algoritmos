#include "../Headers/funciones.h"


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

    crearLista(ruta_desierto);

    // Arreglo para el terreno y arreglo separado para los bandidos (inicializado en 0)
    char* mapa = (char*)malloc(config->cantidad_posiciones * sizeof(char));
    int* mapa_bandidos = (int*)calloc(config->cantidad_posiciones, sizeof(int));
    if (!mapa || !mapa_bandidos) return;

    for(i = 0; i < config->cantidad_posiciones; i++) {
        mapa[i] = '.';
    }

    mapa[0] = 'I';
    mapa[config->cantidad_posiciones / 2] = 'S';

    int cantidades[] = {config->maximo_bandidos, config->maximo_premios,
                        config->maximo_vidas_extra, config->maximo_oasis,
                        config->maximo_tormentas};
    char simbolos[] = {'B', 'P', 'V', 'O', 'T'};

    for (int tipo = 0; tipo < 5; tipo++) {
        int colocados = 0;
        while(colocados < cantidades[tipo]) {
            pos_aleatoria = rand() % config->cantidad_posiciones;
            // Solo colocamos si está vacío y no hay otro bandido
            if(mapa[pos_aleatoria] == '.' && mapa_bandidos[pos_aleatoria] == 0) {
                if (simbolos[tipo] == 'B') {
                    mapa_bandidos[pos_aleatoria] = 1; // Bandido al contador
                } else {
                    mapa[pos_aleatoria] = simbolos[tipo]; // Objeto al mapa
                }
                colocados++;
            }
        }
    }

    archivo = fopen("caravana.txt", "w");
    if (!archivo) {
        free(mapa); free(mapa_bandidos);
        return;
    }

    for (i = 0; i < config->cantidad_posiciones; i++) {
        tPosicion pos;
        pos.numero_posicion = i + 1;
        pos.elemento = mapa[i];
        pos.tiene_jugador = (i == 0) ? 1 : 0;
        pos.tiene_bandido = mapa_bandidos[i]; // <--- ¡AQUÍ ESTABA EL BUG DE MEMORIA!

        insertarAlFinal(ruta_desierto, &pos, sizeof(tPosicion));

        // Escribimos en caravana.txt contemplando el nuevo estado
        if (pos.tiene_jugador) {
            fprintf(archivo, "%02d:[%c J]\n", pos.numero_posicion, pos.elemento);
        } else if (pos.tiene_bandido > 0) {
            fprintf(archivo, "%02d:B\n", pos.numero_posicion);
        } else {
            if (pos.elemento == '.') fprintf(archivo, "%02d:.\n", pos.numero_posicion);
            else fprintf(archivo, "%02d:%c\n", pos.numero_posicion, pos.elemento);
        }
    }

    fclose(archivo);
    free(mapa);
    free(mapa_bandidos);
}
// --- LÓGICA DEL JUEGO ---
int tirarDado() {
    return (rand() % 6) + 1; // Genera un número del 1 al 6
}
// IA: Calcula la distancia mínima en un círculo para perseguir al jugador
char obtenerDireccionBandido(int posB, int posJ, int totalPos) {
    int distAdelante = (posJ > posB) ? (posJ - posB) : (totalPos - posB + posJ);
    int distAtras = (posB > posJ) ? (posB - posJ) : (totalPos - posJ + posB);
    return (distAdelante <= distAtras) ? 'F' : 'B';
}

// Devuelve al jugador al Campamento (I) tras ser emboscado
void enviarJugadorAlInicio(tListaDoble *ruta) {
    tNodo *act = *ruta;
    do {
        tPosicion *p = (tPosicion*)act->info;
        if (p->numero_posicion == 1) {
            p->tiene_jugador = 1;
            break;
        }
        act = act->siguiente;
    } while (act != *ruta);
}
// Fase 1: Muestra mapa/estado, pide decision al jugador y encola su movimiento.
// Devuelve 1 si el jugador movio, 0 si perdio el turno por tormenta.
int turnoJugador(tListaDoble *ruta, tCola *colaMovimientos, tCola *colaHistorial, int vidas, int puntos, int *turnos_perdidos)
{
    tMovimiento movActual;
    int posJugador = 1;

    mostrarMapa(ruta);

    // Encontrar posicion actual del jugador
    tNodo *act = *ruta;
    do {
        tPosicion *p = (tPosicion *)act->info;
        if (p->tiene_jugador) posJugador = p->numero_posicion;
        act = act->siguiente;
    } while (act != *ruta);

    printf("\n==================== ESTADO ====================\n");
    printf(" Posicion: %02d |  Vidas: %d |  Puntos: %d\n", posJugador, vidas, puntos);
    printf("================================================\n");

    printf("\n--- TU TURNO ---\n");
    if (*turnos_perdidos > 0) {
        printf("Estas atrapado en la Tormenta de Arena. Pierdes este turno.\n");
        printf("A la computadora (Bandidos) le corresponde jugar mientras tu no puedes!\n");
        (*turnos_perdidos)--;
        return 0; // El jugador no movio
    }

    movActual.entidad = 'J';
    movActual.pos_origen = posJugador;
    movActual.casillas = tirarDado();
    printf("Has sacado un %d en el dado.\n", movActual.casillas);

    char eleccion;
    do {
        printf("Deseas moverte hacia (F) Adelante o (B) Atras? : ");
        scanf(" %c", &eleccion);
        if (eleccion >= 'a' && eleccion <= 'z') eleccion -= 32;
    } while (eleccion != 'F' && eleccion != 'B');
    movActual.direccion = eleccion;

    poner_en_cola(colaMovimientos, &movActual, sizeof(tMovimiento));
    poner_en_cola(colaHistorial,   &movActual, sizeof(tMovimiento));
    return 1; // El jugador movio
}

// Auxiliar: devuelve la posicion actual del jugador en la lista
int obtenerPosJugador(tListaDoble *ruta) {
    tNodo *act = *ruta;
    do {
        tPosicion *p = (tPosicion *)act->info;
        if (p->tiene_jugador) return p->numero_posicion;
        act = act->siguiente;
    } while (act != *ruta);
    return 1;
}

// Fase 2: Encola los movimientos de todos los bandidos activos.
// Usa la posicion actualizada del jugador (puede haber cambiado tras la Fase 1).
void turnoBandidos(tConfig *config, tListaDoble *ruta, tCola *colaMovimientos)
{
    tMovimiento movActual;
    tNodo *act = *ruta;
    int hay_bandidos = 0;
    int posJugador = obtenerPosJugador(ruta); // Posicion real tras resolver Fase 1

    printf("\n--- TURNO DE LA COMPUTADORA (BANDIDOS) ---\n");
    do {
        tPosicion *p = (tPosicion *)act->info;
        for (int b = 0; b < p->tiene_bandido; b++) {
            hay_bandidos = 1;
            movActual.entidad   = 'B';
            movActual.pos_origen = p->numero_posicion;
            movActual.casillas  = tirarDado();
            movActual.direccion = obtenerDireccionBandido(p->numero_posicion, posJugador, config->cantidad_posiciones);

            printf("Bandido en casillero %02d saca un %d. Decide ir hacia %s.\n",
                   p->numero_posicion, movActual.casillas,
                   (movActual.direccion == 'F') ? "Adelante (F)" : "Atras (B)");

            poner_en_cola(colaMovimientos, &movActual, sizeof(tMovimiento));
        }
        act = act->siguiente;
    } while (act != *ruta);

    if (!hay_bandidos)
        printf("No hay bandidos en el mapa en este momento.\n");
    printf("\n");
}

int ejecutarMovimientos(tListaDoble *ruta, tCola *colaMovimientos, int *vidas, int *puntos, int *turnos_perdidos, int *protegido) {
    tMovimiento mov;

    while (sacar_de_cola(colaMovimientos, &mov, sizeof(tMovimiento))) {
        tNodo *origen = *ruta;
        tPosicion *posOrigen = NULL;

        // 1. Buscar entidad basada en pos_origen
        do {
            tPosicion *p = (tPosicion *)origen->info;
            if ((mov.entidad == 'J' && p->tiene_jugador) ||
                (mov.entidad == 'B' && p->numero_posicion == mov.pos_origen && p->tiene_bandido > 0)) {
                posOrigen = p;
                break;
            }
            origen = origen->siguiente;
        } while (origen != *ruta);

        if (posOrigen != NULL) {
            tNodo *destino = origen;
            char dirActual = mov.direccion;

            // 2. Desplazamiento y Rebote
            for (int i = 0; i < mov.casillas; i++) {
                if (dirActual == 'F') destino = destino->siguiente;
                else destino = destino->anterior;

                tPosicion *pDest = (tPosicion *)destino->info;

                if (mov.entidad == 'J' && pDest->elemento == 'S' && i < mov.casillas - 1) {
                    printf("Pasaste la Ciudad Refugio (Casillero %d). ¡Rebotando!\n", pDest->numero_posicion);
                    dirActual = (dirActual == 'F') ? 'B' : 'F';
                }
            }

            tPosicion *posDestino = (tPosicion *)destino->info;

            // 3. Resolución de Colisiones
            if (mov.entidad == 'J') {
                posOrigen->tiene_jugador = 0;
                posDestino->tiene_jugador = 1;

                if (posDestino->tiene_bandido > 0) {
                    if (*protegido) {
                        printf("Pisaste a un bandido, pero el Oasis te protegio!\n");
                    } else {
                        (*vidas)--;
                        printf("CAISTE EN UNA EMBOSCADA! Pierdes una vida.\n");
                        posDestino->tiene_jugador = 0;
                        posDestino->tiene_bandido--; // Eliminamos al bandido
                        enviarJugadorAlInicio(ruta);
                        printf("Has huido de regreso al Campamento Inicial.\n");
                        continue; // Termina la resolución de este jugador
                    }
                }

                if (posDestino->elemento == 'P') {
                    (*puntos)++; posDestino->elemento = '.';
                    printf("Atrapaste un premio! Puntos: %d\n", *puntos);
                }
                else if (posDestino->elemento == 'V') {
                    (*vidas)++; posDestino->elemento = '.';
                    printf("Vida extra! Vidas: %d\n", *vidas);
                }
                else if (posDestino->elemento == 'S') {
                    printf("LLEGASTE A LA CIUDAD REFUGIO!\n");
                    return 0; // Victoria
                }
                else if (posDestino->elemento == 'T') {
                    if (*protegido) printf("Tormenta evadida gracias al Oasis!\n");
                    else { printf("Caiste en una Tormenta! Pierdes el proximo turno.\n"); *turnos_perdidos = 1; }
                }
                else if (posDestino->elemento == 'O') {
                    printf("Oasis! Eres inmune a Tormentas y Bandidos el proximo turno.\n");
                    *protegido = 1;
                }
            }
            else if (mov.entidad == 'B') {
                posOrigen->tiene_bandido--; // Sale de su casilla
                posDestino->tiene_bandido++; // Entra a la nueva

                if (posDestino->tiene_jugador) {
                    if (*protegido) {
                        printf("Bandido bloqueado por el Oasis!\n");
                    } else {
                        (*vidas)--;
                        printf("UN BANDIDO TE ALCANZO! Pierdes una vida.\n");
                        posDestino->tiene_jugador = 0;
                        posDestino->tiene_bandido--; // Se elimina al chocar
                        enviarJugadorAlInicio(ruta);
                        printf("Has huido de regreso al Campamento Inicial.\n");
                    }
                }
            }
        }
    }

    // Si tenías protección y no caíste en tormenta, se desactiva al final del turno
    if (*protegido == 1 && *turnos_perdidos == 0) *protegido = 0;

    return 1;
}

// --- VISUALIZADOR DEL TABLERO ---
void mostrarMapa(tListaDoble *ruta) {
    if (*ruta == NULL) return;

    printf("\n============================= MAPA DEL DESIERTO =============================\n");
    tNodo *act = *ruta;
    do {
        tPosicion *p = (tPosicion *)act->info;

        // Prioridad de renderizado: 1. Jugador, 2. Bandidos, 3. Terreno
        if (p->tiene_jugador) {
            printf("[%c J] ", p->elemento);
        } else if (p->tiene_bandido > 0) {
            printf("B ");
        } else if (p->elemento == '.') {
            printf(". ");
        } else {
            printf("%c ", p->elemento);
        }

        act = act->siguiente;
    } while (act != *ruta);
    printf("\n=============================================================================\n");
}
// --- REGISTRO DE MOVIMIENTOS ---
// Recorre la cola sin desencolar: itera nodo a nodo desde frente hasta fondo
void mostrarHistorial(tCola *historial)
{
    printf("\n========== HISTORIAL DE MOVIMIENTOS ==========\n");

    if (historial->pri == NULL) {
        printf("  (No se registro ningun movimiento)\n");
        printf("==============================================\n");
        return;
    }

    tNodoCola *actual = historial->pri;
    int turno = 1;
    while (actual != NULL) {
        tMovimiento *m = (tMovimiento*)actual->info;
        printf("  Turno %2d: %c%d\n", turno, m->direccion, m->casillas);
        actual = actual->sig;
        turno++;
    }
    printf("==============================================\n");
    printf("  Total de turnos jugados: %d\n", turno - 1);
    printf("==============================================\n");
}
// --- BUCLE PRINCIPAL DEL JUEGO ---
void iniciarPartida(tConfig *config, tListaDoble *ruta) {
    int vidas = config->vidas_inicio;
    int puntos = 0;
    int jugando = 1;
    int turnos_perdidos = 0;
    int protegido = 0;
    tCola colaMovimientos;
    tCola colaHistorial;

    crear_cola(&colaMovimientos);
    crear_cola(&colaHistorial);

    printf("\n>>> INICIANDO TRAVESIA HACIA LA CIUDAD REFUGIO <<<\n");

    while (jugando == 1 && vidas > 0) {

        // === FASE 1: TURNO DEL JUGADOR ===
        int jugadorMovio = turnoJugador(ruta, &colaMovimientos, &colaHistorial, vidas, puntos, &turnos_perdidos);

        if (jugadorMovio) {
            printf("\n--- Resolviendo movimiento del jugador ---\n");
            jugando = ejecutarMovimientos(ruta, &colaMovimientos, &vidas, &puntos, &turnos_perdidos, &protegido);
            if (jugando == 0) break; // Victoria: no hay turno de bandidos
        }

        if (vidas <= 0) break; // Derrota por emboscada en Fase 1

        // === FASE 2: TURNO DE LOS BANDIDOS ===
        // obtenerPosJugador() se llama dentro de turnoBandidos para usar
        // la posicion actualizada (puede haber vuelto al inicio tras emboscada)
        turnoBandidos(config, ruta, &colaMovimientos);

        printf("--- Resolviendo movimiento de los bandidos ---\n");
        jugando = ejecutarMovimientos(ruta, &colaMovimientos, &vidas, &puntos, &turnos_perdidos, &protegido);

        // La proteccion del Oasis se consume al final del turno completo
        if (protegido && turnos_perdidos == 0) protegido = 0;
    }

    if (jugando == 0) {
        printf("\n¡VICTORIA! Has llegado a la Ciudad Refugio con %d punto(s).\n", puntos);
    } else {
        printf("\nHas perdido todas tus vidas. El desierto te ha consumido.\n");
    }

    mostrarHistorial(&colaHistorial);
    vaciar_cola(&colaHistorial);
    vaciar_cola(&colaMovimientos);
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
