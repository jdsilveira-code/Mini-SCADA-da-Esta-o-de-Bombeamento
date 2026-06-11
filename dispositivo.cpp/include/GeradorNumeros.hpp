#ifndef GERADOR_NUMEROS_HPP
#define GERADOR_NUMEROS_HPP

#include <random>

class GeradorAleatorio {
private:
    std::mt19937 gerador;

public:
    GeradorAleatorio() {
        std::random_device rd;
        gerador.seed(rd());
    }

    float decimal(float minimo, float maximo) {
        std::uniform_real_distribution<float> dist(minimo, maximo);
        return dist(gerador);
    }
};

#endif // GERADOR_NUMEROS_HPP
