#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#include <vector>
struct EntradaPagina
{
    int marco;            // Número del marco en memoria física.
    int indiceSecundario; // Índice en memoria secundaria (-1 si no está en memoria secundaria).
    bool valido;          // Indica si la página está cargada en memoria física.
    bool sucio;           // Indica si la página fue modificada.
    bool referenciado;    // Indica si la página fue referenciada recientemente.
};

class TablaDePaginas
{
private:
    int punteroReloj; // Puntero para el algoritmo de reloj.

public:
    std::vector<EntradaPagina> tabla;

    // Constructor
    TablaDePaginas(int numPaginas);

    // Métodos para gestionar entradas
    void actualizarEntrada(int numeroPagina, int marco, bool valido, bool sucio, bool referenciado, int indiceSecundario = -1);
    EntradaPagina obtenerEntrada(int numeroPagina) const;
    void establecerReferencia(int numeroPagina, bool referenciado);
    void establecerSucio(int numeroPagina, bool sucio);

    // Algoritmos de reemplazo
    int buscarReemplazoNRU();   // Para memoria principal
    int buscarReemplazoReloj(); // Para memoria secundaria
    int numPaginas() const;     // Retorna el número total de páginas
};

#endif // PAGE_TABLE_H
