#ifndef ATUADORES_HPP
#define ATUADORES_HPP

#include "sensores.hpp"
#include <string>

class BombaAgua{
    private:
        std::string Tag;
        float Potencia{0.0f};
        // referencia para o sensor de nível que controla a bomba
        SensorNivel* sensorNivel{nullptr};

    public:
        void EcolherPotencia(float NovaPotencia){ 
            Potencia = NovaPotencia;
        }
        float getPotencia(){
            return Potencia;
        }
};

class Varetas{
    private:
        std:: string Tag;
        float ValorAtual;

    public:
        void AjustarQueima(float NovoValor){
            ValorAtual = NovoValor;
        }
        float getValorAtual(){
            return ValorAtual;
        }
};
#endif // ATUADORES_HPP