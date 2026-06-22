#ifndef JSON_EXPORTER_HPP
#define JSON_EXPORTER_HPP

#include <string>
#include "Sensores.hpp"

class EstrategiaAlarme;

class JsonExporter {
public:
    static std::string gerarJsonLeitura(Sensor& sensor);
    static std::string gerarJsonAlarme(EstrategiaAlarme& alarme);
};

#endif // JSON_EXPORTER_HPP
