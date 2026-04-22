# 🏜️ Caravana del Desierto
**Trabajo Práctico — Algoritmos y Estructuras de Datos 2026**
Universidad Nacional de La Matanza - Prohibido Rendirse

---

## Estructura del proyecto

```
Caravana_del_Desierto/
├── Sources/
│   ├── main.c          <- punto de entrada, inicialización y menú
│   ├── funciones.c     <- desarrollo de funciones generales 
│   └── cola.c          <- desarrollo de primitivas TDA Cola
├── Headers/
│   ├── config.h        <- structs, constantes y defines compartidos
│   ├── funciones.h     <- prototipos de funciones generales
│   └── colan.h         <- prototipos del TDA Cola
└── config.txt          <- parámetros de configuración del juego
```

---


## Fechas importantes

- **Entrega:** 17/06/2026 a las 23:59
- **Defensa:** 24/06/2026 y 08/07/2026

---

## Cómo compilar

Desde Code::Blocks: `Build > Build and Run` (F9)

Desde consola:
```bash
gcc -Wall -Wextra -o caravana Sources/main.c Sources/funciones.c Sources/pila_din.c -I Headers
```

## Ramas de trabajo
Cada integrante trabaja en su propia rama y hace PR a `main`:
```
git checkout -b nombre/modulo
```
