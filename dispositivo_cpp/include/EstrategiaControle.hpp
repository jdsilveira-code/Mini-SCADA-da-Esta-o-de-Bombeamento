#ifndef ESTRATEGIA_CONTROLE_HPP
#define ESTRATEGIA_CONTROLE_HPP

#include "Atuadores.hpp"
#include "Sensores.hpp"

class EstrategiaControle {
    protected:
        float LimiteMin;
        float LimiteMax;
    public:
        EstrategiaControle(float limiteMin, float limiteMax);
        EstrategiaControle(float limiteMax);
        virtual void aplicar(Sensor* sensor, Atuador* atuador) = 0;
        virtual ~EstrategiaControle() {}
};

class ControleNivel : public EstrategiaControle {
    public:
        ControleNivel(float baixo, float alto);
        void aplicar(Sensor* sensor, Atuador* atuador) override;
};

class ControleTemperatura : public EstrategiaControle {
    public:
        ControleTemperatura(float RMax);
        void aplicar(Sensor* sensor, Atuador* atuador) override;
};

class ControleQueima : public EstrategiaControle {
    private:
        float DoseAcumuladaMin;
        float DoseAcumuladaMax;
        float TempMax;
        SensorTemp* SensorTemperatura;
    public:
        ControleQueima(float Dose, float qMax);
        ControleQueima(float DoseMin, float DoseMax, SensorTemp* sTemp, float tempMax);
        void aplicar(Sensor* sensor, Atuador* atuador) override;
};

#endif