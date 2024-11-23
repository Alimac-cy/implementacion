#include "MMU.h"

// Constructor
MMU::MMU(const std::string &archivoAlmacenamiento)
    : memoriaPrincipal(4, 64), memoriaSecundaria(archivoAlmacenamiento)
{
}

void MMU::manejarFalloDePagina(int paginaLogica, Proceso *proceso)
{
    try
    {
        int indiceSecundario = proceso->ObtenerTablaDePaginas()->indiceSecundario(paginaLogica);
        std::vector<std::string> data = memoriaSecundaria.leer_pagina(indiceSecundario, 4);

        int marcoLibre = memoriaPrincipal.asignar_frame();
        if (marcoLibre == -1)
        {
            // No hay marcos disponibles; aplicar reemplazo
            int paginaReemplazo = proceso->ObtenerTablaDePaginas()->buscarReemplazoNRU();
            if (proceso->ObtenerTablaDePaginas()->EstaModificado(paginaReemplazo))
            {
                std::vector<std::string> datosReemplazo = memoriaPrincipal.obtener_frame(
                    proceso->ObtenerTablaDePaginas()->ObtenerMarco(paginaReemplazo));
                memoriaSecundaria.escribir_pagina(proceso->ObtenerTablaDePaginas()->indiceSecundario(paginaReemplazo), datosReemplazo);
            }

            memoriaPrincipal.liberar_frame(proceso->ObtenerTablaDePaginas()->ObtenerMarco(paginaReemplazo));
            proceso->ObtenerTablaDePaginas()->ActualizarValidez(paginaReemplazo, false);
        }

        memoriaPrincipal.actualizar_frame(marcoLibre, data);
        proceso->ObtenerTablaDePaginas()->ActualizarFrame(paginaLogica, marcoLibre);
        proceso->ObtenerTablaDePaginas()->ActualizarValidez(paginaLogica, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERROR] " << e.what() << "\n";
    }
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
        int indiceSecundario = (indiceInicio < indices.size()) ? indices[indiceInicio] : -1;
        tabla->AgregarEntrada(i, -1, indiceSecundario, false);
    }

    proceso->setTablaDePaginas(tabla);
    procesosActuales.push_back(proceso);
}

PrimaryMemory MMU::getMemoriaPrincipal() const
{
    return memoriaPrincipal;
}
