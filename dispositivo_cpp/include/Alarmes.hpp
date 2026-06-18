#ifndef ALARMES_HPP
#define ALARMES_HPP

#include "Sensores.hpp"
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


#endif;