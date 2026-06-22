#ifndef JSON_EXPORTER_HPP
#define JSON_EXPORTER_HPP

#include <string>

class Sensor;
class EstrategiaAlarme;

class JsonExporter {
public:
    static std::string gerarJsonLeitura(Sensor& sensor);
    static std::string gerarJsonAlarme(EstrategiaAlarme& alarme);
};

#endif // JSON_EXPORTER_HPP
