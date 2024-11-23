#include "PrimaryMemory.h"

// Constructor
PrimaryMemory::PrimaryMemory(int TamanoFrame, int Tamano)
    : TamanoFrame(TamanoFrame), Tamano(Tamano)
{
    int numMarcos = (Tamano + TamanoFrame - 1) / TamanoFrame;            // Redondear hacia arriba
    Marcos.resize(numMarcos, std::vector<std::string>(TamanoFrame, "")); // Inicializa todos los marcos con líneas vacías.
}

// Método para asignar un marco libre
int PrimaryMemory::asignar_frame()
{
    for (int i = 0; i < Marcos.size(); ++i)
    {
        if (Marcos[i][0].empty()) // Verifica si el marco está vacío (línea inicial vacía).
        {
            return i; // Devuelve el índice del marco libre.
        }
    }
    return -1; // No hay marcos disponibles.
}

// Método para liberar un marco
bool PrimaryMemory::liberar_frame(int indice)
{
    if (indice >= 0 && indice < Marcos.size())
    {
        for (auto &linea : Marcos[indice])
        {
            linea.clear(); // Borra el contenido de todas las líneas en el marco.
        }
        return true;
    }
    return false;
}

// Método para obtener un marco
std::vector<std::string> PrimaryMemory::obtener_frame(int indice)
{
    if (indice >= 0 && indice < Marcos.size())
    {
        return Marcos[indice];
    }
    return {}; // Devuelve un marco vacío si el índice es inválido.
}

// Método para actualizar un marco
bool PrimaryMemory::actualizar_frame(int indice, const std::vector<std::string> &datos)
{
    if (indice >= 0 && indice < Marcos.size() && datos.size() <= static_cast<size_t>(TamanoFrame))
    {
        Marcos[indice] = datos; // Actualiza el marco con los datos proporcionados.
        return true;
    }
    return false;
}

std::string PrimaryMemory::obtenerInstruccion(int direccionFisica) const
{
    int frameSize = 4; // Tamaño del frame
    int marco = direccionFisica / frameSize;
    int desplazamiento = direccionFisica % frameSize;

    if (marco < 0 || marco >= Marcos.size())
    {
        return "";
    }

    return Marcos[marco][desplazamiento];
}


void PrimaryMemory::imprimirEstado() const
{
    std::cout << "[ESTADO DE LA MEMORIA PRINCIPAL]\n";
    for (int i = 0; i < Marcos.size(); ++i)
    {
        std::cout << "Marco " << i << ": ";
        for (const auto &linea : Marcos[i])
        {
            if (linea.empty())
            {
                std::cout << "[VACÍO] ";
            }
            else
            {
                std::cout << linea << " _ ";
            }
        }
        std::cout << "\n";
    }
}


