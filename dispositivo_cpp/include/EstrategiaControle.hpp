#ifndef ESTRATEGIA_CONTROLE_HPP
#define ESTRATEGIA_CONTROLE_HPP

#include <string>
#include "Atuadores.hpp"
#include "Sensores.hpp"

class EstrategiaControle {
protected:
    float LimiteMin;
    float LimiteMax;

public:
    EstrategiaControle(float limiteMin, float limiteMax);
    explicit EstrategiaControle(float limiteMax);
    virtual ~EstrategiaControle() = default;

    virtual void aplicar(Sensor* sensor, Atuador* atuador) = 0;
};

class ControleNivel : public EstrategiaControle {
public:
    ControleNivel(float baixo, float alto);
    void aplicar(Sensor* sensor, Atuador* atuador) override;
};

class ControleTemperatura : public EstrategiaControle {
public:
    explicit ControleTemperatura(float tempMax);
    void aplicar(Sensor* sensor, Atuador* atuador) override;
};

class ControlePressao : public EstrategiaControle {
public:
    ControlePressao(float baixo, float alto);
    void aplicar(Sensor* sensor, Atuador* atuador) override;
};

enum class AcaoBomba {
    AUTOMATICO,
    LIGAR,
    DESLIGAR
};

enum class AcaoVaretas {
    AUTOMATICO,
    INSERIR,
    RETIRAR
};

class ControleBombaManual : public EstrategiaControle {
private:
    AcaoBomba Acao;

public:
    ControleBombaManual();
    void setAcao(AcaoBomba novaAcao);
    void aplicar(Sensor* sensor, Atuador* atuador) override;
};

class ControleBombaAutomatico : public EstrategiaControle {
private:
    ControleNivel& ControleNivelDependencia;
    ControleTemperatura& ControleTemperaturaDependencia;
    ControlePressao& ControlePressaoDependencia;
    SensorNivel* SensorNivelDependencia;
    SensorTemp* SensorTempDependencia;
    SensorPressao* SensorPressaoDependencia;

public:
    ControleBombaAutomatico(
        ControleNivel& ctrlNivel,
        ControleTemperatura& ctrlTemp,
        ControlePressao& ctrlPress,
        SensorNivel* sNivel,
        SensorTemp* sTemp,
        SensorPressao* sPress
    );
    void aplicar(Sensor* sensor, Atuador* atuador) override;
};

class ControleQueima : public EstrategiaControle {
private:
    float DoseAcumuladaMin;
    float DoseAcumuladaMax;
    float TempMax;
    SensorTemp* SensorTemperaturaDependencia;

public:
    ControleQueima(float doseAcumuladaMax, float tempMax);
    ControleQueima(float doseMin, float doseMax, SensorTemp* sTemp, float tempMax);
    void aplicar(Sensor* sensor, Atuador* atuador) override;
};

// Estratégia Manual (comandos diretos)
class EstrategiaManual : public EstrategiaControle {
private:
    AcaoBomba AcaoBombaAtual;
    float PotenciaBomba;
    AcaoVaretas acaoVaretas;   

public:
    EstrategiaManual();
    void setAcaoBomba(AcaoBomba a);
    void setPotenciaBomba(float p);
    void setAcaoVaretas(AcaoVaretas a);
    void aplicar(Sensor* sensor, Atuador* atuador) override;
};

// Estratégia Automática (composta pelos controles automáticos)
class EstrategiaAutomatica : public EstrategiaControle {
private:
    ControleBombaAutomatico& CtrlBombaAuto;
    ControleQueima& CtrlQueimaDep;

public:
    EstrategiaAutomatica(ControleBombaAutomatico& ctrlBombaAuto, ControleQueima& ctrlQueima);
    void aplicar(Sensor* sensor, Atuador* atuador) override;
};

#endif