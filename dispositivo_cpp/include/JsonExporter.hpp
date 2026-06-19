#ifndef JSON_EXPORTER_HPP
#define JSON_EXPORTER_HPP

#include <string>
#include "Sensores.hpp"

class JsonExporter {
public:
    static std::string gerarJsonLeitura(Sensor& sensor);
};

#endif // JSON_EXPORTER_HPP
