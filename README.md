# 🏜️ Caravana del Desierto
**Trabajo Práctico — Algoritmos y Estructuras de Datos 2026**
Universidad Nacional de La Matanza — Comisiones 02-3300 y 03-3600

---

## Descripción

Juego en C donde una caravana (el jugador) recorre una ruta desértica desde el
Campamento Inicial (`I`) hasta la Ciudad Refugio (`S`), evitando bandidos y
aprovechando los recursos (premios, vidas extra, oasis) mientras esquiva las
tormentas. La ruta se modela como una **lista circular doblemente enlazada** y
los jugadores se administran con una capa de datos indexada por un **árbol de
búsqueda binaria**.

---

## Estructura del proyecto

Build con Code::Blocks. Los TDA están organizados en carpetas dentro de
`EstructurasDatos/`.

```
TP_Caravana/
├── main.c / main.h              <- entrada, inicialización y bucle del menú
├── funciones.c / funciones.h    <- config, dado, índice, errores, alta de jugadores, ranking (decl.)
├── juego.c / juego.h            <- motor del juego (escenario, turnos, efectos, partida)
├── ranking.c                    <- cálculo y muestra del ranking
├── lotes_de_prueba.c / .h       <- carga y persistencia genérica de archivos
├── config.h                     <- structs, constantes y defines compartidos
├── config.txt                   <- parámetros de configuración del juego
│
├── EstructurasDatos/
│   ├── TDA_Cola/               cola_dinamica.c / .h   <- TDA Cola dinámica (genérica)
│   ├── TDA_Lista/              Lista_funciones.c / Lista_Header.h  <- TDA Lista simple (índice/ranking)
│   ├── TDA_ListaDobleCircular/ ListaDoble.c / ListaDoble.h  <- TDA Lista circular doble (la ruta)
│   └── TDA_Arbol/              Arbol_Funciones.c / Arbol_Header.h  <- TDA Árbol (ABB, índice)
│
├── jugadores.dat                <- archivo binario de jugadores
├── partidas.dat                 <- archivo binario de partidas
├── indiceJugadores.idx          <- índice ABB persistido (binario)
└── Caravana_del_Desierto.cbp    <- proyecto Code::Blocks
```

> Las carpetas `bin/` y `obj/` se generan al compilar y **no** se entregan.

---

## Configuración (`config.txt`)

Parámetros del tablero, uno por línea con formato `clave=valor`:

| Parámetro              | Significado                              |
|------------------------|------------------------------------------|
| `cantidad_posiciones`  | Cantidad de casilleros de la ruta        |
| `vidas_inicio`         | Vidas iniciales del jugador              |
| `maximo_bandidos`      | Bandidos a distribuir                    |
| `maximo_premios`       | Premios a distribuir                     |
| `maximo_vidas_extra`   | Vidas extra a distribuir                 |
| `maximo_oasis`         | Oasis a distribuir                       |
| `maximo_tormentas`     | Tormentas a distribuir                   |

---

## Cómo compilar y ejecutar

**Desde Code::Blocks:** abrir `Caravana_del_Desierto.cbp` y `Build > Build and Run` (F9).

**Desde consola (MinGW), parado en la raíz del proyecto:**
```bash
gcc -Wall -o caravana main.c funciones.c juego.c lotes_de_prueba.c ranking.c \
    EstructurasDatos/TDA_Cola/cola_dinamica.c \
    EstructurasDatos/TDA_Lista/Lista_funciones.c \
    EstructurasDatos/TDA_ListaDobleCircular/ListaDoble.c \
    EstructurasDatos/TDA_Arbol/Arbol_Funciones.c
```

> Requisito de la cátedra: debe compilar con **0 errores y 0 warnings**.

---

## Estado de implementación

| Componente | Estado |
|---|---|
| Lectura de `config.txt` | ✅ Hecho |
| TDA Cola dinámica (genérica) | ✅ Hecho |
| TDA Lista simple (genérica, para índice/ranking) | ✅ Hecho |
| TDA Lista circular doblemente enlazada (la ruta) | ✅ Hecho |
| TDA Árbol de búsqueda binaria (índice) | ✅ Hecho |
| Generación del tablero (`caravana.txt`) | ✅ Hecho |
| Lógica de la partida (movimientos, efectos, vidas, rebote) | ✅ Hecho |
| IA de bandidos | ✅ Hecho |
| Encolado de movimientos y registro `FX`/`BX` | ✅ Hecho |
| Ranking de jugadores | ✅ Hecho |
| Menú principal + alta de jugador | ✅ Hecho |
| Encapsulamiento del TDA lista (el juego no accede a los nodos) | ✅ Hecho |
| Capa de datos: jugadores/partidas + índice ABB persistido | ✅ Hecho |

> **Pendiente para la entrega:** al iniciar, el programa todavía corre un lote de
> prueba que **sobrescribe** `jugadores.dat` / `partidas.dat`, y **regenera** el
> índice en vez de **cargar el persistido**. Para cumplir la consigna (ítem g) hay
> que desactivar el lote de prueba y cargar el índice guardado al iniciar.

---

## Decisiones de diseño

> Decisiones tomadas por el equipo que la consigna deja a criterio del grupo
> (ítem j). Cada una debe quedar clara, uniforme y justificada.

### Ranking

- **Criterio de orden:** los jugadores se ordenan por **puntos totales acumulados
  a lo largo de todas sus partidas** (de mayor a menor), tal como pide la consigna
  (ítem h). Los puntos viven en cada partida, así que el total se obtiene sumando
  todas las partidas de cada jugador.
- **Criterio de desempate:** a igualdad de puntos, va primero el jugador con
  **menos movimientos en total** (premia la eficiencia, y el dato ya está
  registrado en cada partida).
- **Implementación:** módulo `ranking.c` (su única función pública,
  `mostrarRanking`, se declara en `funciones.h`). Lee `partidas.dat` y reutiliza
  el TDA Lista genérico: inserta ordenado por nombre acumulando los duplicados
  (suma puntos y movimientos) y luego reordena por puntos para mostrar la tabla,
  que incluye una columna con los movimientos totales.

### Capa de datos e índice ABB

- El archivo de jugadores se accede a través de un **índice implementado sobre un
  árbol de búsqueda binaria** (consigna ítem g).
- El índice se **persiste en `indiceJugadores.idx`** (binario) y se reconstruye
  **balanceado** a partir del archivo, de modo que las búsquedas se mantengan
  eficientes.

### Ruta, movimiento y encapsulamiento

- **La ruta** es una **lista circular doblemente enlazada** (`TDA_ListaDobleCircular`).
  Cada nodo es una posición (`tPosicion`). El Inicio (`I`) es la posición 1 y la
  Salida (`S`) la última.
- **Movimiento del jugador:** se resuelve **por número de posición** (no caminando
  nodos). Si al avanzar se pasa de la Salida, **rebota**: los lugares sobrantes los
  hace para atrás, de modo que el movimiento total sea exactamente el dado.
- **Movimiento de los bandidos:** **circular, sin rebote** (pueden cruzar Inicio y
  Salida), calculado con módulo sobre la cantidad de posiciones.
- **Encapsulamiento del TDA:** el juego (`juego.c`) **no accede nunca a los nodos**
  (`->siguiente`/`->anterior`/`->info`). Toda la navegación pasa por primitivas de
  la lista: `buscarPtrElementoListaDC`, `desplazarYAplicar` y
  `recorrerListaDC_Condicionada`. Los **efectos** del juego (premio, vida, oasis,
  tormenta, bandido) se aplican en `juego.c`; la lista solo navega.
- **Registro de movimientos:** se encolan los movimientos (jugador y bandidos) y al
  finalizar la partida se muestra el historial en formato `FX` / `BX`.

### Pendientes de documentar

> A completar/decidir por el equipo antes de la entrega:
>
> - **Arranque (capa de datos):** desactivar el lote de prueba y cargar el índice
>   ABB persistido al iniciar (hoy se sobrescriben los archivos y se regenera el índice).
> - Limpieza de código viejo comentado y del volcado de depuración al salir.

---

## Fechas importantes

- **Entrega:** 17/06/2026 a las 23:59
- **Defensa:** 24/06/2026 y 08/07/2026

---

## Trabajo colaborativo

Cada integrante trabaja en su propia rama y hace Pull Request a `main`:
```
git checkout -b nombre/modulo
```
Antes de cada PR conviene traer `main` a la rama propia (`git merge main`) para
resolver conflictos ahí y que la integración entre limpia. El desarrollo
colaborativo se refleja en los commits de cada integrante.
