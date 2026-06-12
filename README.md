# 🏜️ Caravana del Desierto
**Trabajo Práctico — Algoritmos y Estructuras de Datos 2026**
Universidad Nacional de La Matanza — Comisiones 02-3300 y 03-3600

---

## Descripción

Juego en C donde una caravana (el jugador) recorre una ruta desértica desde el
Campamento Inicial (`I`) hasta la Ciudad Refugio (`S`), evitando bandidos y
aprovechando los recursos (premios, vidas extra, oasis) mientras esquiva las
tormentas. La ruta se modela como una **lista circular doblemente enlazada**.

---

## Estructura del proyecto

Todos los archivos están en la raíz del proyecto (build con Code::Blocks).

```
dev-Dasha/
├── main.c / main.h                 <- punto de entrada e inicialización
├── funciones.c / funciones.h       <- menú, lectura de config, dado, partida
├── config.h                        <- structs, constantes y defines compartidos
├── config.txt                      <- parámetros de configuración del juego
│
├── TDA / estructuras genéricas
│   ├── cola_dinamica.c / .h        <- TDA Cola dinámica (genérica)
│   └── Lista_funciones.c / Lista_Header.h   <- TDA Lista enlazada (genérica)
│
├── Capa de datos
│   ├── lotes_de_prueba.c / .h      <- structs tJugador/tPartida y persistencia
│   ├── ranking.c / ranking.h       <- cálculo y muestra del ranking
│   ├── jugadores.dat               <- archivo binario de jugadores
│   └── partidas.dat                <- archivo binario de partidas
│
└── Caravana_del_Desierto.cbp       <- proyecto Code::Blocks
```

> Las carpetas `bin/` y `obj/` son generadas al compilar y **no** se entregan.

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

**Desde consola (MinGW):**
```bash
gcc -Wall -Wextra -o caravana main.c funciones.c cola_dinamica.c Lista_funciones.c lotes_de_prueba.c ranking.c
```

> Requisito de la cátedra: debe compilar con **0 errores y 0 warnings**.

---

## Decisiones de diseño

> En esta sección documentamos las decisiones que tomamos como equipo y que la
> consigna deja a criterio del grupo (ítem j). Cada decisión debe quedar clara,
> uniforme y justificada.

### Ranking

- **Criterio de orden:** los jugadores se ordenan por **puntos totales acumulados
  a lo largo de todas sus partidas** (de mayor a menor), tal como pide la consigna
  (ítem h). El total se calcula sumando los puntos de cada partida del jugador
  (los puntos viven en cada partida, no en el jugador).
- **Criterio de desempate:** si dos jugadores tienen los **mismos puntos**, queda
  primero el que hizo **menos movimientos en total** (más eficiente). Se eligió
  este criterio porque el dato de movimientos ya está registrado en cada partida
  y premia la eficiencia del jugador.
- **Implementación:** módulo `ranking.c` / `ranking.h`. Se reutiliza la lista
  genérica (`Lista_funciones.c`): se inserta ordenado por nombre con acumulación
  de duplicados (suma puntos y movimientos), y luego se reordena por puntos para
  mostrar la tabla.

### Otras decisiones

> _Pendientes de documentar por el equipo a medida que se implementen:_
>
> - **Ruta del desierto (lista circular doble):** _(a definir)_
> - **Generación del tablero / `caravana.txt`:** convención de casilleros
>   compuestos (`[J]`, `[O J]`, etc.) _(a definir)_
> - **Movimiento de bandidos (IA):** criterio de avance/retroceso _(a definir)_
> - **Registro de movimientos (FX/BX):** estructura usada _(a definir)_
> - **Capa de datos / índice ABB:** _(a definir)_

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
El desarrollo colaborativo se refleja a través de los commits de cada integrante.
