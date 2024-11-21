#include "PageTable.h"

// Constructor
TablaDePaginas::TablaDePaginas(int numPaginas) : punteroReloj(0)
{
    tabla.resize(numPaginas, {-1, -1, false, false, false});
}

// Actualizar entrada
void TablaDePaginas::actualizarEntrada(int numeroPagina, int marco, bool valido, bool sucio, bool referenciado, int indiceSecundario)
{
    if (numeroPagina < 0 || numeroPagina >= tabla.size())
        return;

    tabla[numeroPagina] = {marco, indiceSecundario, valido, sucio, referenciado};
}

// Obtener entrada
EntradaPagina TablaDePaginas::obtenerEntrada(int numeroPagina) const
{
    if (numeroPagina < 0 || numeroPagina >= tabla.size())
        return {-1, -1, false, false, false}; // Retornar una entrada inválida si el índice está fuera de rango

    return tabla[numeroPagina];
}

// Establecer referencia
void TablaDePaginas::establecerReferencia(int numeroPagina, bool referenciado)
{
    if (numeroPagina < 0 || numeroPagina >= tabla.size())
        return;

    tabla[numeroPagina].referenciado = referenciado;
}

// Establecer sucio
void TablaDePaginas::establecerSucio(int numeroPagina, bool sucio)
{
    if (numeroPagina < 0 || numeroPagina >= tabla.size())
        return;

    tabla[numeroPagina].sucio = sucio;
}

// Buscar reemplazo con NRU (Not Recently Used)
int TablaDePaginas::buscarReemplazoNRU()
{
    // Clase 00: no referenciado, no sucio
    for (int i = 0; i < tabla.size(); ++i)
    {
        if (!tabla[i].referenciado && !tabla[i].sucio && tabla[i].valido)
            return i;
    }

    // Clase 01: no referenciado, sucio
    for (int i = 0; i < tabla.size(); ++i)
    {
        if (!tabla[i].referenciado && tabla[i].sucio && tabla[i].valido)
            return i;
    }

    // Clase 10: referenciado, no sucio
    for (int i = 0; i < tabla.size(); ++i)
    {
        if (tabla[i].referenciado && !tabla[i].sucio && tabla[i].valido)
            return i;
    }

    // Clase 11: referenciado, sucio
    for (int i = 0; i < tabla.size(); ++i)
    {
        if (tabla[i].referenciado && tabla[i].sucio && tabla[i].valido)
            return i;
    }

    return -1; // No se encontró una página para reemplazo
}

// Buscar reemplazo con algoritmo de Reloj
int TablaDePaginas::buscarReemplazoReloj()
{
    while (true)
    {
        int pagina = punteroReloj % tabla.size();
        auto &entrada = tabla[pagina];

        if (!entrada.referenciado && entrada.valido)
        {
            punteroReloj = (punteroReloj + 1) % tabla.size();
            return pagina; // Página seleccionada para reemplazo
        }

        entrada.referenciado = false; // Resetear bit de referencia
        punteroReloj = (punteroReloj + 1) % tabla.size();
    }
}

// Obtener el número total de páginas
int TablaDePaginas::numPaginas() const
{
    return tabla.size();
}
