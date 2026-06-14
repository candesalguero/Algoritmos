#include "juego.h"

int leerConfig(tConfiguracion* config, const char *arch)
{
    char linea[150];
    char clave[50];
    int valor;

    FILE *pf=fopen(arch, "rt");

    if(!pf)
        return ERR_ARCH;


    config->cantidad_posiciones = 0;
    config->vidas_inicio = 0;
    config->maximo_bandidos = 0;
    config->maximo_premios = 0;
    config->maximo_vidas_extra = 0;
    config->maximo_oasis = 0;
    config->maximo_tormentas = 0;

    while (fgets(linea, sizeof(linea), archivo)) {
        if (sscanf(linea, " %[^:]:%d", clave, &valor) == 2) {
            if (strcmp(clave, "cantidad_posiciones") == 0) config->cantidad_posiciones = valor;
            else if (strcmp(clave, "vidas_inicio") == 0) config->vidas_inicio = valor;
            else if (strcmp(clave, "maximo_bandidos") == 0) config->maximo_bandidos = valor;
            else if (strcmp(clave, "maximo_premios") == 0) config->maximo_premios = valor;
            else if (strcmp(clave, "maximo_vidas_extra") == 0) config->maximo_vidas_extra = valor;
            else if (strcmp(clave, "maximo_oasis") == 0) config->maximo_oasis = valor;
            else if (strcmp(clave, "maximo_tormentas") == 0) config->maximo_tormentas = valor;
        }
    }

    fclose(pf);


    if (config->cantidad_posiciones <= 0 || config->vidas_inicio <= 0) {
        printf("Error: Archivo de configuracion corrupto o incompleto.\n");
        return 0;
    }

    return TODO_OK;
}

void generarEscenario(tConfig* config, tListaDoble* ruta_desierto)
{
    int i, pos_aleatoria, tipo, colocados, intentos, maxIntentos;
    FILE *archivo;
    char* mapa;
    int* mapa_bandidos;
    int cantidades[] = {config->maximo_bandidos, config->maximo_premios,
                        config->maximo_vidas_extra, config->maximo_oasis,
                        config->maximo_tormentas};
    char simbolos[5] = {'B', 'P', 'V', 'O', 'T'};
    tPosicion pos;

    crearLista(ruta_desierto);

    mapa = (char*)malloc(config->cantidad_posiciones * sizeof(char));
    mapa_bandidos = (int*)calloc(config->cantidad_posiciones, sizeof(int));
    if (!mapa || !mapa_bandidos)
        return;

    for(i = 0; i < config->cantidad_posiciones; i++) {
        mapa[i] = '.';
    }

    mapa[0] = 'I';
    mapa[config->cantidad_posiciones - 1] = 'S'; //La 'S' ahora se ubica en la última posición del mapa


    for (tipo = 0; tipo < 5; tipo++)
    {
        colocados = 0;
        intentos = 0;
        maxIntentos = config->cantidad_posiciones * 10;
        while(colocados < cantidades[tipo] && intentos < maxIntentos) {
            pos_aleatoria = rand() % config->cantidad_posiciones;
            if(mapa[pos_aleatoria] == '.' && mapa_bandidos[pos_aleatoria] == 0) {
                if (simbolos[tipo] == 'B') {
                    mapa_bandidos[pos_aleatoria] = 1;
                } else {
                    mapa[pos_aleatoria] = simbolos[tipo];
                }
                colocados++;
            }
            intentos++;
        }
        if (colocados < cantidades[tipo]) {
            printf("Advertencia: solo se colocaron %d de %d [%c]. Sin espacio en el mapa.\n",
                   colocados, cantidades[tipo], simbolos[tipo]);
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
        pos.tiene_bandido = mapa_bandidos[i];

        insertarAlFinal(ruta_desierto, &pos, sizeof(tPosicion));

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

int tirarDado()
{
    return (rand() % 6) + 1;
}

char obtenerDireccionBandido(int posB, int posJ, int totalPos)
{
    int distAdelante = (posJ >= posB) ? (posJ - posB) : (totalPos - posB + posJ);
    int distAtras = (posB >= posJ) ? (posB - posJ) : (totalPos - posJ + posB);
    return (distAdelante <= distAtras) ? 'F' : 'B';
}

void enviarJugadorAlInicio(tListaDoble *ruta)
{
    tNodo *act = *ruta;
    int encontrado = 0;
    tPosicion *p;
    do {
        p = (tPosicion*)act->info;
        if (p->numero_posicion == 1) {
            p->tiene_jugador = 1;
            encontrado = 1;
        }
        if (!encontrado) {
            act = act->siguiente;
        }
    } while (act != *ruta && !encontrado);
}

int turnoJugador(tListaDoble *ruta, tCola *colaMovimientos, tCola *colaHistorial, int vidas, int puntos, int *turnos_perdidos, int protegido)
{
    tMovimiento movActual;
    int posJugador = 1;
    char accion = '\0';
    char entrada[20];
    tNodo *act = *ruta;
    tPosicion *p;
    int puede_retroceder;

    mostrarMapa(ruta, protegido);

    tNodo *act = *ruta;
    do {
        p = (tPosicion *)act->info;
        if (p->tiene_jugador) posJugador = p->numero_posicion;
        act = act->siguiente;
    } while (act != *ruta);

    printf("\n==================== ESTADO ====================\n");
    printf(" Posicion: %02d |  Vidas: %d |  Puntos: %d | Escudo: %s\n",
           posJugador, vidas, puntos, protegido > 0 ? "ACTIVO (*)" : "Inactivo");
    printf("================================================\n");

    printf("\n--- TU TURNO ---\n");
    if (*turnos_perdidos > 0) {
        printf("Estas atrapado en la Tormenta de Arena. Pierdes este turno.\n");
        printf("A la computadora (Bandidos) le corresponde jugar mientras tu no puedes!\n");
        (*turnos_perdidos)--;
        return 0;
    }
    // --- NUEVO: Dar la opción de tirar dados o salir ---

    do {
        printf("Tirar los dados (T) o Salir del juego (S): ");

        if (fgets(entrada, sizeof(entrada), stdin) != NULL) {

            // Verificamos si fgets logró capturar el Enter (\n)
            if (strchr(entrada, '\n') == NULL) {
                // Si NO hay \n en el string, el usuario escribió más de 99 letras.
                // Limpiamos la basura que sobró en el teclado.
                limpiarBuffer();
            } else {
                // Si capturó el \n, el buffer ya está limpio.
                // Solo le quitamos el salto de línea a nuestra cadena.
                entrada[strcspn(entrada, "\n")] = '\0';
            }

            // Validamos que sea exactamente 1 letra
            if (strlen(entrada) == 1)
            {
                accion = entrada[0];
                if (accion >= 'a' && accion <= 'z') {
                    accion -= 32;
                }
                if (accion != 'T' && accion != 'S') {
                printf("Opcion invalida. Ingrese T para tirar o S para salir.\n");
                accion = '\0';
                }
            }
            else {
                printf("Opcion invalida. Ingrese T para tirar o S para salir.\n");
                accion = '\0'; // Invalida el input si escribió una palabra
            }
        }
    } while (accion != 'T' && accion != 'S');

    if (accion == 'S') {
        printf("Abandonaste la partida. Hasta la proxima, caravanero!\n");
        return -1; // Retornamos -1 para indicar que el jugador abandonó la partida
    }
    movActual.entidad = 'J';
    movActual.pos_origen = posJugador;
    movActual.casillas = tirarDado();
    printf("Has sacado un %d en el dado.\n", movActual.casillas);
    // --- RESTRICCIÓN DEL JUGADOR SOBRE EL CAMPAMENTO INICIAL ---
    puede_retroceder = (posJugador - movActual.casillas >= 1);

    // --- NUEVO: Preguntar siempre y validar el retroceso ---
    accion = '\0'; // Inicializamos en un estado vacío/inválido de control

    do {

        printf("Deseas moverte hacia (F) Adelante o (B) Atras?: ");

        if (fgets(entrada, sizeof(entrada), stdin) != NULL) {

            if (strchr(entrada, '\n') == NULL)
                limpiarBuffer();
            else
                entrada[strcspn(entrada, "\n")] = '\0';

            if (strlen(entrada) == 1) {

                accion = entrada[0];

                if (accion >= 'a' && accion <= 'z')
                    accion -= 32;

                if (accion != 'F' && accion != 'B') {
                    printf("Opcion invalida. Ingresa F para avanzar o B para retroceder.\n");
                    accion = '\0';
                }

                if (accion == 'B') {

                    if (posJugador == 1) {

                        printf("Estas en el Campamento Inicial (I). Solo podes avanzar.\n");
                        accion = '\0';
                    }
                    else if (!puede_retroceder) {

                        printf("No puedes retroceder porque cruzarias el Campamento Inicial (I). Solo podes avanzar.\n");
                        accion = '\0';
                    }
                }
            }
            else {

                printf("Opcion invalida. Ingresa F para avanzar o B para retroceder.\n");
                accion = '\0';
            }
        }

    } while (accion != 'F' && accion != 'B');

    movActual.direccion = accion;

    poner_en_cola(colaMovimientos, &movActual, sizeof(tMovimiento));
    poner_en_cola(colaHistorial,   &movActual, sizeof(tMovimiento));
    return 1;
}

int obtenerPosJugador(tListaDoble *ruta)
{
    tNodo *act = *ruta;
    tPosicion *p;
    do {
        p = (tPosicion *)act->info;
        if (p->tiene_jugador) return p->numero_posicion;
        act = act->siguiente;
    } while (act != *ruta);
    return 1;
}

void turnoBandidos(tConfig *config, tListaDoble *ruta, tCola *colaMovimientos)
{
    tMovimiento movActual;
    tNodo *act = *ruta;
    int hay_bandidos = 0;
    tPosicion *p;
    int b;
    int posJugador = obtenerPosJugador(ruta);

    printf("\n--- TURNO DE LA COMPUTADORA (BANDIDOS) ---\n");
    do {
        p = (tPosicion *)act->info;
        for (b = 0; b < p->tiene_bandido; b++) {
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

int ejecutarMovimientos(tListaDoble *ruta, tCola *colaMovimientos, int *vidas, int *puntos, int *turnos_perdidos, int *protegido)
{
    tMovimiento mov;
    int encontrado = 0;
    tNodo *origen;
    tNodo *destino;
    tPosicion *posOrigen;
    tPosicion *p;
    tPosicion *pDest;
    tPosicion *posDestino;
    char dirActual;
    int i;

    while (sacar_de_cola(colaMovimientos, &mov, sizeof(tMovimiento))) {
        origen = *ruta;
        posOrigen = NULL;
        encontrado = 0;
        do {
            p = (tPosicion *)origen->info;
            if ((mov.entidad == 'J' && p->tiene_jugador) ||
                (mov.entidad == 'B' && p->numero_posicion == mov.pos_origen && p->tiene_bandido > 0)) {
                posOrigen = p;
                encontrado = 1;
            }
            if (!encontrado) {
                origen = origen->siguiente;
            }
        } while (origen != *ruta && !encontrado);

        if (posOrigen != NULL) {
            destino = origen;
            dirActual = mov.direccion;

            for (i = 0; i < mov.casillas; i++) {
                if (dirActual == 'F') destino = destino->siguiente;
                else destino = destino->anterior;

                pDest = (tPosicion *)destino->info;

                if (mov.entidad == 'J' && pDest->elemento == 'S' && i < mov.casillas - 1) {
                    printf("Pasaste la Ciudad Refugio (Casillero %d). Rebotando!\n", pDest->numero_posicion);
                    dirActual = (dirActual == 'F') ? 'B' : 'F';
                }
            }

            posDestino = (tPosicion *)destino->info;

            if (mov.entidad == 'J')
            {
                posOrigen->tiene_jugador = 0;
                posDestino->tiene_jugador = 1;

                if (posDestino->tiene_bandido > 0) {
                    if (*protegido) {
                        printf("El bandido intento atraparte, pero la proteccion del oasis te salvo!\n");
                        *protegido = 0; // <-- CORRECCIÓN: El escudo se consume tras el primer golpe
                        // <-- NUEVO: Verificamos si hay un SEGUNDO bandido esperando en esa misma casilla
                        if (posDestino->tiene_bandido > 1)
                        {
                            (*vidas)--;
                            printf("OTRO BANDIDO ESTABA AHI Y TE ALCANZO! Pierdes una vida.\n");
                            posDestino->tiene_jugador = 0;
                            posDestino->tiene_bandido--;
                            enviarJugadorAlInicio(ruta);
                            printf("Has huido de regreso al Campamento Inicial.\n");
                            continue;
                        }
                    }
                    else {
                        (*vidas)--;
                        printf("CAISTE EN UNA EMBOSCADA! Pierdes una vida.\n");
                        posDestino->tiene_jugador = 0;
                        posDestino->tiene_bandido--;
                        enviarJugadorAlInicio(ruta);
                        printf("Has huido de regreso al Campamento Inicial.\n");
                        continue;
                    }
                }

                if (posDestino->elemento == 'P') {
                    (*puntos)++;
                    posDestino->elemento = '.';
                    printf("Atrapaste un premio! Puntos: %d\n", *puntos);
                }
                else if (posDestino->elemento == 'V') {
                    (*vidas)++;
                    posDestino->elemento = '.';
                    printf("Vida extra! Vidas: %d\n", *vidas);
                }
                else if (posDestino->elemento == 'S') {
                    printf("LLEGASTE A LA CIUDAD REFUGIO!\n");
                    return 0;
                }
                else if (posDestino->elemento == 'T') {
                    if (*protegido) {
                        printf("Caiste en una tormenta, pero la proteccion del oasis te protegio!\n");
                    } else {
                        printf("Caiste en una Tormenta! Pierdes el proximo turno.\n");
                        *turnos_perdidos = 1;
                    }
                }
                else if (posDestino->elemento == 'O') {
                    printf("Oasis! Eres inmune a Tormentas y Bandidos por un turno completo.\n");
                    *protegido = 2;
                }
            }
            else if (mov.entidad == 'B') {
                posOrigen->tiene_bandido--;
                posDestino->tiene_bandido++;

                if (posDestino->tiene_jugador) {
                    if (*protegido) {
                        printf("El bandido intento atraparte, pero la proteccion del oasis te salvo!\n");
                        *protegido = 0; // <-- CORRECCIÓN: El escudo se consume
                    } else {
                        (*vidas)--;
                        printf("UN BANDIDO TE ALCANZO! Pierdes una vida.\n");
                        posDestino->tiene_jugador = 0;
                        posDestino->tiene_bandido--;
                        enviarJugadorAlInicio(ruta);
                        printf("Has huido de regreso al Campamento Inicial.\n");
                    }
                }
            }
        }
    }

    return 1;
}

void mostrarMapa(tListaDoble *ruta, int protegido)
{
    tNodo *act;
    tPosicion *p;
    if (*ruta == NULL) return;

    printf("\n============================= MAPA DEL DESIERTO =============================\n");
    act = *ruta;
    do {
        p = (tPosicion *)act->info;

        if (p->tiene_jugador) {
            if (protegido > 0) {
                printf("[%c J*] ", p->elemento); // Indicador visual de escudo activo
            } else {
                printf("[%c J] ", p->elemento);
            }
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

int mostrarHistorial(tCola *historial) /** MUESTRO Y DESENCOLAR*/
{
    tNodoCola *actual;
    int turno = 1;
    tMovimiento *m;

    printf("\n========== HISTORIAL DE MOVIMIENTOS ==========\n");
    /**ACA PONER SI LA COLA ESTA VACIA*/
    if (historial->pri == NULL) {
        printf("  (No se registro ningun movimiento)\n");
        printf("==============================================\n");
        return 0; /* Retorna 0 movimientos */
    }

    actual = historial->pri;
    while (actual != NULL)
    {
        m = (tMovimiento*)actual->info;
        printf("  Turno %2d: %c%d\n", turno, m->direccion, m->casillas);
        actual = actual->sig;
        turno++;
    }
    printf("==============================================\n");
    printf("  Total de turnos jugados: %d\n", turno - 1);
    printf("==============================================\n");
    /** USAR UN PUNTERO ACUMULADOR O QUE RETORNE LA CANTIDAD DE MOVIMIENTOS*/
    return turno - 1; /* <-- Retorna la cantidad de movimientos */
}
void Pausar()
{
    system("pause");
}

void LimpiarPantalla()
{
    system("cls");
}
int iniciarPartida(tConfig *config, tListaDoble *ruta, tPartida *partidaActual)
{
    int vidas = config->vidas_inicio;
    int puntos = 0;
    int jugando = 1;
    int turnos_perdidos = 0;
    int protegido = 0;

    tCola colaMovimientos;
    tCola colaHistorial;
    int jugadorMovio;
    int puntos_finales = -1;

    crear_cola(&colaMovimientos);
    crear_cola(&colaHistorial);

    printf("\n>>> INICIANDO TRAVESIA HACIA LA CIUDAD REFUGIO <<<\n");

    while (jugando == 1 && vidas > 0) {

        // === FASE 1: TURNO DEL JUGADOR ===
        jugadorMovio = turnoJugador(ruta, &colaMovimientos, &colaHistorial, vidas, puntos, &turnos_perdidos, protegido);

        /// --- NUEVO: Capturar si el jugador decidió abandonar ---
        if (jugadorMovio == -1) {
            printf("\nHas decidido abandonar la caravana...\n");
            jugando = -1; // Marcamos un estado especial de abandono

        }
        if (jugando == 1 && jugadorMovio == 1) {
            printf("\n--- Resolviendo movimiento del jugador ---\n");
            jugando = ejecutarMovimientos(ruta, &colaMovimientos, &vidas, &puntos, &turnos_perdidos, &protegido);
            Pausar();
            LimpiarPantalla();
        }
        // === LÓGICA DE DESACTIVACIÓN DEL OASIS Y FASE 2 ===
        // Solo se ejecuta si el jugador no abandonó, no ganó ni perdió todas sus vidas en la fase 1
        if (jugando == 1 && vidas > 0)
        {
            if (protegido == 1) {
                protegido = 0;
            } else if (protegido == 2) {
                protegido = 1;
            }

            // === FASE 2: TURNO DE LOS BANDIDOS ===
            turnoBandidos(config, ruta, &colaMovimientos);

            if (!cola_vacia(&colaMovimientos))
            {
                printf("--- Resolviendo movimiento de los bandidos ---\n");
                jugando = ejecutarMovimientos(ruta, &colaMovimientos, &vidas, &puntos, &turnos_perdidos, &protegido);
                mostrarMapa(ruta, protegido);
            }
                Pausar();
                LimpiarPantalla();
        }
    }

    if (jugando == 0) {
        printf("\nVICTORIA! Has llegado a la Ciudad Refugio con %d punto(s).\n", puntos);
        puntos_finales = puntos;//Si jugo hasta el final se queda con los puntos que obtuvo
    } else if (jugando == 1) {
        printf("\nHas perdido todas tus vidas. El desierto te ha consumido.\n");
        puntos_finales = puntos; //Si jugo hasta el final se queda con los puntos que obtuvo
    } else if (jugando == -1) {
        printf("\nRegresando al menu principal. Partida abandonada.\n");
        puntos_finales = -1;
    }

    Pausar();
    LimpiarPantalla();

    /* --- CARGAMOS LA ESTRUCTURA --- */
    partidaActual->puntos = puntos; /* Asignamos los puntos conseguidos */
    partidaActual->cant_movimientos = mostrarHistorial(&colaHistorial); /* Extraemos movimientos */

    Pausar();
    LimpiarPantalla();
    // --- LIBERACIÓN DE MEMORIA ---
    vaciar_cola(&colaHistorial); //ESTO LO PUEDO SACAR Y PONER EN MOSTRARHISTORIAL SACAR DE COLA Y YA ME QUEDA VACIA
    vaciar_cola(&colaMovimientos);
    vaciarLista(ruta);
    return puntos_finales;
}
void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
void mostrarMenu()
{
    printf("\n==============================\n");
    printf("   CARAVANA DEL DESIERTO\n");
    printf("==============================\n");
    printf("1. Nueva Partida\n");
    printf("2. Ver Ranking de Jugadores\n");
    printf("3. Salir\n");
    printf("------------------------------\n");
    printf("Seleccione una opcion: ");
}
