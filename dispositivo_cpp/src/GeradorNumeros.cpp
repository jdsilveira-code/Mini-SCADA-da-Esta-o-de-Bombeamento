#include "GeradorNumeros.hpp"

GeradorAleatorio::GeradorAleatorio() {
    std::random_device rd;
    gerador.seed(rd());
}

float GeradorAleatorio::decimal(float minimo, float maximo) {
    std::uniform_real_distribution<float> dist(minimo, maximo);
    return dist(gerador);
}
