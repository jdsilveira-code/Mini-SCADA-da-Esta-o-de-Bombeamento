#include "JsonExporter.hpp"
#include "Sensores.hpp"
#include "Alarmes.hpp"
#include "Atuadores.hpp"
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

std::string JsonExporter::gerarJsonAtuadorBomba(BombaAgua& bomba, const std::string& timestamp) {
    std::ostringstream json;
    json << "{"
         << "\"tipo\": \"atuador\", "
         << "\"tag\": \"" << bomba.getTag() << "\", "
         << "\"estado\": \"" << (bomba.isLigado() ? "LIGADA" : "DESLIGADA") << "\", "
         << "\"potencia\": " << bomba.getPotencia() << ", "
         << "\"timestamp\": \"" << timestamp << "\""
         << "}";
    return json.str();
}

std::string JsonExporter::gerarJsonAtuadorVaretas(Varetas& varetas, const std::string& timestamp) {
    std::ostringstream json;
    json << "{"
         << "\"tipo\": \"atuador\", "
         << "\"tag\": \"" << varetas.getTag() << "\", "
         << "\"estado\": \"" << (varetas.isLigado() ? "RETIRADAS" : "INSERIDAS") << "\", "
         << "\"valor\": " << varetas.getValorAtual() << ", "
         << "\"timestamp\": \"" << timestamp << "\""
         << "}";
    return json.str();
}

std::string JsonExporter::gerarJsonFalha(const std::string& mensagem, float doseAcumulada, const std::string& timestamp) {
    std::ostringstream json;
    json << "{"
         << "\"tipo\": \"falha\", "
         << "\"severidade\": \"CRITICA\", "
         << "\"mensagem\": \"" << mensagem << "\", "
         << "\"dose_acumulada\": " << doseAcumulada << ", "
         << "\"timestamp\": \"" << timestamp << "\""
         << "}";
    return json.str();
}