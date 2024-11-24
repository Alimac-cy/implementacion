#ifndef PRIMARY_MEMORY_H
#define PRIMARY_MEMORY_H

#include <vector>
#include <string>
#include <iostream>

class PrimaryMemory
{
private:
    std::vector<std::vector<std::string>> Marcos; // Lista de marcos, cada uno es un conjunto de líneas.
    int Tamano;                                   // Tamaño total de la memoria física (en número de líneas).
    int TamanoFrame;                              // Tamaño de cada marco de página (en número de líneas).

public:
    // Constructor
    PrimaryMemory(int TamanoFrame, int Tamano);

    // Métodos.
    int asignarFrame();                                                      // Asigna un marco de página libre.
    bool liberarFrame(int indice);                                           // Libera un marco de página previamente asignado.
    std::vector<std::string> obtenerFrame(int indice);                       // Devuelve el marco en el índice especificado.
    bool actualizarFrame(int indice, const std::vector<std::string> &datos); // Actualiza el contenido de un marco específico.
    std::string obtenerInstruccion(int direccionFisica);
    bool actualizarInstruccion(int direccionFisica, const std::string &nuevaInstruccion); // actualizar instruccion especifica
    void imprimirEstado() const;
};

#endif // PRIMARY_MEMORY_H
