#ifndef ALARMES_HPP
#define ALARMES_HPP

#include "Sensores.hpp"
#include <string>

class EstrategiaAlarme {
    protected:
        int LimiteMin;
        int LimiteMax;
        std::string Tag;

    public:
        EstrategiaAlarme(int min, int max, std::string tag);
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





#endif // ALARMES_HPP
class AlarmeTemperatura : public EstrategiaAlarme {
    private:
    /*
    int limiteMinimo = 600;
    int limiteMaximo = 1750;
    */;

    public:
        AlarmeTemperatura(int limiteMinimo, int limiteMaximo);
        void verificar(Sensor* sensor) override;
};

class AlarmeNivel : public EstrategiaAlarme {
    private:
    /*
    int limiteMinimo = 50;
    int limiteMaximo = 99;
    */;
    public:
        AlarmeNivel(int limiteMinimo, int limiteMaximo);
        void verificar(Sensor* sensor) override;
};
    
class AlarmeRadiacao : public EstrategiaAlarme {
    private:
        /*
        int limiteMaximo = 100;
        */;
    public:
        AlarmeRadiacao(int limiteMaximo);
        void verificar(Sensor* sensor) override;
};

class AlarmeVazao : public EstrategiaAlarme {
    private:
    /*
        int limiteMinimo;
        int limiteMaximo;
    */;
    public:
        AlarmeVazao(int limiteMinimo, int limiteMaximo);
        void verificar(Sensor* sensor) override;
};
/*
*/;