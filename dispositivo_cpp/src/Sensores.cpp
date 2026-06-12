#include "sensores.hpp"

double Sensor::getValorAtual(){
    return ValorAtual;
}

SensorNivel::SensorNivel(std::string tag,float max, float min) {
    Tag = tag;
    ValorMax = max;
    ValorMin = min;
    UnidadeMedida = "m";
}

void SensorNivel::ler(){
    ValorAtual = gerador.decimal(ValorMin, ValorMax);
}