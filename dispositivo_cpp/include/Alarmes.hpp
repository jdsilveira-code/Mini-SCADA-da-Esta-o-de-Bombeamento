#ifndef ALARMES_HPP
#define ALARMES_HPP

#include "Sensores.hpp"
#include <string>
#include <string>

class EstrategiaAlarme {
    protected:
        int LimiteMin;
        int LimiteMax;
        std::string Tag;
        std::string StatusAlarme;

    public:
        EstrategiaAlarme(int min, int max, std::string tag, std::string Status);
        EstrategiaAlarme(int max, std::string tag);
        virtual void verificar(Sensor* sensor) = 0;
        virtual ~EstrategiaAlarme() {}
};

/*
class GerenciadorAlarme {
    public:
        // inicializa com os limites definidos nos comentários
        EstrategiaAlarme alarmeTemperatura{600, 1750, "LT-101"};
        EstrategiaAlarme alarmeNivel{50, 99, ""};
        EstrategiaAlarme alarmeRadiacao{100, ""};
        EstrategiaAlarme alarmeVazao{10, 500, ""};
};

*/;





class AlarmeTemperatura : public EstrategiaAlarme {
    public:
        AlarmeTemperatura(int limiteMinimo, int limiteMaximo);
        void verificar(Sensor* sensor) override;
};

class AlarmeNivel : public EstrategiaAlarme {
    public:
        AlarmeNivel(int limiteMinimo, int limiteMaximo);
        void verificar(Sensor* sensor) override;
};
    
class AlarmeRadiacao : public EstrategiaAlarme {
    public:
        AlarmeRadiacao(int limiteMaximo);
        void verificar(Sensor* sensor) override;
};

class AlarmeVazao : public EstrategiaAlarme {
    public:
        AlarmeVazao(int limiteMinimo, int limiteMaximo);
        void verificar(Sensor* sensor) override;
};


#endif