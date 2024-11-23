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

    if (paginaLogica < 0 || paginaLogica >= tablaPaginas->obtenerNumPaginas())
    {
        std::cerr << "[ERROR] Dirección lógica fuera de rango.\n";
        return -1;
    }

    if (!tablaPaginas->estaSecundario(paginaLogica))
    {
        manejarFalloDePagina(paginaLogica, proceso);
    }

    int marco = tablaPaginas->obtenerMarco(paginaLogica);
    int direccionFisica = (marco * 4) + offset;

    return direccionFisica;
}

void MMU::manejarFalloDePagina(int paginaLogica, Proceso *proceso)
{
    try
    {
        // Obtener el índice secundario para la página lógica
        int indiceSecundario = proceso->ObtenerTablaDePaginas()->indiceSecundario(paginaLogica);
        std::vector<std::string> data = memoriaSecundaria.leerPagina(indiceSecundario, 4);

        int marcoLibre;
        int marcosAsignados = contarMarcosAsignados(proceso->obtenerId());

        // Si el proceso no ha alcanzado su límite de marcos, intenta asignar un marco libre
        if (marcosAsignados < frameLimit)
        {
            marcoLibre = memoriaPrincipal.asignarFrame();
            if (marcoLibre != -1)
            {
                // Agregar el marco asignado al proceso en processFrames
                processFrames.push_back(std::make_pair(proceso->obtenerId(), marcoLibre));
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
            int marcoReemplazo = proceso->ObtenerTablaDePaginas()->obtenerMarco(paginaReemplazo);

            // Si la página está modificada, escribirla en memoria secundaria
            if (proceso->ObtenerTablaDePaginas()->estaModificado(paginaReemplazo))
            {
                std::vector<std::string> datosReemplazo = memoriaPrincipal.obtenerFrame(marcoReemplazo);
                memoriaSecundaria.escribirPagina(proceso->ObtenerTablaDePaginas()->indiceSecundario(paginaReemplazo), datosReemplazo);
            }

            // Liberar el marco del reemplazo
            memoriaPrincipal.liberarFrame(marcoReemplazo);
            proceso->ObtenerTablaDePaginas()->actualizarValidez(paginaReemplazo, false);

            // Asignar marcoLibre al marco que acabamos de liberar
            marcoLibre = marcoReemplazo;
        }

        // Cargar la nueva página en marcoLibre
        memoriaPrincipal.actualizarFrame(marcoLibre, data);
        proceso->ObtenerTablaDePaginas()->actualizarFrame(paginaLogica, marcoLibre);
        proceso->ObtenerTablaDePaginas()->actualizarValidez(paginaLogica, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERROR] " << e.what() << "\n";
    }
}


void MMU::asignarProceso(Proceso *proceso)
{
    std::vector<int> indices = memoriaSecundaria.obtenerIndicesMemSecundariaDeProcesos(proceso->obtenerId());
    proceso->setTotalInstrucciones(indices.size());

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
        tabla->agregarEntrada(i, -1, indiceSecundario, false);
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
            memoriaPrincipal.liberarFrame(marco);

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
        if (procesosActuales[i]->obtenerId() == procesoId)
        {
            //Liberar la memoria del objeto Proceso si es necesario
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

PrimaryMemory MMU::getMemoriaPrincipal() const
{
    return memoriaPrincipal;
}
