#ifndef ESTADO_REATOR_HPP
#define ESTADO_REATOR_HPP

enum class EstadoReator {
    NORMAL,
    ALERTA,      // dose >= 250
    EXPLODIDO    // dose >= 300
};

class Reator {
public:
    // Singleton: obtém a instância única
    static Reator& getInstance();

    // Getters e setters
    EstadoReator getEstado() const;
    void setEstado(EstadoReator novoEstado);

    // Métodos auxiliares
    bool isNormal() const;
    bool isAlerta() const;
    bool isExplodido() const;

private:
    // Construtor privado (singleton)
    Reator() = default;

    // Estado atual
    EstadoReator estado = EstadoReator::NORMAL;
};

#endif