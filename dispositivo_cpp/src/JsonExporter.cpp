#include "JsonExporter.hpp"
#include "Alarmes.hpp"
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

std::string JsonExporter::gerarJsonAlarme(EstrategiaAlarme& alarme) {
	std::ostringstream json;
	json << "{"
		 << "\"tag\": \"" << alarme.getTag() << "\", "
		 << "\"tipo\": \"alarme\", "
		 << "\"status\": \"" << alarme.getStatusAlarme() << "\", "
		 << "\"timestamp\": \"" << alarme.getTimestamp() << "\""
		 << "}";
	return json.str();
}
