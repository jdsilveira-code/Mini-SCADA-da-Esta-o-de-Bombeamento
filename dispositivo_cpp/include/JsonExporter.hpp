#ifndef JSON_EXPORTER_HPP
#define JSON_EXPORTER_HPP

#include <string>

// Forward declarations
class Sensor;
class EstrategiaAlarme;
class BombaAgua;
class Varetas;

class JsonExporter {
public:
    static std::string gerarJsonLeitura(Sensor& sensor);
    static std::string gerarJsonAlarme(EstrategiaAlarme& alarme);
    static std::string gerarJsonAtuadorBomba(BombaAgua& bomba, const std::string& timestamp);
    static std::string gerarJsonAtuadorVaretas(Varetas& varetas, const std::string& timestamp);
    static std::string gerarJsonFalha(const std::string& mensagem, float doseAcumulada, const std::string& timestamp);
};

#endif