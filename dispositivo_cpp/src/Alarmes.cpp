#include "Alarmes.hpp"

/*
AlarmeTemperatura::AlarmeTemperatura(int limiteMinimo, int limiteMaximo)
: limiteMinimo(limiteMinimo), limiteMaximo(limiteMaximo) {}

void AlarmeTemperatura::verificar(Sensor* sensor) {
    if (sensor == nullptr) {
        return;
    }
    
    int valor = sensor->getValorAtual();
    if (valor < limiteMinimo) {
        sensor->setStatus("ALERTA - TEMPERATURA BAIXA");
    } else if (valor > limiteMaximo) {
        sensor->setStatus("ALERTA - TEMPERATURA ALTA");
    } else {
        sensor->setStatus("OPERACIONAL");
    }
}

AlarmeNivel::AlarmeNivel(int limiteMinimo, int limiteMaximo)
: limiteMinimo(limiteMinimo), limiteMaximo(limiteMaximo) {}

void AlarmeNivel::verificar(Sensor* sensor) {
    if (sensor == nullptr) {
        return;
    }
    
    int valor = sensor->getValorAtual();
    if (valor < limiteMinimo) {
        sensor->setStatus("ALERTA - NÍVEL BAIXO");
    } else if (valor > limiteMaximo) {
        sensor->setStatus("ALERTA - NÍVEL ALTO");
    } else {
        sensor->setStatus("OPERACIONAL");
    }
}


AlarmeRadiacao::AlarmeRadiacao(int limiteMaximo)
: limiteMaximo(limiteMaximo) {}

void AlarmeRadiacao::verificar(Sensor* sensor) {
    if (sensor == nullptr) {
        return;
    }
    
    int valor = sensor->getValorAtual();
    if (valor > limiteMaximo) {
        sensor->setStatus("ALERTA");
    } else {
        sensor->setStatus("OPERACIONAL");
    }
}

AlarmeVazao::AlarmeVazao(int limiteMinimo, int limiteMaximo)
: limiteMinimo(limiteMinimo), limiteMaximo(limiteMaximo) {}

void AlarmeVazao::verificar(Sensor* sensor) {
    if (sensor == nullptr) {
        return;
    }
    
    int valor = sensor->getValorAtual();
    if (valor < limiteMinimo) {
        sensor->setStatus("ALERTA - VAZÃO BAIXA");
    } else if (valor > limiteMaximo) {
        sensor->setStatus("ALERTA - VAZÃO ALTA");
    } else {
        sensor->setStatus("OPERACIONAL");
    }
}

*/;