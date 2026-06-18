#include "EstrategiaControle.hpp"

ControleNivel::ControleNivel(float baixo, float alto) : nivelBaixo(baixo), nivelAlto(alto) {}

void ControleNivel::aplicar(Sensor* sensor, Atuador* atuador) {
    SensorNivel* sNivel = dynamic_cast<SensorNivel*>(sensor);
    if (!sNivel) return;

    float nivel = sNivel->getValorAtual();

    if (nivel < nivelBaixo) {
        atuador->ligar();
    } else if (nivel > nivelAlto) {
        atuador->desligar();
    }
}

ControleTemperatura::ControleTemperatura(float RMax) : RadMax(RMax) {}

void ControleTemperatura::aplicar(Sensor* sensor, Atuador* atuador) {
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
