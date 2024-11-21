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
        std::cout << "[ERROR] Proceso con ID " << idProceso << " no encontrado. Traducción fallida.\n";
        return -1;
    }

    int numeroPagina = direccionLogica / tamanoPagina;
    int offset = direccionLogica % tamanoPagina;

    std::cout << "[INFO] Traducción iniciada para dirección lógica: " << direccionLogica
              << " (Página: " << numeroPagina << ", Offset: " << offset << ").\n";

    auto tablaPaginas = proceso->obtenerTablaDePaginas();
    auto entrada = tablaPaginas->obtenerEntrada(numeroPagina);

    // Revisar si la dirección está en caché
    if (cache.find(direccionLogica) != cache.end())
    {
        logSistema.registrarHit();
        logSistema.registrarEvento("Hit en caché para dirección lógica: " + std::to_string(direccionLogica));
        std::cout << "[SUCCESS] Dirección lógica " << direccionLogica << " encontrada en caché.\n";

        auto marco = cache[direccionLogica] / tamanoPagina;
        proceso->obtenerTablaDePaginas()->establecerReferencia(marco, true);
        return cache[direccionLogica];
    }

    // Revisar si está en la memoria principal
    if (entrada.valido)
    {
        logSistema.registrarHit();
        logSistema.registrarEvento("Hit en memoria principal para página: " + std::to_string(numeroPagina));
        std::cout << "[SUCCESS] Página " << numeroPagina << " encontrada en memoria principal.\n";

        // Actualizar bit de referencia
        tablaPaginas->establecerReferencia(numeroPagina, true);

        // Agregar la dirección a la caché
        actualizarCache(direccionLogica, entrada.marco * tamanoPagina);

        return entrada.marco * tamanoPagina + offset;
    }

    // Revisar si está en la memoria secundaria
    if (entrada.indiceSecundario != -1)
    {
        logSistema.registrarHit();
        logSistema.registrarEvento("Hit en memoria secundaria para página: " + std::to_string(numeroPagina));
        std::cout << "[INFO] Página " << numeroPagina << " encontrada en memoria secundaria. Cargando a memoria principal.\n";

        // Manejar fallo para cargar la página desde secundaria
        manejarFalloDePagina(direccionLogica, proceso, true);
        entrada = tablaPaginas->obtenerEntrada(numeroPagina);

        // Agregar la dirección a la caché
        actualizarCache(direccionLogica, entrada.marco * tamanoPagina);

        return entrada.marco * tamanoPagina + offset;
    }

    // Si no está en memoria principal ni secundaria, es un fallo de página
    logSistema.registrarMiss();
    logSistema.registrarEvento("Miss para dirección lógica: " + std::to_string(direccionLogica));
    std::cout << "[WARNING] Dirección lógica " << direccionLogica << " no encontrada en ninguna memoria. Procesando fallo de página.\n";

    manejarFalloDePagina(direccionLogica, proceso, false); // Manejar fallo como nuevo
    entrada = tablaPaginas->obtenerEntrada(numeroPagina);

    // Agregar la dirección a la caché
    actualizarCache(direccionLogica, entrada.marco * tamanoPagina);

    std::cout << "[SUCCESS] Dirección lógica " << direccionLogica << " traducida exitosamente a dirección física "
              << entrada.marco * tamanoPagina + offset << ".\n";

    return entrada.marco * tamanoPagina + offset;
}

// Manejo de fallo de página
void MMU::manejarFalloDePagina(int direccionLogica, Proceso *proceso, bool cargarDesdeSecundaria)
{
    int numeroPagina = direccionLogica / tamanoPagina;

    if (cargarDesdeSecundaria)
    {
        // Cargar desde memoria secundaria
        std::cout << "[INFO] Cargando página " << numeroPagina << " desde memoria secundaria.\n";

        int marcoLibre = memoriaPrincipal.asignarMarco(numeroPagina);
        if (marcoLibre == -1)
        {
            std::cout << "[WARNING] No hay marcos libres en memoria principal. Aplicando reemplazo.\n";
            int paginaReemplazo = proceso->obtenerTablaDePaginas()->buscarReemplazoNRU();
            reemplazarPaginaEnMemoriaPrincipal(paginaReemplazo, proceso);
            marcoLibre = memoriaPrincipal.asignarMarco(numeroPagina);
        }

        if (marcoLibre != -1)
        {
            // Leer de memoria secundaria a memoria principal
            memoriaSecundaria.leerPagina(numeroPagina, marcoLibre);

            // Actualizar tabla de páginas
            proceso->obtenerTablaDePaginas()->actualizarEntrada(numeroPagina, marcoLibre, true, false, true, -1);
            logSistema.registrarEvento("Página " + std::to_string(numeroPagina) + " cargada desde memoria secundaria.");
        }
    }
    else
    {
        // Manejo de fallo normal
        int marcoLibre = memoriaPrincipal.asignarMarco(numeroPagina);
        if (marcoLibre == -1)
        {
            int paginaReemplazo = proceso->obtenerTablaDePaginas()->buscarReemplazoNRU();
            reemplazarPaginaEnMemoriaPrincipal(paginaReemplazo, proceso);
            marcoLibre = memoriaPrincipal.asignarMarco(numeroPagina);
        }

        if (marcoLibre != -1)
        {
            proceso->obtenerTablaDePaginas()->actualizarEntrada(numeroPagina, marcoLibre, true, false, true, -1);
            logSistema.registrarEvento("Página " + std::to_string(numeroPagina) + " cargada en memoria principal.");
        }
    }
}

void MMU::reemplazarPaginaEnMemoriaSecundaria(Proceso *proceso, int numeroPagina, EntradaPagina paginaActual)
{
    std::cout << "[INFO] Memoria secundaria llena. Aplicando algoritmo de Reloj para reemplazo.\n";

    while (true)
    {
        auto entradaSecundaria = proceso->obtenerTablaDePaginas()->obtenerEntrada(punteroReloj);

        if (!entradaSecundaria.referenciado && entradaSecundaria.indiceSecundario != -1)
        {
            std::cout << "[INFO] Página " << punteroReloj << " seleccionada para reemplazo en memoria secundaria.\n";

            memoriaSecundaria.escribirPagina(punteroReloj, paginaActual.marco);

            // Actualizar tabla de páginas con nueva entrada
            proceso->obtenerTablaDePaginas()->actualizarEntrada(
                punteroReloj, -1, false, false, false, punteroReloj);

            punteroReloj = (punteroReloj + 1) % proceso->obtenerTablaDePaginas()->numPaginas();
            return;
        }

        // Resetear el bit de referencia y avanzar el puntero
        proceso->obtenerTablaDePaginas()->establecerReferencia(punteroReloj, false);
        punteroReloj = (punteroReloj + 1) % proceso->obtenerTablaDePaginas()->numPaginas();
    }
}

void MMU::reemplazarPaginaEnMemoriaPrincipal(int paginaReemplazo, Proceso *proceso)
{
    auto entradaReemplazo = proceso->obtenerTablaDePaginas()->obtenerEntrada(paginaReemplazo);

    if (entradaReemplazo.valido)
    {
        if (entradaReemplazo.sucio)
        {
            std::cout << "[INFO] Página " << paginaReemplazo << " está sucia. Moviendo a memoria secundaria.\n";
            memoriaSecundaria.escribirPagina(paginaReemplazo, entradaReemplazo.marco);
            logSistema.registrarEvento("Página " + std::to_string(paginaReemplazo) + " movida a memoria secundaria.");
        }

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
        std::cout << "[INFO] Caché llena. Eliminando dirección lógica " << direccionAntigua << ".\n";
    }

    // Agregar nueva entrada a la caché
    cache[direccionLogica] = direccionFisica;
    colaCache.push(direccionLogica);
    std::cout << "[INFO] Dirección lógica " << direccionLogica << " añadida a la caché con dirección física " << direccionFisica << ".\n";
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

    std::cout << "\nEstado de la Memoria Secundaria:\n";
    memoriaSecundaria.imprimirEstado();
}
