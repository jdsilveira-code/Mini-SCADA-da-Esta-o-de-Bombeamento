#include "Sensores.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

double Sensor::getValorAtual() {
    return ValorAtual;
}

std::string Sensor::getTag() {
    return Tag;
}

std::string Sensor::getUnidadeMedida() {
    return UnidadeMedida;
}

void Sensor::atualizarTimestamp() {
    auto agora = std::chrono::system_clock::now();
    std::time_t tempo = std::chrono::system_clock::to_time_t(agora);
    std::tm tempoUtc{};

#ifdef _WIN32
    gmtime_s(&tempoUtc, &tempo);
#else
    gmtime_r(&tempo, &tempoUtc);
#endif

    std::ostringstream saida;
    saida << std::put_time(&tempoUtc, "%Y-%m-%dT%H:%M:%SZ");
    Timestamp = saida.str();
}

std::string Sensor::getTimestamp() {
    return Timestamp;
}

std::string Sensor::getStatus() {
    return Status;
}

void Sensor::setStatus(std::string novoStatus) {
    Status = novoStatus;
}

SensorNivel::SensorNivel(std::string tag, float max, float min) {
    Tag = tag;
    ValorMax = max;
    ValorMin = min;
    UnidadeMedida = "m";
}

void SensorNivel::ler() {
    ValorAtual = gerador.decimal(ValorMin, ValorMax);
    atualizarTimestamp();
}

SensorRadiacao::SensorRadiacao(std::string tag, float max, float min) {
    Tag = tag;
    ValorMax = max;
    ValorMin = min;
    UnidadeMedida = "W/m2";
}

void SensorRadiacao::ler() {
    ValorAtual = gerador.decimal(ValorMin, ValorMax);
    atualizarTimestamp();
}

SensorTemp::SensorTemp(std::string tag, float max, float min) {
    Tag = tag;
    ValorMax = max;
    ValorMin = min;
    UnidadeMedida = "grC";
}

void SensorTemp::ler() {
    ValorAtual = gerador.decimal(ValorMin, ValorMax);
    atualizarTimestamp();
}

SensorVazao::SensorVazao(std::string tag, float max, float min) {
    Tag = tag;
    ValorMax = max;
    ValorMin = min;
    UnidadeMedida = "m3/h";
}

void SensorVazao::ler() {
    ValorAtual = gerador.decimal(ValorMin, ValorMax);
    atualizarTimestamp();
}
