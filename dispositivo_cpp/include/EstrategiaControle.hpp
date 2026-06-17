#ifndef ESTRATEGIA_CONTROLE_HPP
#define ESTRATEGIA_CONTROLE_HPP

#include "Atuadores.hpp"

class EstrategiaControle {
    public:
        virtual void aplicar(Sensor* sensor, Atuador* atuador) = 0;
        virtual ~EstrategiaControle() {}
};

class ControleNivel : public EstrategiaControle {
    private:
        float nivelBaixo;
        float nivelAlto;

    public:
        ControleNivel(float baixo, float alto);
        void aplicar(Sensor* sensor, Atuador* atuador) override;
};

class ControleTemperatura : public EstrategiaControle {
    private:
        float RadMax;

    public:
        ControleTemperatura(float RMax);
        void aplicar(Sensor* sensor, Atuador* atuador) override;
};

class ControleQueima : public EstrategiaControle {
    private:
        float DoseAcumuladaMin;
        float DoseAcumuladaMax;

    public:
        ControleQueima(float Dose, float qMax);
        //void aplicar(Sensor* sensor, Atuador* atuador) override;
};

#endif // ESTRATEGIA_CONTROLE_HPP
