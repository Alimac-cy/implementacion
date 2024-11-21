#include "MMU.h"
#include "SecondaryMemory.h"
#include "PrimaryMemory.h"
#include "Process.h"
#include "PageTable.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

// Función para inicializar la memoria secundaria con instrucciones de prueba
void inicializarMemoriaSecundaria(const std::string &archivo)
{
    std::ofstream memoriaSecundaria(archivo);
    memoriaSecundaria << "LOAD A, 10\n";
    memoriaSecundaria << "STORE A, 20\n";
    memoriaSecundaria << "ADD B, A\n";
    memoriaSecundaria << "SUB B, 5\n";
    memoriaSecundaria << "MUL C, B\n";
    memoriaSecundaria << "DIV D, C\n";
    memoriaSecundaria << "HALT\n";
    memoriaSecundaria.close();
}

int main()
{
    // Configuración inicial
    const int tamanoPagina = 4 * 1024; // 4 KB por página
    const int tamanoCache = 5;         // Capacidad de caché
    const std::string archivoAlmacenamiento = "memoria_secundaria.txt";

    // Inicializar memoria secundaria con datos de prueba
    inicializarMemoriaSecundaria(archivoAlmacenamiento);

    // Crear la MMU
    MMU mmu(tamanoPagina, tamanoCache, archivoAlmacenamiento);

    // Crear procesos con tablas de páginas
    TablaDePaginas tablaProceso1(10); // 10 páginas para el proceso 1
    Proceso proceso1(1, &tablaProceso1);
    mmu.agregarProceso(&proceso1);

    TablaDePaginas tablaProceso2(8); // 8 páginas para el proceso 2
    Proceso proceso2(2, &tablaProceso2);
    mmu.agregarProceso(&proceso2);

    // Revisar las direcciones lógicas del Proceso 1
    std::cout << "[TEST] Revisando direcciones lógicas del Proceso 1...\n";
    for (int direccionLogica = 0; direccionLogica < 10 * tamanoPagina; direccionLogica += tamanoPagina / 4)
    {
        int direccionFisica = mmu.traducirDireccion(direccionLogica, 1);
        if (direccionFisica != -1)
        {
            // Leer datos simulados desde la memoria principal
            int marco = direccionFisica / tamanoPagina;
            std::string datos = mmu.getMemoriaPrincipal().leerMarco(marco);
            std::cout << "Dirección lógica: " << direccionLogica
                      << " -> Dirección física: " << direccionFisica
                      << " (Marco: " << marco << ")"
                      << " | Instrucción: " << datos << "\n";
        }
        else
        {
            std::cout << "Fallo en la traducción de la dirección lógica: " << direccionLogica << "\n";
        }
    }

    // Revisar las direcciones lógicas del Proceso 2
    std::cout << "\n[TEST] Revisando direcciones lógicas del Proceso 2...\n";
    for (int direccionLogica = 0; direccionLogica < 8 * tamanoPagina; direccionLogica += tamanoPagina / 4)
    {
        int direccionFisica = mmu.traducirDireccion(direccionLogica, 2);
        if (direccionFisica != -1)
        {
            int marco = direccionFisica / tamanoPagina;
            std::string datos = mmu.getMemoriaPrincipal().leerMarco(marco);
            std::cout << "Dirección lógica: " << direccionLogica
                      << " -> Dirección física: " << direccionFisica
                      << " (Marco: " << marco << ")"
                      << " | Instrucción: " << datos << "\n";
        }
        else
        {
            std::cout << "Fallo en la traducción de la dirección lógica: " << direccionLogica << "\n";
        }
    }

    // Estado final de la memoria principal
    std::cout << "\n[FINAL] Estado de la Memoria Principal:\n";
    mmu.imprimirEstadoMemorias();
    mmu.imprimirEstadisticas();
    return 0;
}
