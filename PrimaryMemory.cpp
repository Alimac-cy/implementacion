#include "PrimaryMemory.h"

// Constructor
MemoriaPrincipal::MemoriaPrincipal(int tamanoMarco, int tamanoTotal)
    : tamanoMarco(tamanoMarco), tamanoTotal(tamanoTotal)
{
    int numMarcos = tamanoTotal / tamanoMarco;
    marcos.resize(numMarcos, ""); // Inicializa los marcos como vacíos (cadenas vacías).
}

// Método para asignar un marco libre
int MemoriaPrincipal::asignarMarco(int numeroPagina)
{
    for (int i = 0; i < marcos.size(); ++i)
    {
        if (marcos[i].empty()) // Encuentra un marco vacío
        {
            marcos[i] = "Página " + std::to_string(numeroPagina); // Representación básica
            std::cout << "[SUCCESS] Página " << numeroPagina << " asignada al marco " << i << ".\n";
            return i;
        }
    }
    return -1; // No hay marcos disponibles.
}

// Método para liberar un marco
void MemoriaPrincipal::liberarMarco(int indiceMarco)
{
    if (indiceMarco >= 0 && indiceMarco < marcos.size())
    {
        std::cout << "[INFO] Liberando marco " << indiceMarco << " que contenía: " << marcos[indiceMarco] << ".\n";
        marcos[indiceMarco] = ""; // Marca el marco como vacío.
    }
    else
    {
        std::cout << "[ERROR] Índice de marco inválido: " << indiceMarco << ".\n";
    }
}

// Método para obtener el contenido de un marco
std::string MemoriaPrincipal::leerMarco(int indiceMarco) const
{
    if (indiceMarco >= 0 && indiceMarco < marcos.size())
    {
        return marcos[indiceMarco];
    }
    std::cerr << "[ERROR] Índice de marco inválido: " << indiceMarco << ".\n";
    return "";
}

// Método para escribir datos en un marco específico
void MemoriaPrincipal::escribirMarco(int indiceMarco, const std::string &datos)
{
    if (indiceMarco >= 0 && indiceMarco < marcos.size())
    {
        marcos[indiceMarco] = datos; // Guarda los datos en el marco
    }
    else
    {
        std::cerr << "[ERROR] Índice de marco inválido: " << indiceMarco << ".\n";
    }
}

// Método para imprimir el estado actual de la memoria
void MemoriaPrincipal::imprimirEstado() const
{
    std::cout << "[INFO] Estado de la Memoria Principal:\n";
    for (int i = 0; i < marcos.size(); ++i)
    {
        if (marcos[i].empty())
        {
            std::cout << "Marco " << i << ": [VACÍO]\n";
        }
        else
        {
            std::cout << "Marco " << i << ": " << marcos[i] << "\n";
        }
    }
}
