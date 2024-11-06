#ifndef SECONDARY_MEMORY_H
#define SECONDARY_MEMORY_H

#include <string>

class SecondaryMemory {
public:
    SecondaryMemory(const std::string& archivo, int tamano);
    std::string leerPagina(int pagina);
    void escribirPagina(int pagina, const std::string& datos);
    void eliminarPagina(int pagina);

private:
    std::string archivo;
    int tamano;
};

#endif // SECONDARY_MEMORY_H
