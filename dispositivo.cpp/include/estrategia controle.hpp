#ifndef ESTRATEGIA_CONTROLE_HPP
#define ESTRATEGIA_CONTROLE_HPP

#include "Atuadores.hpp"

// Interface do padrao Strategy para regras de controle
class EstrategiaControle {
    public:
        virtual void aplicar(Sensor* sensor, Atuador* atuador) = 0;
        virtual ~EstrategiaControle() {}
};

// Liga a bomba quando o nivel esta baixo, desliga quando esta alto
class ControleNivel : public EstrategiaControle {
    private:
        float nivelBaixo;
        float nivelAlto;

    public:
        ControleNivel(float baixo, float alto) : nivelBaixo(baixo), nivelAlto(alto) {}

        void aplicar(Sensor* sensor, Atuador* atuador) override {
            float nivel = sensor->getValorAtual();

            if (nivel < nivelBaixo) {
                atuador->ligar();
            } else if (nivel > nivelAlto) {
                atuador->desligar();
            }
        }
};

// Controla as varetas baseado na temperatura do sistema
class ControleTemperatura : public EstrategiaControle {
    private:
        float tempMaxima;

    public:
        ControleTemperatura(float tMax) : tempMaxima(tMax) {}

        void aplicar(Sensor* sensor, Atuador* atuador) override {
            float temp = sensor->getValorAtual();

            if (temp > tempMaxima) {
                atuador->desligar();
            } else {
                atuador->ligar();
            }
        }
};

#endif // ESTRATEGIA_CONTROLE_HPP
