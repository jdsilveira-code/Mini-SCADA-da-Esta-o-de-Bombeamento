#ifndef ESTRATEGIA_CONTROLE_HPP
#define ESTRATEGIA_CONTROLE_HPP

#include "Atuadores.hpp"
#include "Sensores.hpp"
#include <string>

class EstrategiaControle {
    protected:
        int LimiteMin;
        int LimiteMax;
        std::string Tag;
        std::string Modo = "Nao selecionado";
    public:
        EstrategiaControle(int min, int max, std::string tag);
        EstrategiaControle(int max, std::string tag);
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
    private:

    public:
        ControleTemperatura(float RMax);
        void aplicar(Sensor* sensor, Atuador* atuador) override;
};

class ControleQueima : public EstrategiaControle {
    private:

    public:
        ControleQueima(float Dose, float qMax);
        //void aplicar(Sensor* sensor, Atuador* atuador) override;
};

#endif