#ifndef ALARMES_HPP
#define ALARMES_HPP

#include <string>

class Sensor;

class EstrategiaAlarme {
    protected:
        int LimiteMin;
        int LimiteMax;
        std::string Tag;
        std::string StatusAlarme;
        std::string Timestamp;
        void atualizarTimestamp();

    public:
        EstrategiaAlarme(int min, int max, const std::string& tag);
        virtual void verificar(Sensor* sensor) = 0;
        std::string getTag() const { return Tag; }
        std::string getStatusAlarme() const { return StatusAlarme; }
        std::string getTimestamp() const { return Timestamp; }
        virtual ~EstrategiaAlarme() {}
};

class AlarmeTemperatura : public EstrategiaAlarme {
    public:
        AlarmeTemperatura(int limiteMinimo, int limiteMaximo, const std::string& tag);
        void verificar(Sensor* sensor) override;
};

class AlarmeNivel : public EstrategiaAlarme {
    public:
        AlarmeNivel(int limiteMinimo, int limiteMaximo, const std::string& tag);
        void verificar(Sensor* sensor) override;
};

class AlarmeRadiacao : public EstrategiaAlarme {
    public:
        AlarmeRadiacao(int limiteMaximo, const std::string& tag);
        void verificar(Sensor* sensor) override;
};

class AlarmeVazao : public EstrategiaAlarme {
    public:
        AlarmeVazao(int limiteMinimo, int limiteMaximo, const std::string& tag);
        void verificar(Sensor* sensor) override;
};

#endif