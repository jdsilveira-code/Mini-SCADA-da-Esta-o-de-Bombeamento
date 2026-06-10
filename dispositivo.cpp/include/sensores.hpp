
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
        virtual void ler() {}
        
        double getValorAtual(){
            return ValorAtual;
        }

};

class SensorNivel : public Sensor{
    private:
        float ValorMax;
        float ValorMin;
    public:
        SensorNivel() = default;
        void ler() override { ValorAtual = gerador.decimal(ValorMin, ValorMax); }
};

class SensorRadiacao : public Sensor{
    private:
        float ValorMax;
        float ValorMin;
    public:
        SensorRadiacao() = default;
        void ler() override { ValorAtual = gerador.decimal(ValorMin, ValorMax); }
};

class SensorTemp : public Sensor{
    private:
        float ValorMax;
        float ValorMin;
    public:
        SensorTemp() = default;
        void ler() override { ValorAtual = gerador.decimal(ValorMin, ValorMax); }
};

class SensorVazao : public Sensor{
    private:
        float ValorMax;
        float ValorMin;
    public:
        SensorVazao() = default;
        void ler() override { ValorAtual = gerador.decimal(ValorMin, ValorMax); }
};