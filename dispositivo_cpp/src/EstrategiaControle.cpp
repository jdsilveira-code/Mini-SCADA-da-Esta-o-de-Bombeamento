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

    float potencia = ((LimiteMax - nivel) / (LimiteMax - LimiteMin)) * 100.0f;


    // Garantir que a potência esteja dentro do intervalo de 0 a 100
    if (potencia < 0.0f){
        potencia = 0.0f;
    }

    if (potencia > 100.0f){
        potencia = 100.0f;
    }
    

    bomba->setPotencia(potencia);
    bomba->ligar();
    
}

ControleTemperatura::ControleTemperatura(float tempMax)
    : EstrategiaControle(static_cast<int>(tempMax)) {}

void ControleTemperatura::aplicar(Sensor* sensor, Atuador* atuador) {
    SensorTemp* sTemp = dynamic_cast<SensorTemp*>(sensor);
    BombaAgua* bomba = dynamic_cast<BombaAgua*>(atuador);
    if (!sTemp || !atuador) return;

    float temp = static_cast<float>(sTemp->getValorAtual());

    if (temp > LimiteMax) {
        bomba->ligar();
    } else {
        bomba->desligar();
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

    //float dose = sRad->getDoseAcumulada();

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
