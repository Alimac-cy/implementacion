#ifndef PROCESS_H
#define PROCESS_H

#include <vector>
#include <string>

enum ProcessState { EXECUTING, WAITING, FINISHED };

class Process {
public:
    Process(int id);
    void cargarInstrucciones(const std::vector<std::string>& instrucciones);
    void ejecutar();

private:
    int id;
    std::vector<std::string> instrucciones;
    ProcessState estado;
    // Tabla de páginas para el proceso
};

#endif // PROCESS_H
