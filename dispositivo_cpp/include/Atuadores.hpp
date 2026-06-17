#ifndef ATUADORES_HPP
#define ATUADORES_HPP

#include "Sensores.hpp"
#include <string>
#include <stdexcept>

class Atuador {
    protected:
        std::string Tag;
        bool ligado{false};

    public:
        Atuador(std::string tag);

        virtual void ligar();
        virtual void desligar();

        bool isLigado();
        std::string getTag();

        virtual ~Atuador() {}
};

class BombaAgua : public Atuador {
    private:
        float Potencia{0.0f};
        SensorNivel* sensorNivel{nullptr};

    public:
        BombaAgua(std::string tag);

        void ligar(float Potencia) override;
        void desligar() override;

        void setPotencia(float NovaPotencia);
        float getPotencia();

        void vincularSensor(SensorNivel* sensor);
        SensorNivel* getSensor();
};

class Varetas : public Atuador {
    private:
        float ValorAtual{0.0f};

    public:
        Varetas(std::string tag);

        void AjustarQueima(float NovoValor);
        float getValorAtual();
};

#endif // ATUADORES_HPP
