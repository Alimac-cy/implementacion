#ifndef SECONDARY_MEMORY_H
#define SECONDARY_MEMORY_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class MemoriaSecundaria
{
private:
    std::string archivo;      // Archivo que representa el almacenamiento secundario.
    int tamanoPagina;         // Tamaño de cada página.
    int tamanoTotal;          // Tamaño total del almacenamiento secundario.
    std::vector<int> paginas; // Vector para simular el estado de las páginas.

    // Método para actualizar el archivo cuando se escriben o eliminan páginas
    void actualizarArchivo(int numeroPagina, int datos);

public:
    // Constructor
    MemoriaSecundaria(const std::string &archivo, int tamanoPagina, int tamanoTotal);

    // Métodos
    void inicializarArchivo();
    void escribirPagina(int numeroPagina, int datos);
    int leerPagina(int numeroPagina);
    void leerPagina(int numeroPagina, int &marco);
    void eliminarPagina(int numeroPagina);
    void imprimirEstado() const;
    int asignarEspacio(int numeroPagina);
    void liberarEspacio(int indiceSecundario);
    int totalPaginas() const;
    int &obtenerEntrada(int indice);
};

#endif // SECONDARY_MEMORY_H
