#include <iostream>
#include "MMU.h"

int main()
{
    // Inicializar la MMU con tamaño de página de 4 KB, caché de 4 entradas y archivo de memoria secundaria
    MMU mmu(4 * 1024, 4, "memoria_secundaria.txt");

    // Crear tablas de páginas para dos procesos
    TablaDePaginas tablaPaginasProceso1(16); // 16 páginas para el proceso 1
    TablaDePaginas tablaPaginasProceso2(16); // 16 páginas para el proceso 2

    // Crear procesos
    Proceso proceso1(1, {"LOAD 0x0000", "STORE 0x1000", "ADD 0x2000"}, &tablaPaginasProceso1);
    Proceso proceso2(2, {"LOAD 0x3000", "STORE 0x4000", "ADD 0x5000"}, &tablaPaginasProceso2);

    // Agregar procesos a la MMU
    mmu.agregarProceso(&proceso1);
    mmu.agregarProceso(&proceso2);

    // Acceder a direcciones lógicas para simular traducciones
    std::cout << "\n[TEST] Accediendo a direcciones lógicas del Proceso 1...\n";
    for (int i = 0; i < 16; ++i)
    {
        mmu.traducirDireccion(i * 4096, proceso1.obtenerId());
    }

    std::cout << "\n[TEST] Accediendo a direcciones lógicas del Proceso 2...\n";
    for (int i = 0; i < 16; ++i)
    {
        mmu.traducirDireccion(i * 4096, proceso2.obtenerId());
    }

    // Ensuciar páginas del Proceso 1
    std::cout << "\n[TEST] Ensuciando páginas del Proceso 1...\n";
    for (int i = 0; i < 16; ++i)
    {
        tablaPaginasProceso1.establecerSucio(i, true);
    }

    // Forzar fallos de página para ambos procesos
    std::cout << "\n[TEST] Provocando fallos de página con nuevos accesos...\n";
    for (int i = 16; i < 32; ++i)
    {
        mmu.traducirDireccion(i * 4096, proceso1.obtenerId());
        mmu.traducirDireccion(i * 4096, proceso2.obtenerId());
    }

    // Imprimir estado final de las memorias y estadísticas
    std::cout << "\n=== Estado Final ===\n";
    mmu.imprimirEstadoMemorias();

    std::cout << "\n=== Estadísticas ===\n";
    mmu.imprimirEstadisticas();

    return 0;
}
