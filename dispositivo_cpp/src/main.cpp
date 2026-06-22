#include <iostream>
#include <iomanip>
#include <fstream>
#include <thread>
#include <chrono>
#include <cerrno>
#if defined(_WIN32)
#include <direct.h>
#include <windows.h>
#else
#include <sys/stat.h>
#endif
#include "../include/EstrategiaControle.hpp"
#include "../include/Atuadores.hpp"
#include "../include/Sensores.hpp"
#include "../include/Alarmes.hpp"
#include "../include/JsonExporter.hpp"

// Constantes de configuração dos limites de controle do EstrategiaControle.
constexpr float NIVEL_MIN_PCT = 20.0f;
constexpr float NIVEL_MAX_PCT = 80.0f;
constexpr float TEMP_MAX_K    = 350.0f;
constexpr float DOSE_MIN_MSV  = 50.0f;
constexpr float DOSE_MAX_MSV  = 200.0f;

// Constantes de configuração dos limites de operção dos sensores.
constexpr int LimiteMinNivel = 0;
constexpr int LimiteMaxNivel = 100;
constexpr int LimiteMinTemp = 250;
constexpr int LimiteMaxTemp = 400;
constexpr int LimiteMinRadiacao = 0;
constexpr int LimiteMaxRadiacao = 50;
constexpr int LimiteMinVazao = 0;
constexpr int LimiteMaxVazao = 200;

// Tags dos sensores
const std::string TagNivel = "SNV-01";
const std::string TagTemp = "STM-01";
const std::string TagRadiacao = "SRD-01";
const std::string TagVazao = "SVZ-01";

// Tags dos atuadores
const std::string TagBomba = "BAG-01";
const std::string TagVaretas = "VAR-01";

// Constantes de configuração dos limites de operação dos alarmes.
constexpr int LimiteMinAlarmeTemp = 300;
constexpr int LimiteMaxAlarmeTemp = 350;
constexpr int LimiteMinAlarmeNivel = 30;
constexpr int LimiteMaxAlarmeNivel = 99;
constexpr int LimiteMinAlarmeRadiacao = 0;
constexpr int LimiteMaxAlarmeRadiacao = 40;
constexpr int LimiteMinAlarmeVazao = 0;
constexpr int LimiteMaxAlarmeVazao = 180;

static void printSeparador() {
    std::cout << std::string(50, '-') << "\n";
}

// Função para criar o diretório "output" se ele não existir, garantindo que o programa possa salvar os arquivos de leitura.
static bool criarDiretorioOutput() {
#if defined(_WIN32)
    if (_mkdir("output") == 0 || errno == EEXIST) {
        return true;
    }
#else
    if (mkdir("output", 0755) == 0 || errno == EEXIST) {
        return true;
    }
#endif

    std::cerr << "Erro ao criar diretorio output\n";
    return false;
}

// Considere que 30 segundos são 30 minutos
static void pausarEntreCiclos() {
#if defined(_WIN32)
    Sleep(5000);
#else
    std::this_thread::sleep_for(std::chrono::seconds(30));
#endif
}

int main() {
    // Criar diretório de saída para os arquivos de leitura
    criarDiretorioOutput();

    // Atuadores
    BombaAgua bomba  (TagBomba);
    Varetas   varetas(TagVaretas);

    // Sensores
    SensorNivel    sNivel    (TagNivel, LimiteMaxNivel, LimiteMinNivel);
    SensorTemp     sTemp     (TagTemp, LimiteMaxTemp, LimiteMinTemp);
    SensorRadiacao sRadiacao (TagRadiacao, LimiteMaxRadiacao, LimiteMinRadiacao, &varetas);
    SensorVazao    sVazao    (TagVazao, LimiteMaxVazao, LimiteMinVazao);

    // Estratégias
    ControleNivel       ctrlNivel  (NIVEL_MIN_PCT, NIVEL_MAX_PCT);
    ControleTemperatura ctrlTemp   (TEMP_MAX_K);
    ControleQueima      ctrlQueima (DOSE_MIN_MSV, DOSE_MAX_MSV, &sTemp, TEMP_MAX_K);

    // Alarmes
    AlarmeNivel AlarmeNivel(LimiteMinAlarmeNivel, LimiteMaxAlarmeNivel, sNivel.getTag());
    AlarmeTemperatura AlarmeTemperatura(LimiteMinAlarmeTemp, LimiteMaxAlarmeTemp, sTemp.getTag());
    AlarmeRadiacao AlarmeRadiacao(LimiteMaxAlarmeRadiacao, sRadiacao.getTag());
    AlarmeVazao AlarmeVazao(LimiteMinAlarmeVazao, LimiteMaxAlarmeVazao, sVazao.getTag());

    int ciclo = 0;

    // Loop principal de simulação, representando os ciclos de operação do sistema.
    while (true) {
        std::cout << "\n=== CICLO " << ciclo << " ===\n";

        printSeparador();

        // Leitura dos sensores
        sNivel.ler();
        sTemp.ler();
        sRadiacao.ler();
        sVazao.ler();

        AlarmeNivel.verificar(&sNivel);
        AlarmeTemperatura.verificar(&sTemp);
        AlarmeRadiacao.verificar(&sRadiacao);
        AlarmeVazao.verificar(&sVazao);
        
        // Aplicar estratégias
        ctrlNivel.aplicar(&sNivel, &bomba);
        ctrlTemp.aplicar(&sTemp, &bomba);
        ctrlQueima.aplicar(&sRadiacao, &varetas);
        
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
        // Print do estado dos atuadores
        std::cout << "Bomba  [" << bomba.getTag()   << "]: " << (bomba.isLigado()   ? "LIGADA"   : "DESLIGADA") << "\n";
        std::cout << "Varetas[" << varetas.getTag() << "]: " << (varetas.isLigado() ? "RETIRADAS" : "INSERIDAS") << "\n";
        
        printSeparador();
        // Print do estado dos alarmes
        std::cout << "Alarme [" << AlarmeNivel.getTag()         << "]: " << AlarmeNivel.getStatusAlarme()         << "\n";
        std::cout << "Alarme [" << AlarmeTemperatura.getTag()   << "]: " << AlarmeTemperatura.getStatusAlarme()   << "\n";
        std::cout << "Alarme [" << AlarmeRadiacao.getTag()      << "]: " << AlarmeRadiacao.getStatusAlarme()      << "\n";
        std::cout << "Alarme [" << AlarmeVazao.getTag()         << "]: " << AlarmeVazao.getStatusAlarme()         << "\n";

        // Gravar leituras em JSON Lines
        std::ofstream ofs("output/readings.jl", std::ios::app);
        if (ofs) {
            ofs << JsonExporter::gerarJsonLeitura(sNivel) << '\n';
            ofs << JsonExporter::gerarJsonLeitura(sTemp) << '\n';
            ofs << JsonExporter::gerarJsonLeitura(sRadiacao) << '\n';
            ofs << JsonExporter::gerarJsonLeitura(sVazao) << '\n';
            ofs << JsonExporter::gerarJsonAlarme(AlarmeNivel) << '\n';
            ofs << JsonExporter::gerarJsonAlarme(AlarmeTemperatura) << '\n';
            ofs << JsonExporter::gerarJsonAlarme(AlarmeRadiacao) << '\n';
            ofs << JsonExporter::gerarJsonAlarme(AlarmeVazao) << '\n';
            ofs.close();
        } else {
            std::cerr << "Erro ao abrir arquivo output/readings.jl para escrita\n";
        }

        ciclo++;
        pausarEntreCiclos();
    }

    std::cout << "\n=== FIM DOS TESTES ===\n";
    return 0;
}
