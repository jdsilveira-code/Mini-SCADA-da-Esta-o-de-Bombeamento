include "EstrategiaControle.hpp"
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
    SensorNivel* sNivel = dynamic_cast<SensorNivel*>(sensor);
    if (!sNivel) return;

    float nivel = sNivel->getValorAtual();
    SensorNivel* sNivel = dynamic_cast<SensorNivel*>(sensor);
    if (!sNivel) return;

    float nivel = sNivel->getValorAtual();

    if (nivel <= LimiteMin) {
        atuador->setPotencia(100f);
        atuador->ligar();
    } else if (nivel >= LimiteMax) {
        atuador->desligar();
    } else {
        // controle proporcional: potência entre 0% e 100% dependendo da posição do nível
        float potencia = ((nivel - LimiteMin) / (LimiteMax - LimiteMin)) * 100.0f;
        atuador->setPotencia(potencia);
        atuador->ligar();
    }
}

// Controle de temperatura: usa apenas getValorAtual() e ligar/desligar
ControleTemperatura::ControleTemperatura(float RMax)
    : EstrategiaControle(0, static_cast<int>(RMax), "ControleTemperatura") {
    Modo = "Temperatura";
}
// Controle de temperatura: usa apenas getValorAtual() e ligar/desligar
ControleTemperatura::ControleTemperatura(float RMax)
    : EstrategiaControle(0, static_cast<int>(RMax), "ControleTemperatura") {
    Modo = "Temperatura";
}

void ControleTemperatura::aplicar(Sensor* sensor, Atuador* atuador) {
    SensorTemp* sTemp = dynamic_cast<SensorTemp*>(sensor);
    if (!sTemp) return;

    float temp = sTemp->getValorAtual();
    SensorTemp* sTemp = dynamic_cast<SensorTemp*>(sensor);
    if (!sTemp) return;

    float temp = sTemp->getValorAtual();

    if (temp > RadMax) {
        atuador->desligar();
    } else {
        atuador->ligar();
    }
}

ControleQueima::ControleQueima(float DoseMax, SensorTemp* sTemp, float tempMax)
    : DoseAcumuladaMin(-1), DoseAcumuladaMax(DoseMax), TempMax(tempMax), SensorTemperatura(sTemp) {}

ControleQueima::ControleQueima(float DoseMin, float DoseMax, SensorTemp* sTemp, float tempMax)
    : DoseAcumuladaMin(DoseMin), DoseAcumuladaMax(DoseMax), TempMax(tempMax), SensorTemperatura(sTemp) {}

void ControleQueima::aplicar(Sensor* sensor, Atuador* atuador) {
    SensorRadiacao* sRad = dynamic_cast<SensorRadiacao*>(sensor);
    Varetas* varetas     = dynamic_cast<Varetas*>(atuador);
    if (!sRad || !varetas) return;

    float dose = sRad->getDoseAcumulada();

    if (dose >= DoseAcumuladaMax) {
        // dose crítica: inserir varetas completamente (100% = reator "desligado")
        varetas->AjustarQueima(100);
        return;
    }

    if (DoseAcumuladaMin >= 0 && dose < DoseAcumuladaMin) {
        // dose baixa: só retira varetas (mais radiação) se temperatura permitir
        float temp = SensorTemperatura->getValorAtual();
        if (temp < TempMax) {
            varetas->AjustarQueima(0);
        }
    }
}

ControleQueima::ControleQueima(float DoseMax, SensorTemp* sTemp, float tempMax)
    : DoseAcumuladaMin(-1), DoseAcumuladaMax(DoseMax), TempMax(tempMax), SensorTemperatura(sTemp) {}

ControleQueima::ControleQueima(float DoseMin, float DoseMax, SensorTemp* sTemp, float tempMax)
    : DoseAcumuladaMin(DoseMin), DoseAcumuladaMax(DoseMax), TempMax(tempMax), SensorTemperatura(sTemp) {}

void ControleQueima::aplicar(Sensor* sensor, Atuador* atuador) {
    SensorRadiacao* sRad = dynamic_cast<SensorRadiacao*>(sensor);
    Varetas* varetas     = dynamic_cast<Varetas*>(atuador);
    if (!sRad || !varetas) return;

    float dose = sRad->getDoseAcumulada();

    if (dose >= DoseAcumuladaMax) {
        // dose crítica: inserir varetas completamente (100% = reator "desligado")
        varetas->AjustarQueima(100);
        return;
    }

    if (DoseAcumuladaMin >= 0 && dose < DoseAcumuladaMin) {
        // dose baixa: só retira varetas (mais radiação) se temperatura permitir
        float temp = SensorTemperatura->getValorAtual();
        if (temp < TempMax) {
            varetas->AjustarQueima(0);
        }
    }
}
