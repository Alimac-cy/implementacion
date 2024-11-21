#ifndef MMU_H
#define MMU_H

#include <vector>
#include <queue>
#include <map>
#include "Process.h"
#include "PageTable.h"
#include "PrimaryMemory.h"
#include "SecondaryMemory.h"
#include "Log.h"

class MMU
{
private:
    int tamanoMemoria;                   // Tamaño de la memoria física.
    int tamanoPagina;                    // Tamaño de cada página.
    MemoriaPrincipal memoriaPrincipal;   // Memoria principal manejada por la clase.
    MemoriaSecundaria memoriaSecundaria; // Almacenamiento secundario.
    std::map<int, int> cache;            // Memoria asociativa (mapa lógico -> físico).
    int tamanoCache;                     // Tamaño de la caché.
    std::queue<int> colaCache;           // Cola para manejar el reemplazo en la caché.
    int punteroReloj;                    // Puntero para el algoritmo de reloj.

    std::vector<Proceso *> procesosActuales; // Lista de procesos actuales.

    Log logSistema; // Registro de eventos y estadísticas.

    // Métodos auxiliares
    void actualizarCache(int direccionLogica, int direccionFisica);

public:
    // Constructor
    MMU(int tamanoPagina, int tamanoCache, const std::string &archivoAlmacenamiento);

    // Métodos de manejo de procesos
    void agregarProceso(Proceso *proceso);
    void eliminarProceso(int idProceso);
    Proceso *obtenerProceso(int idProceso);

    int traducirDireccion(int direccionLogica, int idProceso);
    void manejarFalloDePagina(int direccionLogica, Proceso *proceso, bool desdeSecundaria);
    void reemplazarPaginaEnMemoriaSecundaria(Proceso *proceso, int numeroPagina, EntradaPagina paginaActual);
    void reemplazarPaginaEnMemoriaPrincipal(int paginaReemplazo, Proceso *proceso);

    // Métodos para imprimir estados
    void imprimirEstadisticas() const;
    void imprimirEstadoMemorias() const;
};
#endif // MMU_H