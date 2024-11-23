#ifndef PROCESS_H
#define PROCESS_H
#include <string>
#include <sstream>
#include "PageTable.h"
#include "MMU.h"

class Proceso
{
private:
    int id;                  // Identificador único del proceso.
    PageTable *tablaPaginas; // Tabla de páginas asociada al proceso.
    int totalInstrucciones;

public:
    // Constructor
    Proceso(int id);

    // Métodos
    void setTablaDePaginas(PageTable *tabla);
    int getId() const;
    PageTable *ObtenerTablaDePaginas() const;
    int ObtenerTotalInstrucciones() const;
    void setTotalInstrucciones (int total);
};

#endif // PROCESS_H
