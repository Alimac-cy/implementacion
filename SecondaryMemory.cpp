#include "SecondaryMemory.h"

// Constructor
MemoriaSecundaria::MemoriaSecundaria(const std::string &archivo, int tamanoPagina, int tamanoTotal)
    : archivo(archivo), tamanoPagina(tamanoPagina), tamanoTotal(tamanoTotal)
{
    int numPaginas = tamanoTotal / tamanoPagina;
    paginas.resize(numPaginas, -1); // Inicializa las páginas como vacías (-1 significa vacío).
    inicializarArchivo();           // Prepara el archivo para almacenamiento.
}

// Método para inicializar el archivo de almacenamiento
void MemoriaSecundaria::inicializarArchivo()
{
    std::ofstream outFile(archivo, std::ios::trunc);
    for (int i = 0; i < paginas.size(); ++i)
    {
        outFile << "Página " << i << ": [VACÍA]\n";
    }
    outFile.close();
    std::cout << "[INFO] Archivo de almacenamiento secundario inicializado.\n";
}

// Método para escribir una página en el almacenamiento secundario
void MemoriaSecundaria::escribirPagina(int numeroPagina, int datos)
{
    if (numeroPagina >= 0 && numeroPagina < paginas.size())
    {
        paginas[numeroPagina] = datos;
        actualizarArchivo(numeroPagina, datos);
        std::cout << "[SUCCESS] Página " << numeroPagina << " escrita en almacenamiento secundario.\n";
    }
    else
    {
        std::cout << "[ERROR] Número de página inválido: " << numeroPagina << ".\n";
    }
}

// Método para leer una página del almacenamiento secundario
int MemoriaSecundaria::leerPagina(int numeroPagina)
{
    if (numeroPagina >= 0 && numeroPagina < paginas.size())
    {
        if (paginas[numeroPagina] != -1)
        {
            std::cout << "[SUCCESS] Página " << numeroPagina << " leída desde almacenamiento secundario.\n";
            return paginas[numeroPagina];
        }
        else
        {
            std::cout << "[WARNING] Página " << numeroPagina << " está vacía.\n";
            return -1;
        }
    }
    else
    {
        std::cout << "[ERROR] Número de página inválido: " << numeroPagina << ".\n";
        return -1;
    }
}

void MemoriaSecundaria::leerPagina(int numeroPagina, int &marco)
{
    if (numeroPagina >= 0 && numeroPagina < paginas.size())
    {
        if (paginas[numeroPagina] != -1)
        {
            marco = paginas[numeroPagina]; // Carga el contenido de la página al marco
            std::cout << "[SUCCESS] Página " << numeroPagina << " leída desde almacenamiento secundario.\n";
        }
        else
        {
            std::cout << "[WARNING] Página " << numeroPagina << " está vacía.\n";
            marco = -1;
        }
    }
    else
    {
        std::cout << "[ERROR] Número de página inválido: " << numeroPagina << ".\n";
        marco = -1;
    }
}

// Método para eliminar una página del almacenamiento secundario
void MemoriaSecundaria::eliminarPagina(int numeroPagina)
{
    if (numeroPagina >= 0 && numeroPagina < paginas.size())
    {
        paginas[numeroPagina] = -1;
        actualizarArchivo(numeroPagina, -1);
        std::cout << "[SUCCESS] Página " << numeroPagina << " eliminada del almacenamiento secundario.\n";
    }
    else
    {
        std::cout << "[ERROR] Número de página inválido: " << numeroPagina << ".\n";
    }
}

// Método para imprimir el estado del almacenamiento secundario
void MemoriaSecundaria::imprimirEstado() const
{
    std::cout << "[INFO] Estado del Almacenamiento Secundario:\n";
    for (int i = 0; i < paginas.size(); ++i)
    {
        if (paginas[i] == -1)
        {
            std::cout << "Página " << i << ": [VACÍA]\n";
        }
        else
        {
            std::cout << "Página " << i << ": " << paginas[i] << "\n";
        }
    }
}

// Método para actualizar el archivo cuando se escriben o eliminan páginas
void MemoriaSecundaria::actualizarArchivo(int numeroPagina, int datos)
{
    std::ifstream inFile(archivo);
    std::vector<std::string> contenido;
    std::string linea;

    // Leer el archivo actual
    while (std::getline(inFile, linea))
    {
        contenido.push_back(linea);
    }
    inFile.close();

    // Actualizar la línea correspondiente
    if (numeroPagina >= 0 && numeroPagina < contenido.size())
    {
        contenido[numeroPagina] = "Página " + std::to_string(numeroPagina) + ": " +
                                  (datos == -1 ? "[VACÍA]" : std::to_string(datos));
    }

    // Escribir el archivo actualizado
    std::ofstream outFile(archivo, std::ios::trunc);
    for (const auto &linea : contenido)
    {
        outFile << linea << "\n";
    }
    outFile.close();
}

int MemoriaSecundaria::asignarEspacio(int numeroPagina)
{
    for (int i = 0; i < paginas.size(); ++i)
    {
        if (paginas[i] == -1)
        {
            paginas[i] = numeroPagina;
            return i;
        }
    }
    std::cerr << "[ERROR] Memoria secundaria llena. No se pudo asignar espacio.\n";
    return -1;
}

void MemoriaSecundaria::liberarEspacio(int indiceSecundario)
{
    if (indiceSecundario >= 0 && indiceSecundario < paginas.size())
    {
        paginas[indiceSecundario] = -1;
    }
    else
    {
        std::cerr << "[ERROR] Índice inválido en memoria secundaria: " << indiceSecundario << ".\n";
    }
}

int MemoriaSecundaria::totalPaginas() const {
    return paginas.size();
}

int& MemoriaSecundaria::obtenerEntrada(int indice) {
    if (indice >= 0 && indice < paginas.size()) {
        return paginas[indice];
    } else {
        throw std::out_of_range("[ERROR] Índice fuera de rango en obtenerEntrada de MemoriaSecundaria.");
    }
}
