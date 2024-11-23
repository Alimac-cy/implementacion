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
        Proceso proceso2(2);
        Proceso proceso3(3);
        Proceso proceso4(4);
        Proceso proceso5(5);
        Proceso proceso6(6);
        Proceso proceso7(7);
        Proceso proceso8(8);
        Proceso proceso9(9);
        Proceso proceso10(10);
        Proceso proceso11(11);
        Proceso proceso12(12);
        Proceso proceso13(13);
        Proceso proceso14(14);

        // Asignar procesos a la MMU
        mmu.asignarProceso(&proceso0);
        mmu.asignarProceso(&proceso1);
        mmu.asignarProceso(&proceso2);
        mmu.asignarProceso(&proceso3);
        mmu.asignarProceso(&proceso4);
        mmu.asignarProceso(&proceso5);
        mmu.asignarProceso(&proceso6);
        mmu.asignarProceso(&proceso7);
        mmu.asignarProceso(&proceso8);
        mmu.asignarProceso(&proceso9);
        mmu.asignarProceso(&proceso10);
        mmu.asignarProceso(&proceso11);
        mmu.asignarProceso(&proceso12);
        mmu.asignarProceso(&proceso13);
        mmu.asignarProceso(&proceso14);

        std::cout << "\n[PROCESAMIENTO] 16 Procesos actuales por lo que el frame limit es: 1 para cada proceso\n";

        // Simular accesos a direcciones lógicas para cada proceso
        std::cout << "\n[EJECUCIÓN] Acceso a direcciones lógicas del proceso 0:\n";
        int direccionFisica;
        // Proceso 0, ejecucion completa de todas sus instrucciones
        for (int i = 0; i < proceso0.ObtenerTotalInstrucciones(); i++)
        {
            std::cout << "Proceso 0, Dirección Lógica " << i << ":\n";
            direccionFisica = mmu.traducirDireccion(i, 0);
            std::cout << "Dirección Física: " << direccionFisica << "\n";
            std::string instruccion = mmu.getMemoriaPrincipal().obtenerInstruccion(direccionFisica);
            std::cout << "Instrucción específica: " << instruccion << "\n\n";
        }

        std::cout << "\n[EJECUCIÓN] Acceso a direcciones lógicas del proceso 1:\n";
        // Proceso 0, ejecucion completa de todas sus instrucciones
        for (int i = 0; i < proceso1.ObtenerTotalInstrucciones(); i++)
        {
            std::cout << "Proceso 1, Dirección Lógica " << i << ":\n";
            direccionFisica = mmu.traducirDireccion(i, 1);
            std::cout << "Dirección Física: " << direccionFisica << "\n";

            std::string instruccion = mmu.getMemoriaPrincipal().obtenerInstruccion(direccionFisica);
            std::cout << "Instrucción específica: " << instruccion << "\n\n";
        }

        // Proceso 0, Dirección Lógica fuera de rango
        std::cout << "Proceso 1, Dirección Lógica 5 (Fuera de rango):\n";
        direccionFisica = mmu.traducirDireccion(5, 1);
        std::cout << "Dirección Física: " << direccionFisica << "\n";

        // Mostrar el estado final de la memoria principal
        std::cout << "\n[ESTADO FINAL DE LA MEMORIA PRINCIPAL]\n";
        mmu.getMemoriaPrincipal().imprimirEstado();
        
        std::cout << "\nLiberar proceso 0:\n";
        mmu.liberarProceso(proceso0.obtenerId());


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
