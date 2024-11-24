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
    int instruccionesEnPagina = tablaPaginas->obtenerInstruccionesEnPagina(paginaLogica);

    if (offset >= instruccionesEnPagina)
    {
        std::cerr << "[ERROR] Instrucines en pagina: " << instruccionesEnPagina << "\n";
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
        int instrucionesPagina = proceso->ObtenerTablaDePaginas()->obtenerInstruccionesEnPagina(paginaLogica);

        std::vector<std::string> data = memoriaSecundaria.leerPagina(indiceSecundario, instrucionesPagina);

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
    int totalInstrucciones = indices.size();
    proceso->setTotalInstrucciones(totalInstrucciones);

    int lineasPorPagina = 4;
    int paginasNecesarias = (indices.size() + lineasPorPagina - 1) / lineasPorPagina;
    PageTable *tabla = new PageTable(paginasNecesarias);

    for (int i = 0; i < paginasNecesarias; ++i)
    {
        int indiceInicio = i * lineasPorPagina;
        int instruccionesEnPagina;
        int instruccionesRestantes = totalInstrucciones - indiceInicio;

        if (instruccionesRestantes >= lineasPorPagina)
        {
            instruccionesEnPagina = lineasPorPagina;
        }
        else
        {
            instruccionesEnPagina = instruccionesRestantes;
        }
        int indiceSecundario;
        if (indiceInicio < indices.size())
        {
            indiceSecundario = indices[indiceInicio];
        }
        else
        {
            indiceSecundario = -1;
        }
        tabla->agregarEntrada(i, -1, indiceSecundario, false, instruccionesEnPagina);
    }

    proceso->setTablaDePaginas(tabla);
    procesosActuales.push_back(proceso);
    if (!procesosActuales.empty())
    {
        frameLimit = totalFrames / procesosActuales.size();
        // std::cout<<"frame limit actual: "<<frameLimit <<std::endl;
    }
}
void MMU::liberarProceso(int procesoId)
{
    // Buscar el proceso en procesosActuales
    Proceso *proceso = nullptr;
    for (auto p : procesosActuales)
    {
        if (p->obtenerId() == procesoId)
        {
            proceso = p;
            break;
        }
    }
    if (!proceso)
    {
        std::cerr << "[ERROR] Proceso con ID " << procesoId << " no encontrado.\n";
        return;
    }

    PageTable *tablaPaginas = proceso->ObtenerTablaDePaginas();

    // Iterar sobre los marcos asignados al proceso
    for (int i = 0; i < processFrames.size();)
    {
        if (processFrames[i].first == procesoId)
        {
            int marco = processFrames[i].second;

            // Encontrar la página lógica que usa este marco
            int paginaLogica = -1;
            for (int p = 0; p < tablaPaginas->obtenerNumPaginas(); ++p)
            {
                if (tablaPaginas->obtenerMarco(p) == marco)
                {
                    paginaLogica = p;
                    break;
                }
            }
            if (paginaLogica != -1)
            {
                // Si la página está modificada
                if (tablaPaginas->estaModificado(paginaLogica))
                {
                    // Obtener los datos del marco desde la memoria principal
                    int indiceSecundario = tablaPaginas->indiceSecundario(paginaLogica);
                    std::vector<std::string> datos = memoriaPrincipal.obtenerFrame(marco);

                    // Escribir solo las líneas que tienen datos en el almacenamiento secundario
                    if (memoriaSecundaria.escribirPagina(indiceSecundario, datos))
                    {
                        tablaPaginas->actualizarModificacion(paginaLogica, false);
                    }
                    else
                    {
                        std::cerr << "[ERROR] No se pudo escribir la página lógica " << paginaLogica << " del proceso " << procesoId << " en almacenamiento secundario.\n";
                    }
                }
                // Liberar el marco en memoria principal
                memoriaPrincipal.liberarFrame(marco);
                tablaPaginas->actualizarValidez(paginaLogica, false);
            }
            else
            {
                std::cerr << "[ERROR] No se encontró página lógica para el marco " << marco << " del proceso " << procesoId << ".\n";
            }
            // Eliminar el marco de processFrames
            processFrames.erase(processFrames.begin() + i);
        }
        else
        {
            ++i;
        }
    }
    // Remover el proceso de la lista de procesos actuales
    for (int i = 0; i < procesosActuales.size();)
    {
        // Si encontramos el proceso con el procesoId
        if (procesosActuales[i]->obtenerId() == procesoId)
        {
            // Eliminar el puntero del vector procesosActuales
            procesosActuales.erase(procesosActuales.begin() + i);
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

PrimaryMemory &MMU::getMemoriaPrincipal()
{
    return memoriaPrincipal;
}


bool MMU::modificarInstruccion(int procesoId, int direccionLogica, const std::string &nuevaInstruccion)
{
    // Traducir dirección lógica a física
    int direccionFisica = traducirDireccion(direccionLogica, procesoId);
    if (direccionFisica == -1)
    {
        std::cerr << "[ERROR] No se pudo traducir la dirección lógica.\n";
        return false;
    }
    if (!memoriaPrincipal.actualizarInstruccion(direccionFisica, nuevaInstruccion))
    {
        std::cerr << "[ERROR] No se pudo actualizar la instrucción en la memoria principal.\n";
        return false;
    }
    // Marcar la página como modificada
    int paginaLogica = direccionLogica / 4;
    Proceso *proceso = procesosActuales[procesoId];
    proceso->ObtenerTablaDePaginas()->actualizarModificacion(paginaLogica, true);
    return true;
}