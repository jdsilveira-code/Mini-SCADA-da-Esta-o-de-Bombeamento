#include <iostream>
#include <random>

using namespace std;

class GeradorAleatorio {
private:
    mt19937 gerador;

public:
    GeradorAleatorio() {
        random_device rd;
        gerador.seed(rd());
    }

    float decimal(float minimo, float maximo) {
        uniform_real_distribution<float> dist(minimo, maximo);
        return dist(gerador);
    }
};