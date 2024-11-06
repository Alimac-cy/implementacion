#ifndef PRIMARY_MEMORY_H
#define PRIMARY_MEMORY_H

#include <vector>
#include <string>

class PrimaryMemory {
public:
    PrimaryMemory(int tamano, int tamanoFrame);
    int asignarFrame();
    void liberarFrame(int marco);
    int obtenerFrame(int indice);
    void actualizarFrame(int marco, const std::string& data);

private:
    std::vector<int> marcos;
    int tamano;
    int tamanoFrame;
};

#endif // PRIMARY_MEMORY_H
