#include "Alarmes.hpp"
#include "Sensores.hpp"

EstrategiaAlarme::EstrategiaAlarme(int min, int max, std::string tag, std::string Status)
    : LimiteMin(min), LimiteMax(max), Tag(tag), StatusAlarme(Status) {}

EstrategiaAlarme::EstrategiaAlarme(int max, std::string tag)
    : LimiteMin(0), LimiteMax(max), Tag(tag), StatusAlarme("Normal") {}

AlarmeTemperatura::AlarmeTemperatura(int limiteMinimo, int limiteMaximo)
    : EstrategiaAlarme(limiteMinimo, limiteMaximo, "TEMP", "Normal") {}

void AlarmeTemperatura::verificar(Sensor* sensor) {
    if (sensor == nullptr) {
        return;
    }
    
    Tag = sensor->getTag();
    int valor = sensor->getValorAtual();
    if (valor < LimiteMin) {
        StatusAlarme = "ALERTA - TEMPERATURA BAIXA";
    } else if (valor > LimiteMax) {
        StatusAlarme = "ALERTA - TEMPERATURA ALTA";
    } else {
        StatusAlarme = "Normal";
    }
}

AlarmeNivel::AlarmeNivel(int limiteMinimo, int limiteMaximo)
    : EstrategiaAlarme(limiteMinimo, limiteMaximo, "NIVEL", "Normal") {}

void AlarmeNivel::verificar(Sensor* sensor) {
    if (sensor == nullptr) {
        return;
    }
    Tag = sensor->getTag();
    int valor = sensor->getValorAtual();
    if (valor < LimiteMin) {
        StatusAlarme = "ALERTA - NÍVEL BAIXO";
    } else if (valor > LimiteMax) {
        StatusAlarme = "ALERTA - NÍVEL ALTO";
    } else {
        StatusAlarme = "Normal";
    }
}



AlarmeRadiacao::AlarmeRadiacao(int limiteMaximo)
    : EstrategiaAlarme(0, limiteMaximo, "RAD", "Normal") {}

void AlarmeRadiacao::verificar(Sensor* sensor) {
    if (sensor == nullptr) {
        return;
    }
    Tag = sensor->getTag();

    int valor = sensor->getValorAtual();
    if (valor > LimiteMax) {
        StatusAlarme = "ALERTA - RADIAÇÃO ALTA";
    } else {
        StatusAlarme = "Normal";
    }
}

AlarmeVazao::AlarmeVazao(int limiteMinimo, int limiteMaximo)
    : EstrategiaAlarme(limiteMinimo, limiteMaximo, "VAZAO", "Normal") {}

void AlarmeVazao::verificar(Sensor* sensor) {
    if (sensor == nullptr) {
        return;
    }
    Tag = sensor->getTag();
    int valor = sensor->getValorAtual();
    if (valor < LimiteMin) {
        StatusAlarme = "ALERTA - VAZÃO BAIXA";
    } else if (valor > LimiteMax) {
        StatusAlarme = "ALERTA - VAZÃO ALTA";
    } else {
        StatusAlarme = "Normal";
    }
}

