#ifndef ATUADORES_HPP
#define ATUADORES_HPP

#include "sensores.hpp"
#include <string>

// Classe base para todos os atuadores da estacao
class Atuador {
    protected:
        std::string Tag;
        bool ligado{false};

    public:
        Atuador(std::string tag) : Tag(tag) {}

        virtual void ligar() { ligado = true; }
        virtual void desligar() { ligado = false; }

        bool isLigado() { return ligado; }
        std::string getTag() { return Tag; }

        virtual ~Atuador() {}
};

class BombaAgua : public Atuador {
    private:
        float Potencia{0.0f};
        SensorNivel* sensorNivel{nullptr};

    public:
        BombaAgua(std::string tag) : Atuador(tag) {}

        void ligar() override {
            ligado = true;
            Potencia = 100.0f;
        }

        void desligar() override {
            ligado = false;
            Potencia = 0.0f;
        }

        void setPotencia(float NovaPotencia){
            if (NovaPotencia < 0.0f || NovaPotencia > 100.0f) {
                throw std::out_of_range("Potencia inválida");
            }
            else{
                Potencia = NovaPotencia;
            }
        }

        float getPotencia(){
            return Potencia;
        }

        void vincularSensor(SensorNivel* sensor) {
            sensorNivel = sensor;
        }

        SensorNivel* getSensor() {
            return sensorNivel;
        }
};

class Varetas : public Atuador {
    private:
        float ValorAtual{0.0f};

    public:
        Varetas(std::string tag) : Atuador(tag) {}

        void ligar() override {
            ligado = true;
        }

        void desligar() override {
            ligado = false;
            ValorAtual = 0.0f;
        }

        void AjustarQueima(float NovoValor){
            ValorAtual = NovoValor;
        }

        float getValorAtual(){
            return ValorAtual;
        }
};

#endif // ATUADORES_HPP
