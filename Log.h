#ifndef LOG_H
#define LOG_H

#include <vector>
#include <string>

class Log {
public:
    void registrarEvento(int evento);
    void imprimirEstadisticas();

private:
    std::vector<std::string> entradas;
};

#endif // LOG_H
