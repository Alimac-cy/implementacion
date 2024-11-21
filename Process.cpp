#include "Process.h"

// Constructor
Proceso::Proceso(int id, TablaDePaginas *tablaPaginas)
    : id(id), estado("esperando"), tablaPaginas(tablaPaginas) {}

// Obtener ID
int Proceso::obtenerId() const
{
    return id;
}

// Obtener lista de instrucciones
const std::vector<std::string> &Proceso::obtenerInstrucciones() const
{
    return instrucciones;
}

// Obtener estado actual
std::string Proceso::obtenerEstado() const
{
    return estado;
}

// Cambiar estado
void Proceso::cambiarEstado(const std::string &nuevoEstado)
{
    estado = nuevoEstado;
}

// Obtener la siguiente instrucción
std::string Proceso::obtenerSiguienteInstruccion()
{
    if (!instrucciones.empty())
    {
        std::string instruccion = instrucciones.front();
        instrucciones.erase(instrucciones.begin());
        return instruccion;
    }
    return "";
}

// Obtener tabla de páginas asociada
TablaDePaginas *Proceso::obtenerTablaDePaginas() const
{
    return tablaPaginas;
}
