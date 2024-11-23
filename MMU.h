#ifndef MMU_H
#define MMU_H

#include <vector>
#include <iostream>
#include "Process.h"
#include "PageTable.h"
#include "PrimaryMemory.h"
#include "SecondaryMemory.h"

class Proceso;

class MMU
{
private:
    PrimaryMemory memoriaPrincipal;          // Memoria principal.
    SecondaryMemory memoriaSecundaria;       // Almacenamiento secundario.
    std::vector<Proceso *> procesosActuales; // Procesos en ejecución.
public:
    // Constructor
    MMU(const std::string &archivoAlmacenamiento);
    // Metodos
    int traducirDireccion(int direccionLogica, int procesoId);
    void manejarFalloDePagina(int direccionLogica, Proceso *proceso);
    void asignarProceso(Proceso *proceso);
    PrimaryMemory getMemoriaPrincipal() const;
};

#endif // MMU_H
