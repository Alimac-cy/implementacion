#include "SecondaryMemory.h"

// Constructor
MemoriaSecundaria::MemoriaSecundaria(const std::string &archivo, int tamanoPagina, int tamanoTotal)
    : archivo(archivo), tamanoPagina(tamanoPagina), tamanoTotal(tamanoTotal)
{
    inicializarArchivo();
}

// Inicializa el archivo con datos vacíos si no existe
void MemoriaSecundaria::inicializarArchivo()
{
    std::fstream stream(archivo, std::ios::in | std::ios::out);
    if (!stream)
    {
        std::cout << "[INFO] Archivo no encontrado. Creando archivo nuevo.\n";
        std::ofstream nuevoArchivo(archivo);
        nuevoArchivo.close();
        stream.open(archivo, std::ios::in | std::ios::out);
    }

    // Verifica si el archivo necesita ser llenado con páginas vacías
    int paginasExistentes = totalPaginas();
    int paginasTotales = tamanoTotal / tamanoPagina;
    if (paginasExistentes < paginasTotales)
    {
        stream.seekp(0, std::ios::end);
        for (int i = paginasExistentes; i < paginasTotales; ++i)
        {
            stream << "\n"; // Línea vacía para representar una página vacía
        }
    }

    stream.close();
}

// Escribe datos en una página específica
void MemoriaSecundaria::escribirPagina(int numeroPagina, const std::string &instruccion)
{
    std::ifstream archivoLectura(this->archivo);
    std::vector<std::string> lineas;
    std::string linea;

    if (!archivoLectura)
    {
        throw std::runtime_error("[ERROR] Archivo no encontrado");
    }

    // Leer todas las líneas
    while (std::getline(archivoLectura, linea))
    {
        lineas.push_back(linea);
    }
    archivoLectura.close();

    // Ajustar el tamaño del vector para incluir todas las páginas necesarias
    if (numeroPagina >= lineas.size())
    {
        lineas.resize(numeroPagina + 1, ""); // Rellenar con líneas vacías
    }

    // Actualizar la línea correspondiente al número de página
    lineas[numeroPagina] = instruccion;

    // Escribir de vuelta al archivo
    std::ofstream archivoEscritura(this->archivo);
    for (const auto &l : lineas)
    {
        archivoEscritura << l << "\n";
    }

    archivoEscritura.close();
    std::cout << "[INFO] Página " << numeroPagina << " escrita con instrucción: " << instruccion << "\n";
}

// Lee los datos de una página específica
std::string MemoriaSecundaria::leerPagina(int numeroPagina)
{
    std::ifstream archivo(this->archivo);
    if (!archivo)
    {
        throw std::runtime_error("[ERROR] Archivo no encontrado");
    }

    std::string linea;
    int contador = 0;

    // Buscar la línea correspondiente a la página
    while (std::getline(archivo, linea))
    {
        if (contador == numeroPagina)
        {
            archivo.close();
            std::cout << "[INFO] Página " << numeroPagina << " leída: " << linea << "\n";
            return linea; // Devuelve el contenido de la página directamente
        }
        contador++;
    }

    archivo.close();
    throw std::out_of_range("[ERROR] Página fuera de rango");
}

// Elimina una página del archivo
void MemoriaSecundaria::eliminarPagina(int numeroPagina)
{
    escribirPagina(numeroPagina, ""); // Reemplaza la línea con una instrucción vacía.
    std::cout << "[INFO] Página " << numeroPagina << " eliminada.\n";
}

// Devuelve el número total de páginas disponibles
int MemoriaSecundaria::totalPaginas() const
{
    std::ifstream archivo(this->archivo);
    if (!archivo)
    {
        return 0;
    }

    int contador = 0;
    std::string linea;
    while (std::getline(archivo, linea))
    {
        contador++;
    }
    return contador;
}
