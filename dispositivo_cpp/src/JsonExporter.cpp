#include "JsonExporter.hpp"
#include <sstream>

std::string JsonExporter::gerarJsonLeitura(Sensor& sensor) {
	std::ostringstream json;
	json << "{"
		 << "\"tag\": \"" << sensor.getTag() << "\", "
		 << "\"valor\": " << sensor.getValorAtual() << ", "
		 << "\"unidade\": \"" << sensor.getUnidadeMedida() << "\", "
		 << "\"timestamp\": \"" << sensor.getTimestamp() << "\", "
		 << "\"status\": \"" << sensor.getStatus() << "\""
		 << "}";
	return json.str();
}
