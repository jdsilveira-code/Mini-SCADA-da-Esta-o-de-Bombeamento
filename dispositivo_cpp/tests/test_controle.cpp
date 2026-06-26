// TESTES PARA ESTRATEGIA DE CONTROLE
#include <iostream>
#include <cassert>
#include <cmath>
#include <algorithm>
#include "EstrategiaControle.hpp"
#include "Sensores.hpp"
#include "Atuadores.hpp"

// MOCK para SensorNivel
class SensorNivelMock : public SensorNivel {
private:
    int nivelForcado;
public:
    SensorNivelMock(std::string tag, int max, int min, int nivelInicial)
        : SensorNivel(tag, max, min), nivelForcado(nivelInicial) {}

    void ler() override {}
    int getValorAtual() const override { return nivelForcado; }
    void setNivel(int novoNivel) { nivelForcado = novoNivel; }
};

// MOCK para SensorTemp
class SensorTempMock : public SensorTemp {
private:
    int tempForcada;
public:
    SensorTempMock(std::string tag, int max, int min, int tempInicial)
        : SensorTemp(tag, max, min), tempForcada(tempInicial) {}

    void ler() override {}
    int getValorAtual() const override { return tempForcada; }
    void setTemp(int novaTemp) { tempForcada = novaTemp; }
};

// MOCK para SensorRadiacao (com override pois getDoseAcumulada é virtual)
class SensorRadiacaoMock : public SensorRadiacao {
private:
    int radiacaoForcada;
    float doseAcumuladaForcada;
public:
    SensorRadiacaoMock(std::string tag, int max, int min, IVaretasState* atuador, int radInicial, float doseInicial)
        : SensorRadiacao(tag, max, min, atuador), radiacaoForcada(radInicial), doseAcumuladaForcada(doseInicial) {}

    void ler() override {}
    int getValorAtual() const override { return radiacaoForcada; }
    float getDoseAcumulada() const override { return doseAcumuladaForcada; }

    void setRadiacao(int novaRad) { radiacaoForcada = novaRad; }
    void setDoseAcumulada(float novaDose) { doseAcumuladaForcada = novaDose; }
};

int main() {
    // TESTE: ControleNivel - sempre liga
    {
        SensorNivelMock sensor("SNV-01", 100, 0, 10);
        BombaAgua bomba("BOMBA-01");
        ControleNivel estrategia(20.0f, 80.0f);

        estrategia.aplicar(&sensor, &bomba);
        assert(bomba.isLigado() == true);

        sensor.setNivel(50);
        estrategia.aplicar(&sensor, &bomba);
        assert(bomba.isLigado() == true);

        sensor.setNivel(90);
        estrategia.aplicar(&sensor, &bomba);
        assert(bomba.isLigado() == true);
    }

    // TESTE: ControleTemperatura
    {
        SensorTempMock sensor("STM-01", 400, 250, 300);
        BombaAgua bomba("BOMBA-02");
        ControleTemperatura estrategia(350.0f);

        estrategia.aplicar(&sensor, &bomba);
        assert(bomba.isLigado() == false);

        sensor.setTemp(380);
        estrategia.aplicar(&sensor, &bomba);
        assert(bomba.isLigado() == true);

        sensor.setTemp(350);
        estrategia.aplicar(&sensor, &bomba);
        assert(bomba.isLigado() == false);
    }

    // TESTE: ControleQueima - construtor (dose, qMax)
    {
        SensorRadiacaoMock sensorRad("SRD-01", 100, 0, nullptr, 30, 60.0f);
        Varetas varetas("VAR-01");
        ControleQueima estrategia(50.0f, 150.0f);

        // Dose 60 >= 50 -> ajusta 100%
        estrategia.aplicar(&sensorRad, &varetas);
        assert(std::abs(varetas.getValorAtual() - 100.0f) < 0.1f);
        assert(varetas.isLigado() == false);

        // Dose 40 < 50 -> não altera (mantém 0)
        sensorRad.setDoseAcumulada(40.0f);
        varetas.AjustarQueima(0.0f);
        estrategia.aplicar(&sensorRad, &varetas);
        assert(std::abs(varetas.getValorAtual() - 0.0f) < 0.1f);
    }

    // TESTE: ControleQueima - construtor completo com SensorTemp
    {
        SensorTempMock sensorTemp("STM-02", 600, 300, 400);
        SensorRadiacaoMock sensorRad("SRD-02", 100, 0, nullptr, 25, 80.0f);
        Varetas varetas("VAR-02");
        ControleQueima estrategia(40.0f, 160.0f, &sensorTemp, 500.0f);

        // Teste 1: dose=30 (<min), temp=400 (<max) -> ajusta 0%
        sensorRad.setDoseAcumulada(30.0f);
        sensorTemp.setTemp(400);
        varetas.AjustarQueima(100.0f);
        estrategia.aplicar(&sensorRad, &varetas);
        assert(std::abs(varetas.getValorAtual() - 0.0f) < 0.1f);

        // Teste 2: dose=170 (>max) -> ajusta 100% (ignora temp)
        sensorRad.setDoseAcumulada(170.0f);
        sensorTemp.setTemp(520); // temp > max, mas deve ajustar 100 mesmo assim
        varetas.AjustarQueima(0.0f);
        estrategia.aplicar(&sensorRad, &varetas);
        assert(std::abs(varetas.getValorAtual() - 100.0f) < 0.1f);

        // Teste 3: dose=80 (entre), temp=520 (>max) -> não altera (mantém 50)
        sensorRad.setDoseAcumulada(80.0f);
        sensorTemp.setTemp(520);
        varetas.AjustarQueima(50.0f);
        estrategia.aplicar(&sensorRad, &varetas);
        assert(std::abs(varetas.getValorAtual() - 50.0f) < 0.1f);

        // Teste 4: dose=80 (entre), temp=400 (<max) -> não altera (mantém 50)
        sensorTemp.setTemp(400);
        varetas.AjustarQueima(50.0f);
        estrategia.aplicar(&sensorRad, &varetas);
        assert(std::abs(varetas.getValorAtual() - 50.0f) < 0.1f);
    }

    std::cout << "Todos os testes C++ passaram." << std::endl;
    return 0;
}