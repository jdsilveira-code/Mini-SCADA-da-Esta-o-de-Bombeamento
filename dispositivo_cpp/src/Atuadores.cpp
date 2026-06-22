#include "Atuadores.hpp"

Atuador::Atuador(std::string tag) : Tag(tag) {}

void Atuador::ligar() { ligado = true; }
void Atuador::desligar() { ligado = false; }

bool Atuador::isLigado() const { return ligado; }
std::string Atuador::getTag() { return Tag; }

BombaAgua::BombaAgua(std::string tag) : Atuador(tag) {}

void BombaAgua::ligar() {
    ligado = true;
    Potencia = Potencia > 0.0f ? Potencia : 50.0f; // Liga com potencia setada ou se for 0, liga com 50%.
}

void BombaAgua::desligar() {
    ligado = false;
    Potencia = 0.0f;
}

void BombaAgua::setPotencia(float NovaPotencia) {
    if (NovaPotencia < 0.0f || NovaPotencia > 100.0f)
        throw std::out_of_range("Potencia invalida");
    Potencia = NovaPotencia;
    if (ligado && Potencia == 0.0f) {
        desligar();
    } else if (!ligado && Potencia > 0.0f) {
        ligar();
    }
}

float BombaAgua::getPotencia() { return Potencia; }

Varetas::Varetas(std::string tag) : Atuador(tag) {}

void Varetas::ligar() { ligado = true; }

void Varetas::desligar() {
    ligado = false;
}

bool Varetas::isLigado() const {
    return Atuador::isLigado();
}

void Varetas::AjustarQueima(float NovoValor) {
    if(NovoValor < 0.0f || NovoValor > 100.0f)
        throw std::out_of_range("Queima inválida");
    
    ValorAtual = NovoValor; 
    // Mapear semântica desejada:
    // - ValorAtual == 0.0f  => varetas RETIRADAS (ligado == true)
    // - ValorAtual >  0.0f  => varetas INSERIDAS  (ligado == false)
    if (ValorAtual == 0.0f) {
        ligar();
    } else {
        desligar();
    }
}

float Varetas::getValorAtual() { 
    return ValorAtual; 
}
