#include <iostream>
#include <vector>
#include <string>

class Log {
private:
    struct Evento {
        std::string descripcion; // Descripción del evento.
    };

    std::vector<Evento> entradas; // Registro de eventos.
    int hits;                     // Cantidad de hits (accesos exitosos a la memoria).
    int misses;                   // Cantidad de misses (fallos de memoria).

public:
    Log() : hits(0), misses(0) {}

    void registrarEvento(const std::string& descripcion) {
        entradas.push_back({ descripcion });
    }

    void registrarHit() {
        hits++;
        registrarEvento("Se registró un hit en memoria.");
    }

    void registrarMiss() {
        misses++;
        registrarEvento("Se registró un miss en memoria.");
    }

    void imprimirEstadisticas() const {
        std::cout << "Estadísticas del sistema:\n";
        std::cout << "Hits: " << hits << "\n";
        std::cout << "Misses: " << misses << "\n";
        std::cout << "Eventos registrados:\n";
        for (const auto& evento : entradas) {
            std::cout << "- " << evento.descripcion << "\n";
        }
    }
};
