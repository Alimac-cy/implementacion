#include <iostream>
#include "MMU.h"

int main()
{
    try
    {
        // Crear un archivo simulado para memoria secundaria
        std::string archivoAlmacenamiento = "almacenamiento.txt";

        // Crear MMU
        MMU mmu(archivoAlmacenamiento);

        // Crear procesos
        Proceso proceso0(0);
        Proceso proceso1(1);

        // Asignar procesos a la MMU
        mmu.asignarProceso(&proceso0);
        mmu.asignarProceso(&proceso1);

        // Simular accesos a direcciones lógicas para cada proceso
        std::cout << "\n[EJECUCIÓN] Acceso a direcciones lógicas del proceso 0:\n";
        int direccionFisica;
        // Proceso 0, ejecucion completa de todas sus instrucciones
        for (int i = 0; i < proceso0.ObtenerTotalInstrucciones(); i++)
        {
            std::cout << "Proceso 0, Dirección Lógica " << i << ":\n";
            direccionFisica = mmu.traducirDireccion(i, 0);
            std::cout << "Dirección Física: " << direccionFisica << "\n";
            // std::cout << "Intruccion especifica: " <<  << "\n";
        }
        std::cout << "\n[EJECUCIÓN] Acceso a direcciones lógicas del proceso 1:\n";
        // Proceso 0, ejecucion completa de todas sus instrucciones
        for (int i = 0; i < proceso1.ObtenerTotalInstrucciones(); i++)
        {
            std::cout << "Proceso 1, Dirección Lógica " << i << ":\n";
            direccionFisica = mmu.traducirDireccion(i, 1);
            std::cout << "Dirección Física: " << direccionFisica << "\n";
            // std::cout << "Intruccion especifica: " <<  << "\n";
        }

        // Proceso 0, Dirección Lógica fuera de rango
        std::cout << "Proceso 0, Dirección Lógica 5 (Fuera de rango):\n";
        direccionFisica = mmu.traducirDireccion(5, 0);
        std::cout << "Dirección Física: " << direccionFisica << "\n";

        // Mostrar el estado final de la memoria principal
        std::cout << "\n[ESTADO FINAL DE LA MEMORIA PRINCIPAL]\n";
        mmu.getMemoriaPrincipal().imprimirEstado();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << "\n";
    }

    return 0;
}
