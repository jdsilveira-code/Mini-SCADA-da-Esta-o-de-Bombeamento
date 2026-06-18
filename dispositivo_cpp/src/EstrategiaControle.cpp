#include "EstrategiaControle.hpp"
#include <iostream>

// Construtores da classe base
EstrategiaControle::EstrategiaControle(int min, int max, std::string tag)
    : LimiteMin(min), LimiteMax(max), Tag(std::move(tag)) {}

EstrategiaControle::EstrategiaControle(int max, std::string tag)
    : LimiteMin(0), LimiteMax(max), Tag(std::move(tag)) {}

// Controle de nível: usa valor genérico do sensor e métodos polimórficos do atuador
ControleNivel::ControleNivel(float baixo, float alto)
    : EstrategiaControle(static_cast<int>(baixo), static_cast<int>(alto), "ControleNivel") {
    Modo = "Nivel";
}

void ControleNivel::aplicar(Sensor* sensor, Atuador* atuador) {
    if (!sensor || !atuador) return;

    int valor = sensor->getValorAtual();

    if (valor <= LimiteMin) {
        atuador->setPotencia(75.0f);
        atuador->ligar();
    } else if (valor >= LimiteMax) {
        atuador->desligar();
    } else {
        atuador->setPotencia(50.0f);
    }
}

// Controle de temperatura: usa apenas getValorAtual() e ligar/desligar
ControleTemperatura::ControleTemperatura(float RMax)
    : EstrategiaControle(0, static_cast<int>(RMax), "ControleTemperatura") {
    Modo = "Temperatura";
}

void ControleTemperatura::aplicar(Sensor* sensor, Atuador* atuador) {
    if (!sensor || !atuador) return;

    int tempK = sensor->getValorAtual();
    if (tempK >= LimiteMax) atuador->ligar();
    else atuador->desligar();
}

// Controle de queima: apenas construtor por enquanto
ControleQueima::ControleQueima(float Dose, float qMax)
    : EstrategiaControle(0, static_cast<int>(qMax), "ControleQueima") {
    Modo = "Queima";
    (void)Dose; // placeholder
}

// Nota: a classe ControleQueima não define `aplicar` no header atual.

