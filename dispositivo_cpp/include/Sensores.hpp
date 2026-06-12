#ifndef SENSORES_HPP
#define SENSORES_HPP

#include <string>
#include "GeradorNumeros.hpp"

class Sensor {
    protected:
        float ValorAtual;
        std::string Tag;
        std::string UnidadeMedida;
        GeradorAleatorio gerador;

    public:
        virtual void ler() = 0;
        double getValorAtual();
        virtual ~Sensor() {}
};

class SensorNivel : public Sensor {
    private:
        float ValorMax;
        float ValorMin;
    public:
        SensorNivel(std::string tag, float max, float min);
        void ler() override;
};

class SensorRadiacao : public Sensor {
    private:
        float ValorMax;
        float ValorMin;
    public:
        SensorRadiacao(std::string tag, float max, float min);
        void ler() override;
};

class SensorTemp : public Sensor {
    private:
        float ValorMax;
        float ValorMin;
    public:
        SensorTemp(std::string tag, float max, float min);
        void ler() override;
};

class SensorVazao : public Sensor {
    private:
        float ValorMax;
        float ValorMin;
    public:
        SensorVazao(std::string tag, float max, float min);
        void ler() override;
};

#endif // SENSORES_HPP
