#include <iostream>
#include <iomanip>
#include "../include/EstrategiaControle.hpp"
#include "../include/Atuadores.hpp"
#include "../include/Sensores.hpp"

constexpr float NIVEL_MIN_PCT = 20.0f;
constexpr float NIVEL_MAX_PCT = 80.0f;
constexpr float TEMP_MAX_C    = 350.0f;
constexpr float DOSE_MIN_MSV  = 10.0f;
constexpr float DOSE_MAX_MSV  = 200.0f;

constexpr std::string TagNivel = "SNV-01";
constexpr std::string TagTemp = "STM-01";
constexpr std::string TagRadiacao = "SRD-01";
constexpr std::string TagVazao = "SVZ-01";

constexpr std::string TagBomba = "BAG-01";
constexpr std::string TagVaretas = "VAR-01";

constexpr int LimiteMinNivel = 0;
constexpr int LimiteMaxNivel = 100;
constexpr int LimiteMinTemp = 250;
constexpr int LimiteMaxTemp = 400;
constexpr int LimiteMinRadiacao = 0;
constexpr int LimiteMaxRadiacao = 50;
constexpr int LimiteMinVazao = 0;
constexpr int LimiteMaxVazao = 200;

const int CICLOS = 5;

static void printSeparador() {
    std::cout << std::string(50, '-') << "\n";
}

int main() {
    // Sensores
    SensorNivel    sNivel    (TagNivel, LimiteMaxNivel, LimiteMinNivel);
    SensorTemp     sTemp     (TagTemp, LimiteMaxTemp, LimiteMinTemp);
    SensorRadiacao sRadiacao (TagRadiacao, LimiteMaxRadiacao, LimiteMinRadiacao);
    SensorVazao    sVazao    (TagVazao, LimiteMaxVazao, LimiteMinVazao);

    // Atuadores
    BombaAgua bomba  (TagBomba);
    Varetas   varetas(TagVaretas);

    // Estratégias
    ControleNivel       ctrlNivel  (NIVEL_MIN_PCT, NIVEL_MAX_PCT);
    ControleTemperatura ctrlTemp   (TEMP_MAX_C);
    ControleQueima      ctrlQueima (DOSE_MIN_MSV, DOSE_MAX_MSV, &sTemp, TEMP_MAX_C);

    

    for (int ciclo = 1; ciclo <= CICLOS; ciclo++) {
        std::cout << "\n=== CICLO " << ciclo << " ===\n";
        printSeparador();

        // Leitura dos sensores
        sNivel.ler();
        sTemp.ler();
        sRadiacao.ler();
        sVazao.ler();

        // Print das leituras
        std::cout << "[" << sNivel.getTimestamp()    << "] "
                  << sNivel.getTag()    << " = " << sNivel.getValorAtual()    << " " << sNivel.getUnidadeMedida()    << "\n";
        std::cout << "[" << sTemp.getTimestamp()     << "] "
                  << sTemp.getTag()     << " = " << sTemp.getValorAtual()     << " " << sTemp.getUnidadeMedida()     << "\n";
        std::cout << "[" << sRadiacao.getTimestamp() << "] "
                  << sRadiacao.getTag() << " = " << sRadiacao.getValorAtual() << " " << sRadiacao.getUnidadeMedida()
                  << "  (dose acumulada: " << sRadiacao.getDoseAcumulada() << " mSv)\n";
        std::cout << "[" << sVazao.getTimestamp()    << "] "
                  << sVazao.getTag()    << " = " << sVazao.getValorAtual()    << " " << sVazao.getUnidadeMedida()    << "\n";

        printSeparador();

        // Aplicar estratégias
        ctrlNivel.aplicar(&sNivel, &bomba);
        ctrlTemp.aplicar(&sTemp, &bomba);
        ctrlQueima.aplicar(&sRadiacao, &varetas);

        // Print do estado dos atuadores
        std::cout << "Bomba  [" << bomba.getTag()   << "]: " << (bomba.isLigado()   ? "LIGADA"   : "DESLIGADA") << "\n";
        std::cout << "Varetas[" << varetas.getTag() << "]: " << (varetas.isLigado() ? "INSERIDAS" : "RETIRADAS") << "\n";
    }

    std::cout << "\n=== FIM DOS TESTES ===\n";
    return 0;
}
