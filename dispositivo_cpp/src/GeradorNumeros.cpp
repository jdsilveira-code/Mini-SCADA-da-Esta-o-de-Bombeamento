#include "GeradorNumeros.hpp"

GeradorAleatorio::GeradorAleatorio() {
    std::random_device rd;
    gerador.seed(rd());
}

int GeradorAleatorio::decimal(int minimo, int maximo) {
    std::uniform_int_distribution<int> dist(minimo, maximo);
    return dist(gerador);
}
