#include "Alarmes.hpp"
#include "Sensores.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

void EstrategiaAlarme::atualizarTimestamp() {
    auto agora = std::chrono::system_clock::now();
    std::time_t tempo = std::chrono::system_clock::to_time_t(agora);
    std::tm tempoUtc{};
    std::tm* resultado = std::gmtime(&tempo);
    if (resultado != nullptr) {
        tempoUtc = *resultado;
    }
    std::ostringstream saida;
    saida << std::put_time(&tempoUtc, "%Y-%m-%dT%H:%M:%SZ");
    Timestamp = saida.str();
}

EstrategiaAlarme::EstrategiaAlarme(int min, int max, std::string tag, std::string Status)
    : LimiteMin(min), LimiteMax(max), Tag(tag), StatusAlarme(Status) {}

EstrategiaAlarme::EstrategiaAlarme(int max, std::string tag)
    : LimiteMin(0), LimiteMax(max), Tag(tag), StatusAlarme("Normal") {}

AlarmeTemperatura::AlarmeTemperatura(int limiteMinimo, int limiteMaximo, std::string tag)
    : EstrategiaAlarme(limiteMinimo, limiteMaximo, tag, "Normal") {}

void AlarmeTemperatura::verificar(Sensor* sensor) {
    if (sensor == nullptr) {
        return;
    }
    atualizarTimestamp();
    int valor = sensor->getValorAtual();
    if (valor < LimiteMin) {
        StatusAlarme = "ALERTA - TEMPERATURA BAIXA";
    } else if (valor > LimiteMax) {
        StatusAlarme = "ALERTA - TEMPERATURA ALTA";
    } else {
        StatusAlarme = "Normal";
    }
}

AlarmeNivel::AlarmeNivel(int limiteMinimo, int limiteMaximo, std::string tag)
    : EstrategiaAlarme(limiteMinimo, limiteMaximo, tag, "Normal") {}

void AlarmeNivel::verificar(Sensor* sensor) {
    if (sensor == nullptr) {
        return;
    }
    atualizarTimestamp();
    int valor = sensor->getValorAtual();
    if (valor < LimiteMin) {
        StatusAlarme = "ALERTA - NIVEL BAIXO";
    } else if (valor > LimiteMax) {
        StatusAlarme = "ALERTA - NIVEL ALTO";
    } else {
        StatusAlarme = "Normal";
    }
}



AlarmeRadiacao::AlarmeRadiacao(int limiteMaximo, std::string tag)
    : EstrategiaAlarme(0, limiteMaximo, tag, "Normal") {}

void AlarmeRadiacao::verificar(Sensor* sensor) {
    if (sensor == nullptr) {
        return;
    }
    atualizarTimestamp();
    int valor = sensor->getValorAtual();
    if (valor > LimiteMax) {
        StatusAlarme = "ALERTA - RADIACAO ALTA";
    } else {
        StatusAlarme = "Normal";
    }
}

AlarmeVazao::AlarmeVazao(int limiteMinimo, int limiteMaximo, std::string tag)
    : EstrategiaAlarme(limiteMinimo, limiteMaximo, tag, "Normal") {}

void AlarmeVazao::verificar(Sensor* sensor) {
    if (sensor == nullptr) {
        return;
    }
    atualizarTimestamp();
    int valor = sensor->getValorAtual();
    if (valor < LimiteMin) {
        StatusAlarme = "ALERTA - VAZAO BAIXA";
    } else if (valor > LimiteMax) {
        StatusAlarme = "ALERTA - VAZAO ALTA";
    } else {
        StatusAlarme = "Normal";
    }
}

