#include "juego.h"

/* leerConfig vive en funciones.c (se quito la version duplicada que estaba aca) */

/*void generarEscenario(tConfig* config, tListaDoble* ruta_desierto)
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
    if(!mapa)
        return SIN_MEM;
    mapa_bandidos = (int*)calloc(config->cantidad_posiciones, sizeof(int));
    if(!mapa_bandidos)
    {
        free(mapa);
        return SIN_MEM;
    }

    for(i = 0; i < config->cantidad_posiciones; i++) {
        *(mapa+i) = '.';
    }

    *mapa = 'I';
    mapa[config->cantidad_posiciones - 1] = 'S';


    for (tipo = 0; tipo < 5; tipo++)
    {
        colocados = 0;
        intentos = 0;
        maxIntentos = config->cantidad_posiciones * 10;
        while(colocados < cantidades[tipo] && intentos < maxIntentos) {
            pos_aleatoria = rand() % config->cantidad_posiciones;
            if(mapa[pos_aleatoria] == '.' && mapa_bandidos[pos_aleatoria] == 0) {
                if (simbolos[tipo] == 'B')
                 {
                    mapa_bandidos[pos_aleatoria] = 1;
                } else
                 {
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

    pos.tiene_jugador = (i == 0) ? 1 : 0;
    for (i = 0; i < config->cantidad_posiciones; i++) {
        pos.numero_posicion = i + 1;
        pos.elemento = mapa[i];
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
}*/

/*busco en la lista, si lo encuentra me retorna el puntero*/
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

int cmpPosicionPorNumero(const void *a, const void *b) /*Me compara la posicion con el numero aleatorio que buscamos*/
{
    const tPosicion *pos = (const tPosicion *)a;
    const int *nro = (const int *)b;

    return pos->numero_posicion - *nro;
}

/*FUNCION PARA INICIALIZAR LA RUTA DEL DESIRETO*/
int inicializarRutaDesierto(tListaDoble *ruta_desierto, int cantidad_posiciones)
{
    int i;
    tPosicion pos;

    crearLista(ruta_desierto); /*creo la lista DC, que va a ser nuestro mapa*/

    for(i = 1; i <= cantidad_posiciones; i++)
    {
        pos.numero_posicion = i;
        pos.elemento = '.';
        pos.tiene_bandido = 0;
        pos.tiene_jugador = 0;

        if(i == 1)
        {
            pos.elemento = 'I';
            pos.tiene_jugador = 1;
        }

        if(i == cantidad_posiciones)
            pos.elemento = 'S';

        if(insertarAlFinal(ruta_desierto, &pos, sizeof(tPosicion)) != TODO_OK)
            return SIN_MEM;
    }

    return TODO_OK;
}


int colocarElementoEnRuta(tListaDoble *ruta_desierto, int numero_posicion, char simbolo)
{
    tPosicion *pos;

    pos = (tPosicion *)buscarPtrElementoListaDC(
        ruta_desierto,
        &numero_posicion,
        cmpPosicionPorNumero
    );

    if(pos == NULL)
        return NOT_FOUND_ELEM;

    if(pos->elemento != '.' || pos->tiene_bandido)
        return POS_OCUPADA;

    if(simbolo == 'B')
        pos->tiene_bandido = 1;
    else
        pos->elemento = simbolo;

    return TODO_OK;
}


int colocarElementosAleatorios(tListaDoble *ruta_desierto, int cantidad_posiciones, int cantidad, char simbolo)
{
    int colocados = 0;
    int intentos = 0;
    int maxIntentos = cantidad_posiciones * 10;
    int pos_aleatoria;
    int resultado;

    while(colocados < cantidad && intentos < maxIntentos)
    {
        pos_aleatoria = rand() % cantidad_posiciones + 1;

        resultado = colocarElementoEnRuta(ruta_desierto, pos_aleatoria, simbolo);

        if(resultado == TODO_OK)
            colocados++;

        intentos++;
    }

    return colocados;
}

FILE * abrirArchivo(const char *nombre, const char *modo)
{
    return fopen(nombre, modo);
}

int escribirRutaDesiertoEnArchivo(const char *nombre_archivo, const tListaDoble *ruta_desierto)
{
    FILE *archivo;
    tNodoDoble *act;
    tPosicion *pos;

    archivo = abrirArchivo(nombre_archivo, "wt");

    if(!archivo)
    {
        fclose(archivo);
        return ERR_ARCH;
    }

    act = *ruta_desierto;

    if(!act)
    {
        fclose(archivo);
        return LISTA_VACIA;
    }

    do
    {
        pos = (tPosicion *)act->info;

        if(pos->tiene_jugador)
            fprintf(archivo, "%02d:[%c J]\n", pos->numero_posicion, pos->elemento);
        else if(pos->tiene_bandido)
            fprintf(archivo, "%02d:B\n", pos->numero_posicion);
        else
            fprintf(archivo, "%02d:%c\n", pos->numero_posicion, pos->elemento);

        act = act->siguiente;

    }while(act != *ruta_desierto);

    fclose(archivo);

    return TODO_OK;
}

int generarEscenario(tConfiguracion *config, tListaDoble *ruta_desierto)
{
    int tipo;
    int colocados;

    int cantidades[] = {
        config->maximo_bandidos,
        config->maximo_premios,
        config->maximo_vidas_extra,
        config->maximo_oasis,
        config->maximo_tormentas
    };

    char simbolos[] = {'B', 'P', 'V', 'O', 'T'};

    if(inicializarRutaDesierto(ruta_desierto, config->cantidad_posiciones) != TODO_OK)
        return SIN_MEM;

    for(tipo = 0; tipo < 5; tipo++) /*el 5 ponerlo en un define*/
    {
        colocados = colocarElementosAleatorios(
            ruta_desierto,
            config->cantidad_posiciones,
            cantidades[tipo],
            simbolos[tipo]
        );

        if(colocados < cantidades[tipo])
        {
            printf(
                "Advertencia: solo se colocaron %d de %d [%c]. Sin espacio en el mapa.\n",
                colocados,
                cantidades[tipo],
                simbolos[tipo]
            );
        }
    }

    return escribirRutaDesiertoEnArchivo("caravana.txt", ruta_desierto);
}

int tirarDado()
{
    return (rand() % 6) + 1;
}

char obtenerDireccionBandido(int posB, int posJ, int totalPos)
{
    int distAdelante;
    int distAtras;
    distAdelante = (posJ >= posB) ? (posJ - posB) : (totalPos - posB + posJ);
    distAtras = (posB >= posJ) ? (posB - posJ) : (totalPos - posJ + posB);
    return (distAdelante <= distAtras) ? 'F' : 'B';
}

void enviarJugadorAlInicio(tListaDoble *ruta)
{
    recorrerListaDC_Condicionada(ruta, NULL, accionMandarInicio);
}
int accionMandarInicio(void *dato, void *ctx)
{
    tPosicion *p = (tPosicion *)dato;

    if (p->numero_posicion == 1) {
        p->tiene_jugador = 1;
        return 0; /* ¡Corte temprano! Ya lo pusimos en el inicio, que frene el bucle */
    }
    return 1;
}
int turnoJugador(tListaDoble *ruta, tCola *colaMovimientos, tCola *colaHistorial, tEstadoJugador *jugador)
{
    tMovimiento movActual;
    char opcion;
    int puede_retroceder;

/* 1. Llamamos a mostrarMapa pasándole el nuevo parámetro */
    mostrarMapa(ruta, jugador);

    /* 2. Usamos nuestra función encapsulada para encontrar al jugador sin usar tNodo! */
    jugador->posicion_actual = obtenerPosJugador(ruta);
    
    printf("\n--- TU TURNO ---\n");
    if (jugador->turnos_perdidos > 0) {
        printf("Estas atrapado en la Tormenta de Arena. Pierdes este turno.\n");
        printf("A la computadora (Bandidos) le corresponde jugar mientras tu no puedes!\n");
        jugador->turnos_perdidos--;
        return 0;
    }


    do {
        printf("Tirar los dados (T) o Salir del juego (S): ");
        scanf("%c",&opcion);
        limpiarBuffer();
    } while (opcion != 'T' && opcion != 'S');

    if (opcion == 'S') {
        printf("Abandonaste la partida. Hasta la proxima, caravanero!\n");
        return FIN_PARTIDA; /* el jugador abandono la partida */
    }

    movActual.entidad = 'J';
    movActual.pos_origen = jugador->posicion_actual;
    movActual.casillas = tirarDado();
    printf("Has sacado un %d en el dado.\n", movActual.casillas);
    /*/ --- RESTRICCION DEL JUGADOR SOBRE EL CAMPAMENTO INICIAL ---*/
    puede_retroceder = (jugador->posicion_actual - movActual.casillas >= 1);

    do {
        printf("Deseas moverte hacia (F) Adelante o (B) Atras?: ");
        scanf("%c",&opcion);
        limpiarBuffer();
        if( opcion =='B' && !puede_retroceder)
        {
            printf("No puedes retroceder porque cruzarias el Campamento Inicial (I). Solo podes avanzar.\n");
            puts("Volve a ingresar...");
        }
        if((opcion != 'F' && opcion != 'B'))
            puts("Opcion invalida. Por favor, ingresa F o B.");
    }while( (opcion != 'F' && opcion != 'B') || (opcion == 'B' && !puede_retroceder) );

    movActual.direccion = opcion;

    poner_en_cola(colaMovimientos, &movActual, sizeof(tMovimiento));
    poner_en_cola(colaHistorial,   &movActual, sizeof(tMovimiento));
    return TODO_OK;
}

int obtenerPosJugador(tListaDoble *ruta)
{
    tPosicion p;
    p.numero_posicion = 1;
    recorrerListaDC_Condicionada(ruta, &p, accionBuscarJugador);

    return p.numero_posicion;
}
int accionBuscarJugador(void *dato, void *ctx)
{
    tPosicion *nodoActual = (tPosicion *)dato;
    tPosicion *jugadorEncontrado = (tPosicion *)ctx;

    if (nodoActual->tiene_jugador) {
        *jugadorEncontrado = *nodoActual;
        return 0; /* Se encontro*/
    }

    return 1; /* No está acá, le decimos a la primitiva que siga al próximo nodo */
}
void turnoBandidos(tConfiguracion *config, tListaDoble *ruta, tCola *colaMovimientos, tEstadoJugador *jugador)
{
    tContextoBandidos contexto;
    contexto.colaMovimientos = colaMovimientos;
    contexto.posJugador = jugador->posicion_actual;
    contexto.hay_bandidos = 0;
    contexto.totalPosiciones = config->cantidad_posiciones;

    printf("\n--- TURNO DE LA COMPUTADORA (BANDIDOS) ---\n");
    recorrerListaDC_Condicionada(ruta, &contexto, accionEncolarBandidos);

    if (!contexto.hay_bandidos) {
        printf("No hay bandidos en el mapa en este momento.\n");
    }
    printf("\n");
}

int accionEncolarBandidos(void *ruta, void *ctx) 
{
    tPosicion *p = (tPosicion *)ruta;/*trae el dato de la ruta*/
    tContextoBandidos *contexto = (tContextoBandidos *)ctx; // Desempaquetamos el contexto
    tMovimiento movActual;
    int b;

    /*/ Si hay bandidos en esta casilla, procesamos cada uno**/
    for (b = 0; b < p->tiene_bandido; b++) {
        contexto->hay_bandidos = 1; /*/ Avisamos que encontramos al menos uno*/
        movActual.entidad   = 'B';
        movActual.pos_origen = p->numero_posicion;
        movActual.casillas  = tirarDado();
        
        /*/ Calculamos la dirección usando los datos que vinieron en la estructura de contexto*/
        movActual.direccion = obtenerDireccionBandido(p->numero_posicion, contexto->posJugador, contexto->totalPosiciones);

        printf("Bandido en casillero %02d saca un %d. Decide ir hacia %s.\n",
               p->numero_posicion, movActual.casillas,
               (movActual.direccion == 'F') ? "Adelante (F)" : "Atras (B)");

        /*/ Encolamos el movimiento*/
        poner_en_cola(contexto->colaMovimientos, &movActual, sizeof(tMovimiento));
    }
    
    return TODO_OK; /* Le decimos a la lista que siga recorriendo hasta dar toda la vuelta*/
}


int ejecutarMovimientos(tListaDoble *ruta, tCola *colaMovimientos, tEstadoJugador *jugador, int totalPosiciones)
{
    tMovimiento mov;
    int encontrado = 0;
    tNodoDoble *origen;
    tNodoDoble *destino;
    tPosicion *posOrigen;
    tPosicion *p;
    tPosicion *pDest;
    tPosicion *posDestino;
    char dirActual;
    int i;

    (void)totalPosiciones;

    while (sacar_de_cola(colaMovimientos, &mov, sizeof(tMovimiento)) == TODO_OK) {
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
                    if (jugador->protegido) {
                        printf("El bandido intento atraparte, pero la proteccion del oasis te salvo!\n");
                        jugador->protegido = 0; // <-- CORRECCI�N: El escudo se consume tras el primer golpe
                        // <-- NUEVO: Verificamos si hay un SEGUNDO bandido esperando en esa misma casilla
                        if (posDestino->tiene_bandido > 1)
                        {
                            (jugador->vidas)--;
                            printf("OTRO BANDIDO ESTABA AHI Y TE ALCANZO! Pierdes una vida.\n");
                            posDestino->tiene_jugador = 0;
                            posDestino->tiene_bandido--;
                            enviarJugadorAlInicio(ruta);
                            printf("Has huido de regreso al Campamento Inicial.\n");
                            continue;
                        }
                    }
                    else {
                        (jugador->vidas)--;
                        printf("CAISTE EN UNA EMBOSCADA! Pierdes una vida.\n");
                        posDestino->tiene_jugador = 0;
                        posDestino->tiene_bandido--;
                        enviarJugadorAlInicio(ruta);
                        printf("Has huido de regreso al Campamento Inicial.\n");
                        continue;
                    }
                }

                if (posDestino->elemento == 'P') {
                    (jugador->puntos)++;
                    posDestino->elemento = '.';
                    printf("Atrapaste un premio! Puntos: %d\n", jugador->puntos);
                }
                else if (posDestino->elemento == 'V') {
                    (jugador->vidas)++;
                    posDestino->elemento = '.';
                    printf("Vida extra! Vidas: %d\n", jugador->vidas);
                }
                else if (posDestino->elemento == 'S') {
                    printf("LLEGASTE A LA CIUDAD REFUGIO!\n");
                    return 0;
                }
                else if (posDestino->elemento == 'T') {
                    if (jugador->protegido) {
                        printf("Caiste en una tormenta, pero la proteccion del oasis te protegio!\n");
                    } else {
                        printf("Caiste en una Tormenta! Pierdes el proximo turno.\n");
                        jugador->turnos_perdidos = 1;
                    }
                }
                else if (posDestino->elemento == 'O') {
                    printf("Oasis! Eres inmune a Tormentas y Bandidos por un turno completo.\n");
                    jugador->protegido = 2;
                }
            }
            else if (mov.entidad == 'B') {
                posOrigen->tiene_bandido--;
                posDestino->tiene_bandido++;

                if (posDestino->tiene_jugador) {
                    if (jugador->protegido) {
                        printf("El bandido intento atraparte, pero la proteccion del oasis te salvo!\n");
                        jugador->protegido = 0; // <-- CORRECCION: El escudo se consume
                    } else {
                        (jugador->vidas)--;
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

void mostrarMapa(tListaDoble *ruta, tEstadoJugador *jugador)
{
    if (listaVacia(ruta))
        return;

    // 1. Mostramos los datos del jugador
    printf("\n==================== ESTADO DEL CARAVANERO ====================\n");
    printf(" Posicion: %02d |  Vidas: %d |  Puntos: %d | Escudo: %s\n",
           jugador->posicion_actual,
           jugador->vidas,
           jugador->puntos,
           jugador->protegido > 0 ? "ACTIVO (*)" : "Inactivo");
    printf("================================================================\n");

    // 2. Dibujamos el mapa pasando el puntero de la estructura como contexto genérico
    printf("\n============================= MAPA DEL DESIERTO =============================\n");
    recorrerListaDC_Condicionada(ruta, jugador, accionImprimirCasillero);
    printf("\n=============================================================================\n");
}

int accionImprimirCasillero(void *dato, void *ctx)
{
    tPosicion *p = (tPosicion *)dato;

    // Desempaquetamos el contexto pasándolo directamente a nuestro tipo de estructura
    tEstadoJugador *jugador = (tEstadoJugador *)ctx;

    if (p->tiene_jugador) {
        // Accedemos al escudo de forma limpia a través de la estructura
        if (jugador->protegido > 0) {
            printf("[%c J*] ", p->elemento);
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

    return 1; /* Retorna 1 siempre para que recorra e imprima TODO el mapa */
}

int mostrarHistorial(tCola *historial)
{
    int turno = 1;
    tMovimiento dato;
    printf("\n========== HISTORIAL DE MOVIMIENTOS ==========\n");
    if (cola_vacia(historial)) {
        printf("  (No se registro ningun movimiento)\n");
        printf("==============================================\n");
        return 0; /* Retorna 0 movimientos */
    }
    while (!cola_vacia(historial))
    {
        sacar_de_cola(historial, &dato, sizeof(dato));
        printf("  Turno %2d: %c%d\n", turno, dato.direccion, dato.casillas);
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
int iniciarPartida(tConfiguracion *config, tListaDoble *ruta, tPartida *partidaActual)
{
    int jugando = 1;
    int jugadorMovio;
    int puntos_finales = -1;

    tCola colaMovimientos;
    tCola colaHistorial;
    tEstadoJugador jugador;

    InicializarJugador(&jugador, config);

    crear_cola(&colaMovimientos);
    crear_cola(&colaHistorial);

    printf("\n>>> INICIANDO TRAVESIA HACIA LA CIUDAD REFUGIO <<<\n");

    while (jugando == 1 && jugador.vidas > 0) 
    {
        // === FASE 1: TURNO DEL JUGADOR ===
        jugadorMovio = turnoJugador(ruta, &colaMovimientos, &colaHistorial, &jugador);

        /* --- NUEVO: Capturar si el jugador decidio abandonar ---*/
        if (jugadorMovio == FIN_PARTIDA) {
            printf("\nHas decidido abandonar la caravana...\n");
            /* Si el jugador decide abandonar, se termina la partida y no se registra */
            vaciar_cola(&colaMovimientos);
            vaciar_cola(&colaHistorial);
            vaciarListaDobleC(ruta);
            return FIN_PARTIDA;
        }
        /* === LOGICA DE DESACTIVACION DEL OASIS */
        if (jugador.protegido > 0) 
            jugador.protegido--;

        // === FASE 2: TURNO DE LOS BANDIDOS ===
        turnoBandidos(config, ruta, &colaMovimientos, &jugador);

        if (!cola_vacia(&colaMovimientos))
        {
            printf("--- Resolviendo movimiento del turno ---\n");
            jugando = ejecutarMovimientos(ruta, &colaMovimientos, &jugador, config->cantidad_posiciones);
            /*mostrarMapa(ruta, protegido, config->cantidad_posiciones);*/
        }
            Pausar();
            LimpiarPantalla();
    
    }

    if (jugando == 0) {
        printf("\nVICTORIA! Has llegado a la Ciudad Refugio con %d punto(s).\n", jugador.puntos);
        puntos_finales = jugador.puntos;//Si jugo hasta el final se queda con los puntos que obtuvo
    } 
    else if (jugando == 1) {
        printf("\nHas perdido todas tus vidas. El desierto te ha consumido.\n");
        puntos_finales = jugador.puntos; //Si jugo hasta el final se queda con los puntos que obtuvo
    } 
    else if (jugando == -1) {
        printf("\nRegresando al menu principal. Partida abandonada.\n");
        puntos_finales = -1;
    }

    Pausar();
    LimpiarPantalla();

    /* --- CARGAMOS LA ESTRUCTURA --- */
    partidaActual->puntos = jugador.puntos; /* Asignamos los puntos conseguidos */
    partidaActual->cant_movimientos = mostrarHistorial(&colaHistorial); /* Extraemos movimientos */

    Pausar();
    LimpiarPantalla();
    vaciar_cola(&colaMovimientos);
    vaciarListaDobleC(ruta);
    return puntos_finales;
}

void InicializarJugador(tEstadoJugador *jugador, tConfiguracion *config)
{
    jugador->vidas = config->vidas_inicio;
    jugador->puntos = 0;
    jugador->turnos_perdidos=0;
    jugador->protegido = 0;
    jugador->posicion_actual = 0;
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
