#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H
#include <vector>
struct Pagina
{
    int marco;            // Número del marco en memoria física.
    int indiceSecundario; // Índice en memoria secundaria (-1 si no está en memoria secundaria).
    bool valido;          // Indica si la página está cargada en memoria principal.
    bool modificado;      // Indica si la página fue modificada.
    bool referenciado;    // Indica si la página fue referenciada recientemente.
    int contador;         // Contador para reiniciar bits periódicamente
};

class PageTable
{
private:
    int numPaginas;

    // Método para resetear bits cuando el contador supera el umbral
    void reiniciarBits();

public:
    // Constructor
    PageTable(int numPaginas);
    std::vector<Pagina> tabla;

    // Metodos
    bool AgregarEntrada(int numeroPag, int marco, int indiceSecundario, bool valido);
    bool EliminarEntrada(int numeroPag);
    int ObtenerMarco(int numeroPag);
    int indiceSecundario(int numeroPag);
    void ActualizarFrame(int numeroPag, int marco);
    void ActualizarValidez(int numeroPag, bool validez);
    void ActualizarReferencia(int numeroPag, bool referencia);
    void ActualizarModificacion(int numeroPag, bool modificado);
    int ObtenerNumPaginas() const;
    bool EstaSecundario(int numeroPag) const;
    bool EstaModificado(int numeroPag) const;

    // Algoritmos de reemplazo
    int buscarReemplazoNRU(); // Para memoria principal

    // Incrementar contador para todas las páginas válidas
    void incrementarContadores();
};
#endif // PAGE_TABLE_H
