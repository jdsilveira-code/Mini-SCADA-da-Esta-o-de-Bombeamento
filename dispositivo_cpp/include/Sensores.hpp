#ifndef SENSORES_HPP
#define SENSORES_HPP

#include <string>
#include "GeradorNumeros.hpp"

// Forward declaration para reduzir dependência: usamos apenas a interface de estado
class IVaretasState;

class Sensor {
    protected:
        std::string Tag;
        std::string UnidadeMedida;
        std::string Timestamp;
        std::string Status{"OPERACIONAL"};
        GeradorAleatorio gerador;
        int ValorMax;
        int ValorMin;
        void atualizarTimestamp();

    public:
        virtual void ler() = 0;
        virtual int getValorAtual() const = 0;
        std::string getTag();
        std::string getUnidadeMedida();
        std::string getTimestamp();
        std::string getStatus();
        void setStatus(std::string novoStatus);
        virtual void calibrar();
        virtual ~Sensor() {}
};

class SensorNivel : public Sensor {
    private:
        int AlturaAtual;
        float RaioTanque;
        float VolumeTotalMax;
    public:
        SensorNivel(std::string tag, int max, int min);
        void ler() override;
        int getValorAtual() const override;

        void CalcularVolume(int altura, float raio);
};

class SensorRadiacao : public Sensor {
    private:
        int NivelRadiacaoAtual;
        float DoseAcumulada;
        float LimiteDoseAcumulada;
        IVaretasState* varetasState{nullptr};
        bool varetasRetiradas{false};
    public:
        SensorRadiacao(std::string tag, int max, int min, IVaretasState* atuador);
        void ler() override;
        int getValorAtual() const override;
        void AcumularDose(int nivel,std::string timestamp);
        virtual float getDoseAcumulada() const;

};

class SensorTemp : public Sensor {
    private:
        int TempKelvin;
        float TempCelsius;
    public:
        SensorTemp(std::string tag, int max, int min);
        void ler() override;
        int getValorAtual() const override;

        void ConverterKelvinCelsius(int tempKelvin);
};

class SensorPressao : public Sensor {
    private:
        int PressaoAtual;
    public:
        SensorPressao(std::string tag, int max, int min);
        void ler() override;
        int getValorAtual() const override;
};

#endif // SENSORES_HPP
