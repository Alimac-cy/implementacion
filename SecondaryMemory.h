#ifndef SECONDARY_MEMORY_H
#define SECONDARY_MEMORY_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

class MemoriaSecundaria
{
private:
    std::string archivo; // Ruta al archivo que representa el almacenamiento secundario.
    int tamanoPagina;    // Tamaño de cada página.
    int tamanoTotal;     // Tamaño total del almacenamiento secundario.

    void inicializarArchivo(); // Inicializa el archivo con páginas vacías.

public:
    // Constructor
    MemoriaSecundaria(const std::string &archivo, int tamanoPagina, int tamanoTotal);

    // Métodos principales
    void escribirPagina(int numeroPagina, const std::string &instruccion);
    std::string leerPagina(int numeroPagina);
    void eliminarPagina(int numeroPagina);
    int totalPaginas() const;
};

#endif // SECONDARY_MEMORY_H
