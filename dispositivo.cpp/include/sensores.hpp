#include <string>

class Sensor{
    private:
        float ValorAtual;
        std::string Tag;
        std::string UnidadeMedida;

    public:
        virtual void ler() {}

};

class SensorNivel : public Sensor{
    private:
        float ValorMax;
        float ValorMin;
    public:
        void ler() override{}
};

class SensorRadiacao : public Sensor{
    private:
        float ValorMax;
        float ValorMin;
    public:
        void ler() override{}
};

class SensorTemp : public Sensor{
    private:
        float ValorMax;
        float ValorMin;
    public:
        void ler() override{}
};

class SensorVazao : public Sensor{
    private:
        float ValorMax;
        float ValorMin;
    public:
        void ler() override{}
};