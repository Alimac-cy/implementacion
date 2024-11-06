#ifndef CACHE_H
#define CACHE_H

#include <unordered_map>
#include <string>

class Cache {
public:
    Cache(int tamano);
    bool accederPagina(int direccionLogica);
    void cargarPagina(int direccionLogica, const std::string& datos);
    void eliminarPagina();

private:
    int cache_size;
    std::unordered_map<int, std::string> cache;
};

#endif // CACHE_H
