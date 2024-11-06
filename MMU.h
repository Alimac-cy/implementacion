#ifndef MMU_H
#define MMU_H

#include <vector>
#include "Process.h"
#include "PageTable.h"
#include "PrimaryMemory.h"
#include "SecondaryMemory.h"
#include "Log.h"

class MMU {
public:
    MMU(PrimaryMemory* memoriaPrincipal, SecondaryMemory* almacenamientoSecundario);
    int traducirDireccion(int direccionLogica, Process* proceso);
    void manejarFalloPagina(int direccionLogica, Process* proceso);

private:
    PrimaryMemory* memoriaPrincipal;
    SecondaryMemory* almacenamientoSecundario;
    std::vector<Process*> procesos;
    Log* estadisticas;
};

#endif // MMU_H
