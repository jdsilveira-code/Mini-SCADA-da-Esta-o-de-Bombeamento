#include "EstrategiaControle.hpp"

EstrategiaControle::EstrategiaControle(int limiteMin, int limiteMax)
    : LimiteMin(limiteMin), LimiteMax(limiteMax) {}

EstrategiaControle::EstrategiaControle(int limiteMax)
    : LimiteMin(0), LimiteMax(limiteMax) {}

ControleNivel::ControleNivel(float baixo, float alto)
    : EstrategiaControle(static_cast<int>(baixo), static_cast<int>(alto)) {}

void ControleNivel::aplicar(Sensor* sensor, Atuador* atuador) {
    SensorNivel* sNivel = dynamic_cast<SensorNivel*>(sensor);
    BombaAgua* bomba = dynamic_cast<BombaAgua*>(atuador);
    if (!sNivel || !bomba) return;

    float nivel = static_cast<float>(sNivel->getValorAtual());

    if (nivel <= LimiteMin) {
        bomba->setPotencia(100.0f);
        bomba->ligar();
    } else if (nivel >= LimiteMax) {
        bomba->desligar();
    } else {
        float potencia = ((nivel - LimiteMin) / (LimiteMax - LimiteMin)) * 100.0f;
        bomba->setPotencia(potencia);
        bomba->ligar();
    }
}

ControleTemperatura::ControleTemperatura(float tempMax)
    : EstrategiaControle(static_cast<int>(tempMax)) {}

void ControleTemperatura::aplicar(Sensor* sensor, Atuador* atuador) {
    SensorTemp* sTemp = dynamic_cast<SensorTemp*>(sensor);
    if (!sTemp || !atuador) return;

    float temp = static_cast<float>(sTemp->getValorAtual());

    if (temp > LimiteMax) {
        atuador->desligar();
    } else {
        atuador->ligar();
    }
}

ControleQueima::ControleQueima(float dose, float qMax)
    : EstrategiaControle(static_cast<int>(dose), static_cast<int>(qMax)),
      DoseAcumuladaMin(-1.0f),
      DoseAcumuladaMax(dose),
      TempMax(qMax),
      SensorTemperatura(nullptr) {}

ControleQueima::ControleQueima(float doseMin, float doseMax, SensorTemp* sTemp, float tempMax)
    : EstrategiaControle(static_cast<int>(doseMin), static_cast<int>(doseMax)),
      DoseAcumuladaMin(doseMin),
      DoseAcumuladaMax(doseMax),
      TempMax(tempMax),
      SensorTemperatura(sTemp) {}

void ControleQueima::aplicar(Sensor* sensor, Atuador* atuador) {
    SensorRadiacao* sRad = dynamic_cast<SensorRadiacao*>(sensor);
    Varetas* varetas = dynamic_cast<Varetas*>(atuador);
    if (!sRad || !varetas) return;

    float dose = sRad->getDoseAcumulada();

    if (dose >= DoseAcumuladaMax) {
        varetas->AjustarQueima(100.0f);
        return;
    }

    if (DoseAcumuladaMin >= 0.0f && dose < DoseAcumuladaMin && SensorTemperatura != nullptr) {
        float temp = static_cast<float>(SensorTemperatura->getValorAtual());
        if (temp < TempMax) {
            varetas->AjustarQueima(0.0f);
        }
    }
}
