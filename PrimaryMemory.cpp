#include "PrimaryMemory.h"
#include <iostream>

// Constructor
MemoriaPrincipal::MemoriaPrincipal(int tamanoMarco, int tamanoTotal)
    : tamanoMarco(tamanoMarco), tamanoTotal(tamanoTotal)
{
    int numMarcos = tamanoTotal / tamanoMarco;
    marcos.resize(numMarcos, -1); // Inicializa los marcos como vacíos (-1 significa vacío).
}

// Método para asignar un marco libre
int MemoriaPrincipal::asignarMarco(int numeroPagina)
{
    for (int i = 0; i < marcos.size(); ++i)
    {
        if (marcos[i] == -1)
        { // Encuentra un marco vacío
            marcos[i] = numeroPagina;
            std::cout << "[SUCCESS] Página " << numeroPagina << " asignada al marco " << i << ".\n";
            return i;
        }
    }
    std::cout << "[WARNING] No hay marcos libres en memoria principal.\n";
    return -1; // No hay marcos disponibles.
}

// Método para liberar un marco
void MemoriaPrincipal::liberarMarco(int indiceMarco)
{
    if (indiceMarco >= 0 && indiceMarco < marcos.size())
    {
        std::cout << "[INFO] Liberando marco " << indiceMarco << " que contenía la página " << marcos[indiceMarco] << ".\n";
        marcos[indiceMarco] = -1; // Marca el marco como vacío.
    }
    else
    {
        std::cout << "[ERROR] Índice de marco inválido: " << indiceMarco << ".\n";
    }
}

// Método para obtener el contenido de un marco
int MemoriaPrincipal::obtenerPagina(int indiceMarco) const
{
    if (indiceMarco >= 0 && indiceMarco < marcos.size())
    {
        return marcos[indiceMarco];
    }
    std::cout << "[ERROR] Índice de marco inválido: " << indiceMarco << ".\n";
    return -1;
}

// Método para imprimir el estado actual de la memoria
void MemoriaPrincipal::imprimirEstado() const
{
    std::cout << "[INFO] Estado de la Memoria Principal:\n";
    for (int i = 0; i < marcos.size(); ++i)
    {
        if (marcos[i] == -1)
        {
            std::cout << "Marco " << i << ": [VACÍO]\n";
        }
        else
        {
            std::cout << "Marco " << i << ": Página " << marcos[i] << "\n";
        }
    }
}
