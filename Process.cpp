#include "Process.h"

// Constructor
Proceso::Proceso(int id)
    : id(id) {}

// Asocia una tabla de páginas
void Proceso::setTablaDePaginas(PageTable *tabla)
{
    tablaPaginas = tabla;
}

int Proceso::obtenerId() const
{
    return id;
}

PageTable *Proceso::ObtenerTablaDePaginas() const
{
    return tablaPaginas;
}

int Proceso::ObtenerTotalInstrucciones() const
{
    return totalInstrucciones;
}

void Proceso::setTotalInstrucciones(int total){
    totalInstrucciones = total;
}
