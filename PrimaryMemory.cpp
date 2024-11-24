#include "PrimaryMemory.h"

// Constructor
PrimaryMemory::PrimaryMemory(int TamanoFrame, int Tamano)
    : TamanoFrame(TamanoFrame), Tamano(Tamano)
{
    int numMarcos = (Tamano + TamanoFrame - 1) / TamanoFrame;            // Redondear hacia arriba
    Marcos.resize(numMarcos, std::vector<std::string>(TamanoFrame, "")); // Inicializa todos los marcos con líneas vacías.
}

// Método para asignar un marco libre
int PrimaryMemory::asignarFrame()
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
bool PrimaryMemory::liberarFrame(int indice)
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
std::vector<std::string> PrimaryMemory::obtenerFrame(int indice)
{
    if (indice >= 0 && indice < Marcos.size())
    {
        return Marcos[indice];
    }
    return {}; // Devuelve un marco vacío si el índice es inválido.
}

// Método para actualizar un marco
bool PrimaryMemory::actualizarFrame(int indice, const std::vector<std::string> &datos)
{
    if (indice >= 0 && indice < Marcos.size() && datos.size() <= TamanoFrame)
    {
        Marcos[indice].clear();
        Marcos[indice].resize(TamanoFrame, ""); // Rellenar con cadenas vacías
        // Copiar los datos disponibles
        for (size_t i = 0; i < datos.size(); ++i)
        {
            Marcos[indice][i] = datos[i];
        }
        return true;
    }
    return false;
}

std::string PrimaryMemory::obtenerInstruccion(int direccionFisica)
{
    int marco = direccionFisica / TamanoFrame;
    int desplazamiento = direccionFisica % TamanoFrame;

    if (marco < 0 || marco >= Marcos.size())
    {
        return "";
    }

    if (desplazamiento < 0 || desplazamiento >= Marcos[marco].size())
    {
        return ""; // Posición inválida dentro del marco
    }

    return Marcos[marco][desplazamiento];
}

bool PrimaryMemory::actualizarInstruccion(int direccionFisica, const std::string &nuevaInstruccion)
{
    int marco = direccionFisica / TamanoFrame;
    int offset = direccionFisica % TamanoFrame;

    // Validar marco y offset
    if (marco < 0 || marco >= Marcos.size())
    {
        std::cerr << "[ERROR] Dirección física fuera de rango (marco inválido): " << direccionFisica << "\n";
        return false;
    }
    if (offset < 0)
    {
        std::cerr << "[ERROR] Offset negativo en la dirección física: " << direccionFisica << "\n";
        return false;
    }
    // Si el marco no tiene suficiente tamaño, redimensionar
    if (offset >= Marcos[marco].size())
    {
        if (offset >= TamanoFrame)
        {
            std::cerr << "[ERROR] Offset excede el tamaño máximo del marco: " << direccionFisica << "\n";
            return false;
        }
        Marcos[marco].resize(offset + 1, ""); // Inicializar nuevas instrucciones como vacías
    }
    // Actualizar 
    Marcos[marco][offset] = nuevaInstruccion;

    return true;
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
                std::cout << linea << " ";
            }
        }
        std::cout << "\n";
    }
}
