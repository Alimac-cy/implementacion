# Sistema de Gestión de Memoria (MMU) en C++

## **Visión General**

Este proyecto implementa una Unidad de Gestión de Memoria (MMU) simulada en C++. La MMU gestiona la memoria principal y secundaria, permitiendo la asignación, traducción de direcciones, manejo de fallos de página y la modificación de instrucciones específicas dentro de marcos de memoria. El sistema está diseñado para manejar múltiples procesos, cada uno con su propia tabla de páginas, y asegura la consistencia de los datos al liberar procesos.

## **Características Principales**

- **Asignación Dinámica de Marcos:** La MMU asigna marcos de memoria a los procesos de manera dinámica, respetando un límite de marcos por proceso.
- **Traducción de Direcciones:** Traduce direcciones lógicas a físicas utilizando tablas de páginas.
- **Manejo de Fallos de Página:** Implementa un mecanismo para manejar fallos de página, incluyendo la carga de páginas desde la memoria secundaria y la política de reemplazo NRU (Not Recently Used).
- **Persistencia de Datos:** Al liberar un proceso, las páginas modificadas se escriben de vuelta al almacenamiento secundario (`almacenamiento.txt`).

## **Requisitos**

- **Compilador C++:** Se requiere un compilador compatible con C++11 o superior (por ejemplo, `g++`).

## **Estructura del Proyecto**

- `MMU.h` / `MMU.cpp`: Implementación de la clase MMU.
- `PrimaryMemory.h` / `PrimaryMemory.cpp`: Implementación de la clase de Memoria Principal.
- `SecondaryMemory.h` / `SecondaryMemory.cpp`: Implementación de la clase de Memoria Secundaria.
- `PageTable.h` / `PageTable.cpp`: Implementación de la clase Tabla de Páginas.
- `Proceso.h` / `Proceso.cpp`: Implementación de la clase Proceso.
- `main.cpp`: Archivo principal que ejecuta la simulación.
- `almacenamiento.txt`: Archivo que simula la memoria secundaria.


## **1. `MMU.h` / `MMU.cpp`: Implementación de la clase MMU**

**Descripción:**

La clase `MMU` (Memory Management Unit) es el núcleo del sistema de gestión de memoria. Su responsabilidad principal es gestionar la asignación de marcos de memoria a los procesos, traducir direcciones lógicas a físicas, manejar fallos de página y coordinar la interacción entre la memoria principal y secundaria.

**Funciones Principales:**

- **Traducción de Direcciones:** Convierte direcciones lógicas proporcionadas por los procesos en direcciones físicas en la memoria principal.
- **Manejo de Fallos de Página:** Detecta cuando una página no está cargada en la memoria principal y se encarga de cargarla desde la memoria secundaria.
- **Asignación y Liberación de Marcos:** Asigna marcos libres a los procesos y libera marcos cuando un proceso termina.
- **Modificación de Instrucciones:** Permite la modificación de instrucciones específicas dentro de un marco usando direcciones físicas.
- **Gestión de la Tabla de Páginas:** Interactúa con las tablas de páginas de cada proceso para mantener un seguimiento de qué páginas están cargadas y cuáles están modificadas.

**Interacciones:**

- **`PrimaryMemory`:** Para asignar y liberar marcos de memoria.
- **`SecondaryMemory`:** Para leer y escribir páginas desde y hacia el almacenamiento secundario.
- **`PageTable`:** Para gestionar las tablas de páginas de cada proceso.
- **`Proceso`:** Representa los procesos que utilizan la memoria gestionada por la MMU.

### **2. `PrimaryMemory.h` / `PrimaryMemory.cpp`: Implementación de la clase de Memoria Principal**

**Descripción:**

La clase `PrimaryMemory` simula la memoria principal del sistema. Está organizada en marcos (frames), cada uno de los cuales puede contener múltiples instrucciones.

**Funciones Principales:**

- **Asignación de Marcos:** Identifica y asigna marcos libres a los procesos que lo solicitan.
- **Liberación de Marcos:** Libera marcos cuando ya no son necesarios.
- **Actualización de Instrucciones:** Permite la actualización de instrucciones específicas dentro de un marco sin afectar las demás instrucciones.
- **Obtención de Instrucciones:** Proporciona acceso a instrucciones específicas basadas en direcciones físicas.

### **3. `SecondaryMemory.h` / `SecondaryMemory.cpp`: Implementación de la clase de Memoria Secundaria**

**Descripción:**

La clase `SecondaryMemory` simula la memoria secundaria del sistema, representada por un archivo (`almacenamiento.txt`). Almacena todas las instrucciones que no están actualmente cargadas en la memoria principal.

**Funciones Principales:**

- **Lectura de Páginas:** Lee páginas completas desde el archivo de almacenamiento secundario.
- **Escritura de Páginas:** Escribe páginas modificadas de vuelta al almacenamiento secundario.
- **Gestión de Índices Secundarios:** Mantiene un mapeo entre los procesos y sus respectivas páginas en la memoria secundaria.

### **4. `PageTable.h` / `PageTable.cpp`: Implementación de la clase Tabla de Páginas**

**Descripción:**

La clase `PageTable` representa la tabla de páginas de un proceso. Cada entrada en la tabla de páginas mantiene información sobre una página específica, incluyendo el marco donde está cargada, si está modificada, y otros atributos relevantes.

**Funciones Principales:**

- **Gestión de Entradas:** Agrega, elimina y actualiza entradas en la tabla de páginas.
- **Seguimiento de Estado:** Mantiene información sobre si una página está válida, referenciada o modificada.
- **Política de Reemplazo:** Implementa la política de reemplazo NRU para decidir qué página reemplazar cuando no hay marcos libres disponibles.

## **Compilación**

Para compilar el programa, utiliza el siguiente comando en la terminal. Asegúrate de estar en el directorio raíz del proyecto donde se encuentran los archivos fuente.

```bash
g++ -std=c++11 -o programa main.cpp MMU.cpp PrimaryMemory.cpp SecondaryMemory.cpp PageTable.cpp Process.cpp
```

## **Ejecución del Programa**

Después de compilar correctamente, ejecuta el programa utilizando el siguiente comando:

```bash
./programa
```

## **Corroboración de Tests**
