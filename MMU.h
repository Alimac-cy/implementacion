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
    int frameLimit;                          // Límite de marcos por proceso
    int totalFrames;                         // Número total de marcos en memoria principal
    PrimaryMemory memoriaPrincipal;          // Memoria principal.
    SecondaryMemory memoriaSecundaria;       // Almacenamiento secundario.
    std::vector<Proceso *> procesosActuales; // Procesos en ejecución.
    std::vector<std::pair<int, int>> processFrames; //mapea cuales frames estan ocupados por cuales procesos

public:
    // Constructor
    MMU(const std::string &archivoAlmacenamiento);
    // Metodos
    int traducirDireccion(int direccionLogica, int procesoId);
    void manejarFalloDePagina(int direccionLogica, Proceso *proceso);
    void asignarProceso(Proceso *proceso);
    void liberarProceso(int procesoId);
    int contarMarcosAsignados(int processId);
    PrimaryMemory getMemoriaPrincipal() const;
    void removerMarcoDeProceso(int processId, int marco);
};

#endif // MMU_H
