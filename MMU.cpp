#include "MMU.h"

// Constructor
MMU::MMU(const std::string &archivoAlmacenamiento)
    : memoriaPrincipal(4, 64), memoriaSecundaria(archivoAlmacenamiento)
{
    totalFrames = 64 / 4;
    frameLimit = totalFrames;
}

int MMU::traducirDireccion(int direccionLogica, int procesoId)
{
    if (procesoId < 0 || procesoId >= procesosActuales.size())
    {
        std::cerr << "[ERROR] Proceso ID inválido.\n";
        return -1;
    }

    Proceso *proceso = procesosActuales[procesoId];

    auto tablaPaginas = proceso->ObtenerTablaDePaginas();
    int paginaLogica = direccionLogica / 4; // Tamaño de página: 4 líneas
    int offset = direccionLogica % 4;

    if (paginaLogica < 0 || paginaLogica >= tablaPaginas->ObtenerNumPaginas())
    {
        std::cerr << "[ERROR] Dirección lógica fuera de rango.\n";
        return -1;
    }

    if (!tablaPaginas->EstaSecundario(paginaLogica))
    {
        manejarFalloDePagina(paginaLogica, proceso);
    }

    int marco = tablaPaginas->ObtenerMarco(paginaLogica);
    int direccionFisica = (marco * 4) + offset;

    return direccionFisica;
}

void MMU::manejarFalloDePagina(int paginaLogica, Proceso *proceso)
{
    try
    {
        // Obtener el índice secundario para la página lógica
        int indiceSecundario = proceso->ObtenerTablaDePaginas()->indiceSecundario(paginaLogica);
        std::vector<std::string> data = memoriaSecundaria.leer_pagina(indiceSecundario, 4);

        int marcoLibre;
        int marcosAsignados = contarMarcosAsignados(proceso->getId());

        // Si el proceso no ha alcanzado su límite de marcos, intenta asignar un marco libre
        if (marcosAsignados < frameLimit)
        {
            marcoLibre = memoriaPrincipal.asignar_frame();
            if (marcoLibre != -1)
            {
                // Agregar el marco asignado al proceso en processFrames
                processFrames.push_back(std::make_pair(proceso->getId(), marcoLibre));
            }
        }
        else
        {
            // El proceso ha alcanzado su límite de marcos; no se puede asignar marco libre
            marcoLibre = -1;
        }

        if (marcoLibre == -1)
        {
            // No hay marcos disponibles o se superó el límite; aplicar reemplazo
            int paginaReemplazo = proceso->ObtenerTablaDePaginas()->buscarReemplazoNRU();
            int marcoReemplazo = proceso->ObtenerTablaDePaginas()->ObtenerMarco(paginaReemplazo);

            // Si la página está modificada, escribirla en memoria secundaria
            if (proceso->ObtenerTablaDePaginas()->EstaModificado(paginaReemplazo))
            {
                std::vector<std::string> datosReemplazo = memoriaPrincipal.obtener_frame(marcoReemplazo);
                memoriaSecundaria.escribir_pagina(proceso->ObtenerTablaDePaginas()->indiceSecundario(paginaReemplazo), datosReemplazo);
            }

            // Liberar el marco del reemplazo
            memoriaPrincipal.liberar_frame(marcoReemplazo);
            proceso->ObtenerTablaDePaginas()->ActualizarValidez(paginaReemplazo, false);

            // Asignar marcoLibre al marco que acabamos de liberar
            marcoLibre = marcoReemplazo;
        }

        // Cargar la nueva página en marcoLibre
        memoriaPrincipal.actualizar_frame(marcoLibre, data);
        proceso->ObtenerTablaDePaginas()->ActualizarFrame(paginaLogica, marcoLibre);
        proceso->ObtenerTablaDePaginas()->ActualizarValidez(paginaLogica, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERROR] " << e.what() << "\n";
    }
}


void MMU::asignarProceso(Proceso *proceso)
{
    std::vector<int> indices = memoriaSecundaria.getIndicesMemSecundariaDeProcesos(proceso->getId());
    proceso->setTotalInstrucciones(indices.size());
    for (int indice : indices)
    {
        std::cout << indice << " ";
    }
    std::cout << "\n";

    int lineasPorPagina = 4;
    int paginasNecesarias = (indices.size() + lineasPorPagina - 1) / lineasPorPagina;
    PageTable *tabla = new PageTable(paginasNecesarias);

    for (int i = 0; i < paginasNecesarias; ++i)
    {
        int indiceInicio = i * lineasPorPagina;
        int indiceSecundario;
        if (indiceInicio < indices.size())
        {
            indiceSecundario = indices[indiceInicio];
        }
        else
        {
            indiceSecundario = -1;
        }
        tabla->AgregarEntrada(i, -1, indiceSecundario, false);
    }

    proceso->setTablaDePaginas(tabla);
    procesosActuales.push_back(proceso);
    if (!procesosActuales.empty())
    {
        frameLimit = totalFrames / procesosActuales.size();
    }
}
void MMU::liberarProceso(int procesoId)
{
    // Liberar los marcos asignados al proceso en memoria principal y eliminarlos de processFrames
    for (size_t i = 0; i < processFrames.size();)
    {
        // Si el par en processFrames corresponde al procesoId
        if (processFrames[i].first == procesoId)
        {
            int marco = processFrames[i].second;

            // Liberar el marco en memoria principal
            memoriaPrincipal.liberar_frame(marco);

            // Eliminar el par del vector processFrames
            processFrames.erase(processFrames.begin() + i);
        }
        else
        {
            ++i;
        }
    }
    // Remover el proceso de la lista de procesos actuales
    for (size_t i = 0; i < procesosActuales.size();)
    {
        // Si encontramos el proceso con el procesoId
        if (procesosActuales[i]->getId() == procesoId)
        {
            // Opcional: Liberar la memoria del objeto Proceso si es necesario
            delete procesosActuales[i];

            // Eliminar el puntero del vector procesosActuales
            procesosActuales.erase(procesosActuales.begin() + i);

            // No incrementamos 'i' por la misma razón anterior
        }
        else
        {
            // Incrementamos 'i' si no eliminamos un elemento
            ++i;
        }
    }
    if (!procesosActuales.empty())
    {
        frameLimit = totalFrames / procesosActuales.size();
    }
    else
    {
        frameLimit = totalFrames; // Si no hay procesos, el límite es el total de marcos
    }
}

int MMU::contarMarcosAsignados(int processId)
{
    int count = 0;
    for (const auto &pair : processFrames)
    {
        if (pair.first == processId)
        {
            ++count;
        }
    }
    return count;
}

void MMU::removerMarcoDeProceso(int processId, int marco)
{
    for (std::vector<std::pair<int, int>>::iterator it = processFrames.begin(); it != processFrames.end();)
    {
        if (it->first == processId && it->second == marco)
        {
            it = processFrames.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

PrimaryMemory MMU::getMemoriaPrincipal() const
{
    return memoriaPrincipal;
}
