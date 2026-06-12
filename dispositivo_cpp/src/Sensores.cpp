#include "Sensores.hpp"

double Sensor::getValorAtual() {
    return ValorAtual;
}

SensorNivel::SensorNivel(std::string tag, float max, float min) {
    Tag = tag;
    ValorMax = max;
    ValorMin = min;
    UnidadeMedida = "m";
}

void SensorNivel::ler() {
    ValorAtual = gerador.decimal(ValorMin, ValorMax);
}

SensorRadiacao::SensorRadiacao(std::string tag, float max, float min) {
    Tag = tag;
    ValorMax = max;
    ValorMin = min;
    UnidadeMedida = "W/m2";
}

void SensorRadiacao::ler() {
    ValorAtual = gerador.decimal(ValorMin, ValorMax);
}

SensorTemp::SensorTemp(std::string tag, float max, float min) {
    Tag = tag;
    ValorMax = max;
    ValorMin = min;
    UnidadeMedida = "grC";
}

void SensorTemp::ler() {
    ValorAtual = gerador.decimal(ValorMin, ValorMax);
}

SensorVazao::SensorVazao(std::string tag, float max, float min) {
    Tag = tag;
    ValorMax = max;
    ValorMin = min;
    UnidadeMedida = "m3/h";
}

void SensorVazao::ler() {
    ValorAtual = gerador.decimal(ValorMin, ValorMax);
}
