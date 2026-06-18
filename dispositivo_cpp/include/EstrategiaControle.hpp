#ifndef ESTRATEGIA_CONTROLE_HPP
#define ESTRATEGIA_CONTROLE_HPP

#include "Atuadores.hpp"
#include "Sensores.hpp"


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
    private:

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
    /*
    private:
        float DoseAcumuladaMin;
        float DoseAcumuladaMax;
    */
    public:
        ControleQueima(float Dose, float qMax);
        void aplicar(Sensor* sensor, Atuador* atuador) override;
};

#endif