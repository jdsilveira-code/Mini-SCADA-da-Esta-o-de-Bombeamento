#ifndef GERADOR_NUMEROS_HPP
#define GERADOR_NUMEROS_HPP

#include <random>

class GeradorAleatorio {
private:
    std::mt19937 gerador;

public:
    GeradorAleatorio();
    int decimal(int minimo, int maximo);
};

#endif // GERADOR_NUMEROS_HPP
