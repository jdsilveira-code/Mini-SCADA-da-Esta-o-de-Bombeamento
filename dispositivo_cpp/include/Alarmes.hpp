#ifndef ALARMES_HPP
#define ALARMES_HPP

#include "Sensores.hpp"

class EstrategiaAlarme {
    public:
        virtual void verificar(Sensor* sensor) = 0;
        virtual ~EstrategiaAlarme() {}
};

class AlarmeTemperatura : public EstrategiaAlarme {
    private:
        int limiteMinimo = 600;
        int limiteMaximo = 1750;

    public:
        AlarmeTemperatura(int limiteMinimo, int limiteMaximo);
        void verificar(Sensor* sensor) override;
};

class AlarmeNivel : public EstrategiaAlarme {
    private:
        int limiteMinimo = 50;
        int limiteMaximo = 99;

    public:
        AlarmeNivel(int limiteMinimo, int limiteMaximo);
        void verificar(Sensor* sensor) override;
};
    
class AlarmeRadiacao : public EstrategiaAlarme {
    private:
        int limiteMaximo = 100;

    public:
        AlarmeRadiacao(int limiteMaximo);
        void verificar(Sensor* sensor) override;
};

class AlarmeVazao : public EstrategiaAlarme {
    private:
        int limiteMinimo;
        int limiteMaximo;

    public:
        AlarmeVazao(int limiteMinimo, int limiteMaximo);
        void verificar(Sensor* sensor) override;
};

#endif // ALARMES_HPP
