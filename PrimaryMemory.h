#ifndef PRIMARY_MEMORY_H
#define PRIMARY_MEMORY_H

#include <vector>
#include <iostream>

class MemoriaPrincipal
{
private:
    int tamanoMarco;                 // Tamaño de cada marco (4 KB).
    int tamanoTotal;                 // Tamaño total de la memoria (64 KB).
    std::vector<std::string> marcos; // Vector que representa los marcos en memoria física.
public:
    // Constructor
    MemoriaPrincipal(int tamanoMarco, int tamanoTotal);

    // Método para asignar un marco libre
    int asignarMarco(int numeroPagina);

    // Método para liberar un marco
    void liberarMarco(int indiceMarco);

    // Método para obtener el contenido de un marco
    int obtenerPagina(int indiceMarco) const;

    // Método para imprimir el estado actual de la memoria
    void imprimirEstado() const;

    void escribirMarco(int indiceMarco, const std::string &datos);

    std::string leerMarco(int indiceMarco) const;
};

#endif // PRIMARY_MEMORY_H
