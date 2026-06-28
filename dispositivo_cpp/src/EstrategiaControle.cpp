#include "EstrategiaControle.hpp"
#include <algorithm>
#include <string>

namespace {
    template <typename T>
    T clampValue(const T& value, const T& low, const T& high) {
        return std::max(low, std::min(value, high));
    }
}

EstrategiaControle::EstrategiaControle(float limiteMin, float limiteMax)
    : LimiteMin(limiteMin), LimiteMax(limiteMax) {}

EstrategiaControle::EstrategiaControle(float limiteMax)
    : LimiteMin(0.0f), LimiteMax(limiteMax) {}

ControleNivel::ControleNivel(float baixo, float alto)
    : EstrategiaControle(baixo, alto) {}

void ControleNivel::aplicar(Sensor* sensor, Atuador* atuador) {
    SensorNivel* sensorNivel = dynamic_cast<SensorNivel*>(sensor);
    BombaAgua* bomba = dynamic_cast<BombaAgua*>(atuador);

    if (!sensorNivel || !bomba) {
        return;
    }

    if (LimiteMax <= LimiteMin) {
        bomba->setPotencia(100.0f);
        bomba->ligar();
        return;
    }

    const float nivel = static_cast<float>(sensorNivel->getValorAtual());
    float potencia = ((LimiteMax - nivel) / (LimiteMax - LimiteMin)) * 100.0f;
    potencia = clampValue(potencia, 0.0f, 100.0f);

    bomba->setPotencia(potencia);
    bomba->ligar();
}

ControleTemperatura::ControleTemperatura(float tempMax)
    : EstrategiaControle(tempMax) {}

void ControleTemperatura::aplicar(Sensor* sensor, Atuador* atuador) {
    SensorTemp* sensorTemp = dynamic_cast<SensorTemp*>(sensor);
    BombaAgua* bomba = dynamic_cast<BombaAgua*>(atuador);

    if (!sensorTemp || !bomba) {
        return;
    }

    const float temperatura = static_cast<float>(sensorTemp->getValorAtual());
    if (temperatura > LimiteMax) {
        bomba->ligar();
    } else {
        bomba->desligar();
    }
}

ControlePressao::ControlePressao(float baixo, float alto)
    : EstrategiaControle(baixo, alto) {}

void ControlePressao::aplicar(Sensor* sensor, Atuador* atuador) {
    SensorPressao* sensorPressao = dynamic_cast<SensorPressao*>(sensor);
    BombaAgua* bomba = dynamic_cast<BombaAgua*>(atuador);

    if (!sensorPressao || !bomba) {
        return;
    }

    const float leituraPressao = static_cast<float>(sensorPressao->getValorAtual());

    float potenciaDesejada = bomba->getPotencia();
    if (LimiteMax > 0.0f && leituraPressao > LimiteMax) {
        const float excesso = (leituraPressao - LimiteMax) / LimiteMax;
        const float fatorSensibilidade = 2.0f;
        potenciaDesejada = 100.0f - (excesso * 100.0f * fatorSensibilidade);
        potenciaDesejada = clampValue(potenciaDesejada, 0.0f, 100.0f);
    }

    const float potenciaAtual = bomba->getPotencia();
    if (potenciaDesejada < potenciaAtual) {
        bomba->setPotencia(potenciaDesejada);
    }
}

ControleBombaManual::ControleBombaManual()
    : EstrategiaControle(0.0f), Acao(AcaoBomba::AUTOMATICO) {}

void ControleBombaManual::setAcao(AcaoBomba novaAcao) {
    Acao = novaAcao;
}

void ControleBombaManual::aplicar(Sensor* /*sensor*/, Atuador* atuador) {
    BombaAgua* bomba = dynamic_cast<BombaAgua*>(atuador);
    if (!bomba) {
        return;
    }

    if (Acao == AcaoBomba::LIGAR) {
        bomba->ligar();
    } else if (Acao == AcaoBomba::DESLIGAR) {
        bomba->desligar();
    }
}

ControleBombaAutomatico::ControleBombaAutomatico(
    ControleNivel& ctrlNivel,
    ControleTemperatura& ctrlTemp,
    ControlePressao& ctrlPress,
    SensorNivel* sNivel,
    SensorTemp* sTemp,
    SensorPressao* sPress
) : EstrategiaControle(0.0f),
    ControleNivelDependencia(ctrlNivel),
    ControleTemperaturaDependencia(ctrlTemp),
    ControlePressaoDependencia(ctrlPress),
    SensorNivelDependencia(sNivel),
    SensorTempDependencia(sTemp),
    SensorPressaoDependencia(sPress) {}

void ControleBombaAutomatico::aplicar(Sensor* /*sensor*/, Atuador* atuador) {
    if (!atuador || !SensorNivelDependencia || !SensorTempDependencia || !SensorPressaoDependencia) {
        return;
    }

    ControleNivelDependencia.aplicar(SensorNivelDependencia, atuador);
    ControleTemperaturaDependencia.aplicar(SensorTempDependencia, atuador);
    ControlePressaoDependencia.aplicar(SensorPressaoDependencia, atuador);
}

ControleQueima::ControleQueima(float doseAcumuladaMax, float tempMax)
    : EstrategiaControle(0.0f, doseAcumuladaMax),
      DoseAcumuladaMin(-1.0f),
      DoseAcumuladaMax(doseAcumuladaMax),
      TempMax(tempMax),
      SensorTemperaturaDependencia(nullptr) {}

ControleQueima::ControleQueima(float doseMin, float doseMax, SensorTemp* sTemp, float tempMax)
    : EstrategiaControle(doseMin, doseMax),
      DoseAcumuladaMin(doseMin),
      DoseAcumuladaMax(doseMax),
      TempMax(tempMax),
      SensorTemperaturaDependencia(sTemp) {}

void ControleQueima::aplicar(Sensor* sensor, Atuador* atuador) {
    SensorRadiacao* sensorRadiacao = dynamic_cast<SensorRadiacao*>(sensor);
    Varetas* varetas = dynamic_cast<Varetas*>(atuador);

    if (!sensorRadiacao || !varetas) {
        return;
    }

    const float doseAcumulada = sensorRadiacao->getDoseAcumulada();
    if (doseAcumulada >= DoseAcumuladaMax) {
        varetas->AjustarQueima(100.0f);
        return;
    }

    if (DoseAcumuladaMin >= 0.0f && doseAcumulada < DoseAcumuladaMin && SensorTemperaturaDependencia != nullptr) {
        const float temperatura = static_cast<float>(SensorTemperaturaDependencia->getValorAtual());
        if (temperatura < TempMax) {
            varetas->AjustarQueima(0.0f);
        }
    }
}

// ==================== ESTRATÉGIA MANUAL ====================
EstrategiaManual::EstrategiaManual()
    : EstrategiaControle(0.0f),
      AcaoBombaAtual(AcaoBomba::AUTOMATICO),
      PotenciaBomba(0.0f),
      acaoVaretas(AcaoVaretas::AUTOMATICO)   
{}

void EstrategiaManual::setAcaoBomba(AcaoBomba a) {
    AcaoBombaAtual = a;
}

void EstrategiaManual::setPotenciaBomba(float p) {
    PotenciaBomba = p;
}

void EstrategiaManual::setAcaoVaretas(AcaoVaretas a) {
    acaoVaretas = a;   
}

void EstrategiaManual::aplicar(Sensor* /*sensor*/, Atuador* atuador) {
    if (!atuador) return;

    BombaAgua* bomba = dynamic_cast<BombaAgua*>(atuador);
    if (bomba) {
        if (AcaoBombaAtual == AcaoBomba::LIGAR) {
            bomba->setPotencia(PotenciaBomba);
            bomba->ligar();
        } else if (AcaoBombaAtual == AcaoBomba::DESLIGAR) {
            bomba->desligar();
        }
        return;
    }

    Varetas* varetas = dynamic_cast<Varetas*>(atuador);
    if (varetas) {
        // Agora usando o membro com nome correto
        if (acaoVaretas == AcaoVaretas::INSERIR) {
            varetas->AjustarQueima(100.0f);
        } else if (acaoVaretas == AcaoVaretas::RETIRAR) {
            varetas->AjustarQueima(0.0f);
        }
    }
}

// ==================== ESTRATÉGIA AUTOMÁTICA ====================
EstrategiaAutomatica::EstrategiaAutomatica(ControleBombaAutomatico& ctrlBombaAuto, ControleQueima& ctrlQueima)
    : EstrategiaControle(0.0f), CtrlBombaAuto(ctrlBombaAuto), CtrlQueimaDep(ctrlQueima) {}

void EstrategiaAutomatica::aplicar(Sensor* sensor, Atuador* atuador) {
    if (!atuador) return;

    BombaAgua* bomba = dynamic_cast<BombaAgua*>(atuador);
    if (bomba) {
        CtrlBombaAuto.aplicar(nullptr, atuador);
        return;
    }

    Varetas* varetas = dynamic_cast<Varetas*>(atuador);
    if (varetas) {
        CtrlQueimaDep.aplicar(sensor, atuador);
    }
}