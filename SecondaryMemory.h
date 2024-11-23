#ifndef SECONDARY_MEMORY_H
#define SECONDARY_MEMORY_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdexcept>

class SecondaryMemory
{
private:
    std::string archivo; // Ruta al archivo que representa el almacenamiento secundario.
    int tamano;          // Tamaño total del almacenamiento secundario(en bytes).

public:
    // Constructor
    SecondaryMemory(const std::string &archivo);

    std::vector<std::string> leerPagina(int indice, int cantLineas);  // Lee lo correspondiente a una página del archivo y la devuelve.
    bool escribirPagina(int indice, std::vector<std::string> datos);  // Escribe lo correspondiente una página en el archivo.
    bool eliminarPagina(int indice, int cantLineas);                  // Elimina lo correspondiente una página del archivo.
    std::vector<int> obtenerIndicesMemSecundariaDeProcesos(int idProceso); // Le da a la PageTable todos los indices que necesita en almacenamiento.
};

#endif // SECONDARY_MEMORY_H
