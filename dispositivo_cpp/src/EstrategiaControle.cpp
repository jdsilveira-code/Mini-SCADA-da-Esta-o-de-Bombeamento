#include "EstrategiaControle.hpp"

ControleNivel::ControleNivel(float baixo, float alto) : nivelBaixo(baixo), nivelAlto(alto) {}

void ControleNivel::aplicar(Sensor* sensor, Atuador* atuador) {
    float nivel = sensor->getValorAtual();

    if (nivel < nivelBaixo) {
        atuador->ligar();
    } else if (nivel > nivelAlto) {
        atuador->desligar();
    }
}

ControleTemperatura::ControleTemperatura(float RMax) : RadMax(RMax) {}

void ControleTemperatura::aplicar(Sensor* sensor, Atuador* atuador) {
    float temp = sensor->getValorAtual();

    if (temp > RadMax) {
        atuador->desligar();
    } else {
        atuador->ligar();
    }
}
