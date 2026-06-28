#include "EstadoReator.hpp"

// Implementação do Singleton
Reator& Reator::getInstance() {
    static Reator instance;
    return instance;
}

// Implementação dos métodos
EstadoReator Reator::getEstado() const {
    return estado;
}

void Reator::setEstado(EstadoReator novoEstado) {
    estado = novoEstado;
}

bool Reator::isNormal() const {
    return estado == EstadoReator::NORMAL;
}

bool Reator::isAlerta() const {
    return estado == EstadoReator::ALERTA;
}

bool Reator::isExplodido() const {
    return estado == EstadoReator::EXPLODIDO;
}