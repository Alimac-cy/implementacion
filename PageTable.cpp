#include "PageTable.h"

PageTable::PageTable(int numPaginas) : numPaginas(numPaginas)
{
    tabla.resize(numPaginas, {-1, -1, false, false, false, 0});
}

bool PageTable::agregarEntrada(int numeroPag, int marco, int indiceSecundario, bool valido, int instruccionesEnPagina)
{

    if (numeroPag < 0 || numeroPag >= tabla.size())
    {
        return false;
    }

    tabla[numeroPag] = {marco, indiceSecundario, valido, false, false, 0, instruccionesEnPagina};
    return true;
}

bool PageTable::eliminarEntrada(int numeroPag)
{
    if (numeroPag < 0 || numeroPag >= tabla.size())
    {
        return false;
    }

    tabla[numeroPag] = {-1, -1, false, false, false};
    return true;
}

int PageTable::obtenerMarco(int numeroPag)
{
    if (numeroPag < 0 || numeroPag >= tabla.size())
        return -1; // Retornar una entrada inválida si el índice está fuera de rango

    return tabla[numeroPag].marco;
}

int PageTable::indiceSecundario(int numeroPag)
{
    if (numeroPag < 0 || numeroPag >= tabla.size())
        return -1; // Retornar una entrada inválida si el índice está fuera de rango

    return tabla[numeroPag].indiceSecundario;
}

void PageTable::actualizarValidez(int numeroPag, bool validez)
{
    if (numeroPag < 0 || numeroPag >= tabla.size())
        return;

    tabla[numeroPag].valido = validez;

    if (validez)
    {
        tabla[numeroPag].contador = 0; // Reiniciar contador cuando la página es válida
    }
}

void PageTable::actualizarReferencia(int numeroPag, bool referencia)
{
    if (numeroPag < 0 || numeroPag >= tabla.size())
        return;

    tabla[numeroPag].referenciado = referencia;
    tabla[numeroPag].contador = 0;
}

void PageTable::actualizarModificacion(int numeroPag, bool modificado)
{
    if (numeroPag < 0 || numeroPag >= tabla.size())
        return;

    tabla[numeroPag].modificado = modificado;
    tabla[numeroPag].contador = 0;
}

int PageTable::obtenerNumPaginas() const
{
    return numPaginas;
}

bool PageTable::estaSecundario(int numeroPag) const
{
    return tabla[numeroPag].valido;
}

int PageTable::buscarReemplazoNRU()
{
    // Clase 00: no referenciado, no modificado
    for (int i = 0; i < tabla.size(); ++i)
    {
        if (!tabla[i].referenciado && !tabla[i].modificado && tabla[i].valido)
        {
            incrementarContadores();
            return i;
        }
    }
    // Clase 01: no referenciado, modificado
    for (int i = 0; i < tabla.size(); ++i)
    {
        if (!tabla[i].referenciado && tabla[i].modificado && tabla[i].valido)
        {
            incrementarContadores();
            return i;
        }
    }
    // Clase 10: referenciado, no modificado
    for (int i = 0; i < tabla.size(); ++i)
    {
        if (tabla[i].referenciado && !tabla[i].modificado && tabla[i].valido)
        {
            incrementarContadores();
            return i;
        }
    }
    // Clase 11: referenciado, modificado
    for (int i = 0; i < tabla.size(); ++i)
    {
        if (tabla[i].referenciado && tabla[i].modificado && tabla[i].valido)
        {
            incrementarContadores();
            return i;
        }
    }
    return -1; // No se encontró una página para reemplazo
}

void PageTable::reiniciarBits()
{
    for (auto &entrada : tabla)
    {
        if (entrada.valido && entrada.contador >= 10)
        {
            entrada.referenciado = false;
            entrada.modificado = false;
            entrada.contador = 0; // Reiniciar contador después del reseteo
        }
    }
}

void PageTable::incrementarContadores()
{
    for (auto &entrada : tabla)
    {
        if (entrada.valido)
        {
            entrada.contador++;
        }
    }
    reiniciarBits();
}

bool PageTable::estaModificado(int numeroPag) const
{
    return tabla[numeroPag].modificado;
}

void PageTable::actualizarFrame(int numeroPag, int marco)
{
    tabla[numeroPag].marco = marco;
}

int PageTable::obtenerInstruccionesEnPagina(int numeroPag) const
{
    if (numeroPag < 0 || numeroPag >= tabla.size())
        return 0;
    return tabla[numeroPag].instruccionesEnPagina;
}


