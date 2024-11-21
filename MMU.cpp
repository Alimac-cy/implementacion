#include "MMU.h"
#include <iostream>

// Constructor
MMU::MMU(int tamanoPagina, int tamanoCache, const std::string &archivoAlmacenamiento)
    : memoriaPrincipal(4 * 1024, 64 * 1024),                          // Memoria principal con 4 KB por marco, 64 KB total.
      memoriaSecundaria(archivoAlmacenamiento, 4 * 1024, 256 * 1024), // 4 KB por página, 256 KB total.
      tamanoPagina(tamanoPagina),
      tamanoCache(tamanoCache),
      punteroReloj(0)
{
}

// Agregar proceso
void MMU::agregarProceso(Proceso *proceso)
{
    procesosActuales.push_back(proceso);
    logSistema.registrarEvento("Proceso " + std::to_string(proceso->obtenerId()) + " agregado.");
}

// Eliminar proceso
void MMU::eliminarProceso(int idProceso)
{
    for (auto it = procesosActuales.begin(); it != procesosActuales.end(); ++it)
    {
        if ((*it)->obtenerId() == idProceso)
        {
            procesosActuales.erase(it);
            logSistema.registrarEvento("Proceso " + std::to_string(idProceso) + " eliminado.");
            break;
        }
    }
}

// Obtener proceso por ID
Proceso *MMU::obtenerProceso(int idProceso)
{
    for (auto &proceso : procesosActuales)
    {
        if (proceso->obtenerId() == idProceso)
        {
            return proceso;
        }
    }
    return nullptr;
}

// Traducción de dirección lógica a dirección física
int MMU::traducirDireccion(int direccionLogica, int idProceso)
{
    Proceso *proceso = obtenerProceso(idProceso);
    if (!proceso)
    {
        logSistema.registrarEvento("Error: Proceso no encontrado para ID: " + std::to_string(idProceso));
        return -1;
    }

    int numeroPagina = direccionLogica / tamanoPagina;
    int offset = direccionLogica % tamanoPagina;

    auto tablaPaginas = proceso->obtenerTablaDePaginas();
    auto entrada = tablaPaginas->obtenerEntrada(numeroPagina);

    // Revisar si la dirección está en caché
    if (cache.find(direccionLogica) != cache.end())
    {
        logSistema.registrarHit();
        logSistema.registrarEvento("Hit en caché para dirección lógica: " + std::to_string(direccionLogica));
        return cache[direccionLogica] + offset;
    }

    // Revisar si está en la memoria principal
    if (entrada.valido)
    {
        logSistema.registrarHit();
        logSistema.registrarEvento("Hit en memoria principal para página: " + std::to_string(numeroPagina));

        // Actualizar bit de referencia
        tablaPaginas->establecerReferencia(numeroPagina, true);
        actualizarCache(direccionLogica, entrada.marco * tamanoPagina);

        return entrada.marco * tamanoPagina + offset;
    }

    // Manejo de fallo de página
    logSistema.registrarMiss();
    logSistema.registrarEvento("Miss para dirección lógica: " + std::to_string(direccionLogica));
    manejarFalloDePagina(direccionLogica, proceso);

    // Actualizar entrada después de manejar el fallo
    entrada = tablaPaginas->obtenerEntrada(numeroPagina);
    actualizarCache(direccionLogica, entrada.marco * tamanoPagina);

    return entrada.marco * tamanoPagina + offset;
}

// Manejo de fallo de página
void MMU::manejarFalloDePagina(int direccionLogica, Proceso *proceso)
{
    int numeroPagina = direccionLogica / tamanoPagina;

    // Leer la instrucción desde la memoria secundaria
    std::string instruccion = memoriaSecundaria.leerPagina(numeroPagina);

    // Asignar un marco libre en la memoria principal
    int marcoLibre = memoriaPrincipal.asignarMarco(numeroPagina);
    if (marcoLibre == -1)
    {
        // Aplicar algoritmo de reemplazo si no hay marcos disponibles
        int paginaReemplazo = proceso->obtenerTablaDePaginas()->buscarReemplazoNRU();
        reemplazarPaginaEnMemoriaPrincipal(paginaReemplazo, proceso);
        marcoLibre = memoriaPrincipal.asignarMarco(numeroPagina);
    }

    // Escribir la instrucción en el marco asignado
    memoriaPrincipal.escribirMarco(marcoLibre, instruccion);

    // Actualizar la tabla de páginas del proceso
    proceso->obtenerTablaDePaginas()->actualizarEntrada(numeroPagina, marcoLibre, true, false, true);

    logSistema.registrarEvento("Página " + std::to_string(numeroPagina) +
                               " cargada desde Memoria Secundaria al marco " +
                               std::to_string(marcoLibre) + ".");
}

void MMU::reemplazarPaginaEnMemoriaPrincipal(int paginaReemplazo, Proceso *proceso)
{
    auto entradaReemplazo = proceso->obtenerTablaDePaginas()->obtenerEntrada(paginaReemplazo);

    if (entradaReemplazo.valido)
    {
        // Si la página está sucia, escribirla de vuelta en la Memoria Secundaria
        if (entradaReemplazo.sucio)
        {
            std::string datos = memoriaPrincipal.leerMarco(entradaReemplazo.marco);
            memoriaSecundaria.escribirPagina(paginaReemplazo, datos);
            logSistema.registrarEvento("Página " + std::to_string(paginaReemplazo) + " escrita de vuelta a Memoria Secundaria.");
        }

        // Liberar el marco de la memoria principal
        memoriaPrincipal.liberarMarco(entradaReemplazo.marco);
        proceso->obtenerTablaDePaginas()->actualizarEntrada(paginaReemplazo, -1, false, false, false);
        logSistema.registrarEvento("Página " + std::to_string(paginaReemplazo) + " reemplazada.");
    }
}

void MMU::actualizarCache(int direccionLogica, int direccionFisica)
{
    // Si la caché está llena, elimina la entrada más antigua
    if (cache.size() >= tamanoCache)
    {
        int direccionAntigua = colaCache.front();
        colaCache.pop();
        cache.erase(direccionAntigua);
    }

    // Agregar nueva entrada a la caché
    cache[direccionLogica] = direccionFisica;
    colaCache.push(direccionLogica);
}

// Imprimir estadísticas
void MMU::imprimirEstadisticas() const
{
    logSistema.imprimirEstadisticas();
}

// Imprimir estado de memorias
void MMU::imprimirEstadoMemorias() const
{
    std::cout << "\nEstado de la Memoria Principal:\n";
    memoriaPrincipal.imprimirEstado();
}

const MemoriaPrincipal &MMU::getMemoriaPrincipal() const
{
    return memoriaPrincipal;
}
