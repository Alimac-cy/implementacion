#ifndef INSTRUCTION_SET_H
#define INSTRUCTION_SET_H

#include <vector>
#include <string>

class InstructionSet {
public:
    InstructionSet(const std::vector<std::string>& instrucciones);
    std::string obtenerInstruccion(int indice);

private:
    std::vector<std::string> instrucciones;
};

#endif // INSTRUCTION_SET_H
