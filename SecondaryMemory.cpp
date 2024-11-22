#include "SecondaryMemory.h"

// Constructor
SecondaryMemory::SecondaryMemory(const std::string &archivo)
    : archivo(archivo)
{
    // Inicializar tamano leyendo las líneas del archivo
    std::ifstream file(archivo);

    if (!file)
    {
        throw std::runtime_error("[ERROR] No se pudo abrir el archivo: " + archivo);
    }

    int lineCount = 0;
    std::string line;

    while (std::getline(file, line))
    {
        lineCount++;
    }

    tamano = lineCount;
}

std::vector<std::string> SecondaryMemory::leer_pagina(int indice, int cantLineas)
{
    std::vector<std::string> pagina;

    // Verificar que el índice y la cantidad de líneas sean válidos
    if (indice < 0 || cantLineas <= 0 || indice + cantLineas > tamano)
    {
        throw std::runtime_error("[ERROR] Índice de almacenamiento incorrecto o cantidad de líneas es inválida.");
    }

    std::ifstream file(archivo);
    if (!file)
    {
        throw std::runtime_error("[ERROR] No se pudo abrir el archivo para lectura: " + archivo);
    }

    std::string line;
    int currentLine = 0;

    // Leer el archivo línea por línea
    while (std::getline(file, line))
    {
        if (currentLine >= indice && currentLine < indice + cantLineas)
        {
            pagina.push_back(line);
        }
        currentLine++;

        if (currentLine >= indice + cantLineas)
        {
            break;
        }
    }

    if (pagina.size() < static_cast<size_t>(cantLineas))
    {
        throw std::runtime_error("[ERROR] No se pudieron leer todas las líneas solicitadas del archivo.");
    }

    return pagina;
}

bool SecondaryMemory::escribir_pagina(int indice, std::vector<std::string> datos)
{
    if (indice < 0 || indice >= tamano || indice + datos.size() > tamano)
    {
        return false;
    }
    std::ifstream fileIn(archivo);
    if (!fileIn)
    {
        return false;
    }
    std::vector<std::string> contenidoArchivo;
    std::string linea;
    // Leer todo el contenido del archivo en memoria
    while (std::getline(fileIn, linea))
    {
        contenidoArchivo.push_back(linea);
    }
    fileIn.close();
    // Reemplazar las líneas
    for (size_t i = 0; i < datos.size(); ++i)
    {
        contenidoArchivo[indice + i] = datos[i];
    }
    // Escribimos el contenido actualizado de vuelta al archivo
    std::ofstream fileOut(archivo);
    if (!fileOut)
    {
        return false; // [ERROR] 
    }
    for (const auto &line : contenidoArchivo)
    {
        fileOut << line << "\n";
    }
    fileOut.close();
    return true;
}

bool SecondaryMemory::eliminar_pagina(int indice, int cantLineas)
{
    if (indice < 0 || indice >= tamano || indice + cantLineas > tamano || cantLineas <= 0)
    {
        return false; // [ERROR] 
    }
    std::ifstream fileIn(archivo);
    if (!fileIn)
    {
        return false; // [ERROR] 
    }
    std::vector<std::string> contenidoArchivo;
    std::string linea;
    while (std::getline(fileIn, linea))
    {
        contenidoArchivo.push_back(linea);
    }
    fileIn.close();
    // Vaciar las líneas correspondientes
    for (int i = 0; i < cantLineas; ++i)
    {
        contenidoArchivo[indice + i].clear(); // Dejar las líneas vacías
    }
    // Escribimos el contenido actualizado de vuelta al archivo
    std::ofstream fileOut(archivo);
    if (!fileOut)
    {
        return false; // [ERROR] 
    }
    for (const auto &line : contenidoArchivo)
    {
        fileOut << line << "\n";
    }
    fileOut.close();
    return true;
}

std::vector<int> SecondaryMemory::getIndicesMemSecundariaDeProcesos(int idProceso) // Le da a la PageTable todos los indices que necesita en almacenamiento.
{
    std::vector<int> indices;

    std::ifstream file(archivo);
    if (!file)
    {
        throw std::runtime_error("[ERROR] No se pudo abrir el archivo: " + archivo);
    }

    std::string line;
    int currentLine = 0;
    int currentProcess = 0;

    while (std::getline(file, line))
    {
        if (line == "+")
        {
            currentProcess++; // Avanzar al siguiente proceso
            currentLine++;
            continue;
        }

        if (currentProcess == idProceso)
        {
            indices.push_back(currentLine);
        }

        currentLine++;
    }

    if (indices.empty())
    {
        indices.push_back(-1); // No se encontraron líneas para el proceso
    }

    return indices;
}
