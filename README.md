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
├── funciones.c / funciones.h    <- config, dado, partida, índice, errores, ranking (decl.)
├── ranking.c                    <- cálculo y muestra del ranking
├── lotes_de_prueba.c / .h       <- carga y persistencia genérica de archivos
├── config.h                     <- structs, constantes y defines compartidos
├── config.txt                   <- parámetros de configuración del juego
│
├── EstructurasDatos/
│   ├── TDA_Cola/   cola_dinamica.c / cola_dinamica.h   <- TDA Cola dinámica (genérica)
│   ├── TDA_Lista/  Lista_funciones.c / Lista_Header.h  <- TDA Lista (genérica)
│   └── TDA_Arbol/  Arbol_Funciones.c / Arbol_Header.h  <- TDA Árbol (ABB, índice)
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
gcc -Wall -o caravana main.c funciones.c lotes_de_prueba.c ranking.c \
    EstructurasDatos/TDA_Cola/cola_dinamica.c \
    EstructurasDatos/TDA_Lista/Lista_funciones.c \
    EstructurasDatos/TDA_Arbol/Arbol_Funciones.c
```

> Requisito de la cátedra: debe compilar con **0 errores y 0 warnings**.

---

## Estado de implementación

| Componente | Estado |
|---|---|
| Lectura de `config.txt` | ✅ Hecho |
| TDA Cola dinámica (genérica) | ✅ Hecho |
| TDA Lista (genérica) | ✅ Hecho |
| TDA Árbol de búsqueda binaria (índice) | ✅ Hecho |
| Capa de datos: jugadores/partidas + índice ABB persistido | ✅ Hecho |
| Ranking de jugadores | ✅ Hecho |
| Menú principal | ✅ Hecho |
| Ruta como lista circular doblemente enlazada | ⏳ Pendiente |
| Generación del tablero (`caravana.txt`) | ⏳ Pendiente |
| Lógica de la partida (movimientos, efectos, vidas) | ⏳ Pendiente |
| IA de bandidos | ⏳ Pendiente |
| Encolado de movimientos y registro `FX`/`BX` | ⏳ Pendiente |

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

### Pendientes de documentar

> A completar por el equipo a medida que se implementen:
>
> - **Ruta del desierto (lista circular doble)** y convención de casilleros
>   compuestos en `caravana.txt` (`[J]`, `[O J]`, etc.).
> - **Movimiento de bandidos (IA):** criterio de avance/retroceso.
> - **Registro de movimientos (`FX`/`BX`)** y uso de la cola para los turnos.

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
