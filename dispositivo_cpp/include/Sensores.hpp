#ifndef SENSORES_HPP
#define SENSORES_HPP
#include <string>
#include "GeradorNumeros.hpp"

class Sensor{
    protected:
        float ValorAtual;
        std::string Tag;
        std::string UnidadeMedida;
        GeradorAleatorio gerador;

    public:
        virtual void ler() = 0;
        double getValorAtual();

};

class SensorNivel : public Sensor{
    private:
        float ValorMax;
        float ValorMin;
    public:
        SensorNivel() = default;
        void ler() override;
};

class SensorRadiacao : public Sensor{
    private:
        float ValorMax;
        float ValorMin;
    public:
        SensorRadiacao() = default;
        void ler() override;
};

class SensorTemp : public Sensor{
    private:
        float ValorMax;
        float ValorMin;
    public:
        SensorTemp() = default;
        void ler() override; 
};

class SensorVazao : public Sensor{
    private:
        float ValorMax;
        float ValorMin;
    public:
        SensorVazao() = default;
        void ler() override;
};

#endif // SENSORES_HPP