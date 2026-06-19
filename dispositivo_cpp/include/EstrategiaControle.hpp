#ifndef ESTRATEGIA_CONTROLE_HPP
#define ESTRATEGIA_CONTROLE_HPP

#include "Atuadores.hpp"
#include "Sensores.hpp"

class SensorTemp;

class EstrategiaControle {
    protected:
        int LimiteMin;
        int LimiteMax;
    public:
        EstrategiaControle(int limiteMin, int limiteMax);
        EstrategiaControle(int LimiteMax);
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