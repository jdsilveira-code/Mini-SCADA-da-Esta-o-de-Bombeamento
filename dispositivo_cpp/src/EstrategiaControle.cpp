#include "EstrategiaControle.hpp"
#include <algorithm>

EstrategiaControle::EstrategiaControle(float limiteMin, float limiteMax)
    : LimiteMin(limiteMin), LimiteMax(limiteMax) {}

EstrategiaControle::EstrategiaControle(float limiteMax)
    : LimiteMin(0.0f), LimiteMax(limiteMax) {}

ControleNivel::ControleNivel(float baixo, float alto)
    : EstrategiaControle(baixo, alto) {}

void ControleNivel::aplicar(Sensor* sensor, Atuador* atuador) {
    SensorNivel* sNivel = dynamic_cast<SensorNivel*>(sensor);
    BombaAgua* bomba = dynamic_cast<BombaAgua*>(atuador);

    if (!sNivel || !bomba) return;

    float nivel = static_cast<float>(sNivel->getValorAtual());

    float potencia = ((LimiteMax - nivel) / (LimiteMax - LimiteMin)) * 100.0f;
    potencia = std::clamp(potencia, 0.0f, 100.0f);

    bomba->setPotencia(potencia);
    bomba->ligar();
    
}

ControleTemperatura::ControleTemperatura(float tempMax)
    : EstrategiaControle(tempMax) {}

void ControleTemperatura::aplicar(Sensor* sensor, Atuador* atuador) {
    SensorTemp* sTemp = dynamic_cast<SensorTemp*>(sensor);
    BombaAgua* bomba = dynamic_cast<BombaAgua*>(atuador);
    if (!sTemp || !bomba) return;

    float temp = static_cast<float>(sTemp->getValorAtual());

    if (temp > LimiteMax) {
        bomba->ligar();
    } else {
        bomba->desligar();
    }
}

ControleQueima::ControleQueima(float dose, float qMax)
    : EstrategiaControle(dose, qMax),
      DoseAcumuladaMin(-1.0f),
      DoseAcumuladaMax(dose),
      TempMax(qMax),
      SensorTemperatura(nullptr) {}

ControleQueima::ControleQueima(float doseMin, float doseMax, SensorTemp* sTemp, float tempMax)
    : EstrategiaControle(doseMin, doseMax),
      DoseAcumuladaMin(doseMin),
      DoseAcumuladaMax(doseMax),
      TempMax(tempMax),
      SensorTemperatura(sTemp) {}

void ControleQueima::aplicar(Sensor* sensor, Atuador* atuador) {
    SensorRadiacao* sRad = dynamic_cast<SensorRadiacao*>(sensor);
    Varetas* varetas = dynamic_cast<Varetas*>(atuador);
    
    if (!sRad || !varetas) return;

    if (sRad->getDoseAcumulada() >= DoseAcumuladaMax) {
        varetas->AjustarQueima(100.0f);
        return;
    }

    if (DoseAcumuladaMin >= 0.0f && sRad->getDoseAcumulada() < DoseAcumuladaMin && SensorTemperatura != nullptr) {
        float temp = static_cast<float>(SensorTemperatura->getValorAtual());
        if (temp < TempMax) {
            varetas->AjustarQueima(0.0f);
        }
    }
}
