// main.cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "MMU.h"
#include "Process.h"

int main()
{
    try
    {
        std::string archivoAlmacenamiento = "almacenamiento.txt";
        MMU mmu(archivoAlmacenamiento);
        Proceso proceso0(0); // ID del proceso = 0
        Proceso proceso1(1); // ID del proceso = 1
        mmu.asignarProceso(&proceso0);
        mmu.asignarProceso(&proceso1);
        std::cout << "\n[PROCESAMIENTO] 2 Procesos asignados, frame limit ajustado.\n";
        std::cout << "\n[EJECUCIÓN] Acceso a direcciones lógicas del proceso 0:\n";
        int direccionFisica;
        for (int i = 0; i < proceso0.ObtenerTotalInstrucciones(); i++)
        {
            direccionFisica = mmu.traducirDireccion(i, proceso0.obtenerId());
            if (direccionFisica != -1)
            {
                std::cout << "Dirección Física: " << direccionFisica << "\n";
                std::string instruccion = mmu.getMemoriaPrincipal().obtenerInstruccion(direccionFisica);
                std::cout << "Instrucción específica: " << instruccion << "\n";
            }
            else
            {
                std::cout << "Error en la traducción de dirección.\n\n";
            }
        }
        mmu.getMemoriaPrincipal().imprimirEstado();
        std::cout << "\n[EJECUCIÓN] Acceso a direcciones lógicas del proceso 1:\n";
        for (int i = 0; i < proceso1.ObtenerTotalInstrucciones(); i++)
        {
            direccionFisica = mmu.traducirDireccion(i, proceso1.obtenerId());
            if (direccionFisica != -1)
            {
                std::cout << "Dirección Física: " << direccionFisica << "\n";
                std::string instruccion = mmu.getMemoriaPrincipal().obtenerInstruccion(direccionFisica);
                std::cout << "Instrucción específica: " << instruccion << "\n";
            }
            else
            {
                std::cout << "Error en la traducción de dirección.\n\n";
            }
        }
        mmu.getMemoriaPrincipal().imprimirEstado();
        // Modificar proceso0, direcciones lógicas 2 y 5
        int direccionesAModificarProceso0[] = {2, 5};
        for (int dirLogica : direccionesAModificarProceso0)
        {
            std::cout << "\nModificando instrucción en dirección lógica del proceso 0: " << dirLogica << "\n";
            if (mmu.modificarInstruccion(proceso0.obtenerId(), dirLogica, "P0_INSTRUCCION_MOD_" + std::to_string(dirLogica)))
            {
                std::cout << "Instrucción en dirección lógica " << dirLogica << " modificada.\n";
            }
            else
            {
                std::cout << "[ERROR] No se pudo modificar la instrucción.\n\n";
            }
        }
        // Modificar proceso1, dirección lógica 6
        int direccionesAModificarProceso1[] = {6};
        for (int dirLogica : direccionesAModificarProceso1)
        {
            std::cout << "Modificando instrucción en dirección lógica del proceso 1: " << dirLogica << "\n";
            if (mmu.modificarInstruccion(proceso1.obtenerId(), dirLogica, "P1_INSTRUCCION_MOD_" + std::to_string(dirLogica)))
            {
                std::cout << "Instrucción en dirección lógica " << dirLogica << " modificada.\n";
            }
            else
            {
                std::cout << "[ERROR] No se pudo modificar la instrucción.\n\n";
            }
        }
        std::cout << "\n[ESTADO DE LA MEMORIA PRINCIPAL ANTES de liberar los procesos]\n";
        mmu.getMemoriaPrincipal().imprimirEstado();
        //Liberar los procesos, lo que debería escribir las páginas modificadas de vuelta al almacenamiento secundario
        std::cout << "\nLiberar proceso 0:\n";
        mmu.liberarProceso(proceso0.obtenerId());
        std::cout << "\nLiberar proceso 1:\n";
        mmu.liberarProceso(proceso1.obtenerId());

        std::cout << "\n[ESTADO FINAL DE LA MEMORIA PRINCIPAL]\n";
        mmu.getMemoriaPrincipal().imprimirEstado();

        // Verificar que las páginas modificadas se han escrito correctamente en el almacenamiento secundario
        std::ifstream archivoVerificacion(archivoAlmacenamiento);
        if (!archivoVerificacion)
        {
            std::cerr << "[ERROR] No se pudo abrir el archivo de almacenamiento secundario para verificación.\n";
            return 1;
        }

        std::cout << "\nContenido del archivo de almacenamiento secundario después de modificaciones:\n";
        std::string linea;
        int numLinea = 0;
        while (std::getline(archivoVerificacion, linea))
        {
            std::cout << "Línea " << numLinea << ": " << linea << "\n";
            numLinea++;
        }
        archivoVerificacion.close();
    }
    catch (const std::exception &e)
    {
        std::cerr << "[EXCEPCIÓN] " << e.what() << "\n";
    }

    return 0;
}
