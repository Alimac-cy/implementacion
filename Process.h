#ifndef PROCESS_H
#define PROCESS_H

#include <vector>
#include <string>
#include "PageTable.h"

class Proceso
{
private:
    int id;                                 // Identificador único del proceso.
    std::vector<std::string> instrucciones; // Lista de instrucciones (pseudo-ensamblador).
    std::string estado;                     // Estado del proceso (ejecutando, esperando, terminado).
    TablaDePaginas *tablaPaginas;           // Tabla de páginas asociada al proceso.

public:
    // Constructor
    Proceso(int id, TablaDePaginas *tablaPaginas);
    // Métodos
    int obtenerId() const;
    const std::vector<std::string> &obtenerInstrucciones() const;
    std::string obtenerEstado() const;
    void cambiarEstado(const std::string &nuevoEstado);
    std::string obtenerSiguienteInstruccion();
    TablaDePaginas *obtenerTablaDePaginas() const;
};

#endif // PROCESS_H
