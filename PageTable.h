#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#include <vector>

class PageTable {
public:
    PageTable(int tamano);
    void agregarEntrada(int marco);
    void eliminarEntrada(int indice);
    int obtenerMarco(int direccionLogica);
    void actualizarValidez(int marco, bool esValido);
    void actualizarReferencia(int marco, bool referencia);
    void actualizarModificacion(int marco, bool modificacion);

private:
    std::vector<int> marcos;
    int tamano;
};

#endif // PAGE_TABLE_H
