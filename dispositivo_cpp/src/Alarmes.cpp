#include "Alarmes.hpp"
#include "Sensores.hpp"


AlarmeTemperatura::AlarmeTemperatura(int limiteMinimo, int limiteMaximo, std::string tag, std::StatusAlarme)
: limiteMinimo(limiteMinimo), limiteMaximo(limiteMaximo), tag(tag), StatusAlarme(StatusAlarme) {}

void AlarmeTemperatura::verificar(Sensor* sensor) {
    if (sensor == nullptr) {
        return;
    }
    
    tag = sensor->getTag()
    int valor = sensor->getValorAtual();
    if (valor < limiteMinimo) {
        StatusAlarme = "ALERTA - TEMPERATURA BAIXA";
    } else if (valor > limiteMaximo) {
        StatusAlarme = "ALERTA - TEMPERATURA ALTA";
    } else {
        StatusAlarme = "Normal";
    }
}

AlarmeNivel::AlarmeNivel(int limiteMinimo, int limiteMaximo)
: limiteMinimo(limiteMinimo), limiteMaximo(limiteMaximo) tag(tag), StatusAlarme(StatusAlarme){}

void AlarmeNivel::verificar(Sensor* sensor) {
    if (sensor == nullptr) {
        return;
    }
    tag = sensor->getTag()
    int valor = sensor->getValorAtual();
    if (valor < limiteMinimo) {
        StatusAlarme = "ALERTA - NÍVEL BAIXO";
    } else if (valor > limiteMaximo) {
        StatusAlarme = "ALERTA - NÍVEL ALTO";
    } else {
        StatusAlarme = "Normal";
    }
}



AlarmeRadiacao::AlarmeRadiacao(int limiteMaximo)
: limiteMaximo(limiteMaximo), tag(tag), StatusAlarme(StatusAlarme) {}

void AlarmeRadiacao::verificar(Sensor* sensor) {
    if (sensor == nullptr) {
        return;
    }
    tag = sensor->getTag()

    int valor = sensor->getValorAtual();
    if (valor > limiteMaximo) {
        StatusAlarme = "ALERTA - RADIAÇÃO ALTA";
    } else {
        StatusAlarme = "Normal";
    }
}

AlarmeVazao::AlarmeVazao(int limiteMinimo, int limiteMaximo)
: limiteMinimo(limiteMinimo), limiteMaximo(limiteMaximo), tag(tag), StatusAlarme(StatusAlarme) {}

void AlarmeVazao::verificar(Sensor* sensor) {
    if (sensor == nullptr) {
        return;
    }
    tag = sensor->getTag()
    int valor = sensor->getValorAtual();
    if (valor < limiteMinimo) {
        StatusAlarme = "ALERTA - VAZÃO BAIXA";
    } else if (valor > limiteMaximo) {
        StatusAlarme = "ALERTA - VAZÃO ALTA";
    } else {
        StatusAlarme = "Normal";
    }
}

#endif;