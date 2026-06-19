#include "Sensores.hpp"
#include "AtuadorInterfaces.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cmath>

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

    std::tm* resultado = std::gmtime(&tempo);
    if (resultado != nullptr) {
        tempoUtc = *resultado;
    }

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

void Sensor::calibrar() {
    // implementação padrão de calibração (subclasses podem sobrepor)
}

SensorNivel::SensorNivel(std::string tag, int max, int min) {
    Tag = tag;
    ValorMax = max;
    ValorMin = min;
    UnidadeMedida = "m";
}

void SensorNivel::ler() {
    AlturaAtual = gerador.decimal(ValorMin, ValorMax);
    atualizarTimestamp();
}

int SensorNivel::getValorAtual() const {
    return AlturaAtual;
}

void SensorNivel::CalcularVolume(int altura, float raio) {
    AlturaAtual = altura;
    RaioTanque = raio;
    const double pi = 3.14159265358979323846;
    VolumeTotalMax = static_cast<float>(pi * raio * raio * altura);
}

SensorRadiacao::SensorRadiacao(std::string tag, int max, int min, IVaretasState* atuador) {
    Tag = tag;
    ValorMax = max;
    ValorMin = min;
    UnidadeMedida = "mSv/h";
    DoseAcumulada = 0.0f;
    LimiteDoseAcumulada = 0.0f;
    varetasState = atuador;
    Status = (varetasState != nullptr) ? varetasState->isLigado() : false;
}

void SensorRadiacao::ler() {
    NivelRadiacaoAtual = gerador.decimal(ValorMin, ValorMax);
    atualizarTimestamp();
    AcumularDose(NivelRadiacaoAtual, Timestamp);
}

int SensorRadiacao::getValorAtual() const {
    return NivelRadiacaoAtual;
}

void SensorRadiacao::AcumularDose(int nivel, std::string timestamp) {
    Timestamp = timestamp;
    // Atualiza status consultando o estado atual das varetas
    if (varetasState != nullptr) {
        Status = varetasState->isLigado();
    }

    float delta = static_cast<float>(nivel) / 2.0f;
    if (Status) {
        // Varetas RETIRADAS (reactor funcionando): acumula dose
        DoseAcumulada += delta;
    } else {
        // Varetas INSERIDAS (reactor parado): reduz a dose (dissipação)
        DoseAcumulada -= delta;
    }

    // Não permitir dose negativa
    if (DoseAcumulada < 0.0f) DoseAcumulada = 0.0f;
}

float SensorRadiacao::getDoseAcumulada() const {
    return DoseAcumulada;
}

SensorTemp::SensorTemp(std::string tag, int max, int min) {
    Tag = tag;
    ValorMax = max;
    ValorMin = min;
    UnidadeMedida = "K";
}

void SensorTemp::ler() {
    TempKelvin = gerador.decimal(ValorMin, ValorMax);
    atualizarTimestamp();
}

int SensorTemp::getValorAtual() const {
    return TempKelvin;
}

void SensorTemp::ConverterKelvinCelsius(int tempKelvin) {
    TempCelsius = tempKelvin - 273.15f;
    atualizarTimestamp();
}

SensorVazao::SensorVazao(std::string tag, int max, int min) {
    Tag = tag;
    ValorMax = max;
    ValorMin = min;
    UnidadeMedida = "m3/h";
    VazaoAcumulada = 0.0f;
}

void SensorVazao::ler() {
    VazaoAtual = gerador.decimal(ValorMin, ValorMax);
    atualizarTimestamp();
}

int SensorVazao::getValorAtual() const {
    return VazaoAtual;
}

void SensorVazao::AcumularVazao(int vazao, std::string timestamp) {
    VazaoAcumulada += vazao;
    Timestamp = timestamp;
}
