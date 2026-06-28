#include <filesystem>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <thread>
#include <chrono>
#include <string>
#if defined(_WIN32)
#include <windows.h>
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
constexpr int LimiteMinPressao = 0;
constexpr int LimiteMaxPressao = 200;

// Tags dos sensores
const std::string TagNivel = "SNV-01";
const std::string TagTemp = "STM-01";
const std::string TagRadiacao = "SRD-01";
const std::string TagPressao = "SPR-01";

// Tags dos atuadores
const std::string TagBomba = "BAG-01";
const std::string TagVaretas = "VAR-01";

// Constantes de configuração dos limites de operação dos alarmes.
constexpr int LimiteMinAlarmeNivel = 30;
constexpr int LimiteMaxAlarmeNivel = 99;

constexpr int LimiteMinAlarmeTemp = 300;
constexpr int LimiteMaxAlarmeTemp = 350;
constexpr int LimiteMinAlarmeRadiacao = 10;
constexpr int LimiteMaxAlarmeRadiacao = 40;
constexpr int LimiteMinAlarmePressao = 10;
constexpr int LimiteMaxAlarmePressao = 180;

static void printSeparador() {
    std::cout << std::string(50, '-') << "\n";
}

// Retorna o caminho absoluto para output/ na raiz do projeto
static std::filesystem::path getOutputDir() {
    // __FILE__: dispositivo_cpp/src/main.cpp → sobe 3 níveis → raiz do projeto
    std::filesystem::path srcFile = std::filesystem::absolute(__FILE__);
    return srcFile.parent_path()   // src/
                  .parent_path()   // dispositivo_cpp/
                  .parent_path()   // raiz do projeto
           / "output";
}

static bool criarDiretorioOutput(const std::filesystem::path& dir) {
    std::error_code ec;
    std::filesystem::create_directories(dir, ec);
    if (ec) {
        std::cerr << "Erro ao criar diretorio output: " << ec.message() << "\n";
        return false;
    }
    return true;
}

// Considere que 30 segundos são 30 minutos
static void pausarEntreCiclos() {
#if defined(_WIN32)
    Sleep(5000);
#else
    std::this_thread::sleep_for(std::chrono::seconds(5));
#endif
}

static std::string lerUltimoComandoVaretas() {
    std::ifstream arquivo("output/commands.jl");
    std::string linha;
    std::string ultimaAcao;

    if (!arquivo) {
        return ultimaAcao;
    }

    while (std::getline(arquivo, linha)) {
        if (linha.find("VAR-01") == std::string::npos) {
            continue;
        }

        if (linha.find("AUTOMATICO") != std::string::npos) {
            ultimaAcao = "AUTOMATICO";
        } else if (linha.find("INSERIR") != std::string::npos) {
            ultimaAcao = "INSERIR";
        } else if (linha.find("RETIRAR") != std::string::npos) {
            ultimaAcao = "RETIRAR";
        }
    }

    return ultimaAcao;
}

static std::string lerUltimoComandoBomba() {
    std::ifstream arquivo("output/commands.jl");
    std::string linha;
    std::string ultimaAcao;

    if (!arquivo) {
        return ultimaAcao;
    }

    while (std::getline(arquivo, linha)) {
        if (linha.find("BAG-01") == std::string::npos) {
            continue;
        }

        if (linha.find("AUTOMATICO") != std::string::npos) {
            ultimaAcao = "AUTOMATICO";
        } else if (linha.find("LIGAR") != std::string::npos) {
            ultimaAcao = "LIGAR";
        } else if (linha.find("DESLIGAR") != std::string::npos) {
            ultimaAcao = "DESLIGAR";
        }
    }

    return ultimaAcao;
}

static void aplicarComandoSupervisor(Varetas& varetas, EstrategiaManual& estrManual, EstrategiaAutomatica& estrAuto, SensorRadiacao& sRad) {
    const std::string acao = lerUltimoComandoVaretas();

    if (acao == "INSERIR") {
        estrManual.setAcaoVaretas(AcaoVaretas::INSERIR);
        estrManual.aplicar(&sRad, &varetas);
        std::cout << "Comando supervisor: varetas INSERIDAS\n";
    } else if (acao == "RETIRAR") {
        estrManual.setAcaoVaretas(AcaoVaretas::RETIRAR);
        estrManual.aplicar(&sRad, &varetas);
        std::cout << "Comando supervisor: varetas RETIRADAS\n";
    } else if (acao == "AUTOMATICO") {
        std::cout << "Comando supervisor: varetas em controle automatico\n";
    }
}

static void aplicarComandoSupervisorBomba(
    BombaAgua& bomba,
    ControleBombaManual& controleManual,
    ControleBombaAutomatico& controleAutomatico
) {
    const std::string acao = lerUltimoComandoBomba();

    if (acao == "LIGAR") {
        controleManual.setAcao(AcaoBomba::LIGAR);
        controleManual.aplicar(nullptr, &bomba);
        std::cout << "Comando supervisor: bomba LIGADA manualmente\n";
    } else if (acao == "DESLIGAR") {
        controleManual.setAcao(AcaoBomba::DESLIGAR);
        controleManual.aplicar(nullptr, &bomba);
        std::cout << "Comando supervisor: bomba DESLIGADA manualmente\n";
    } else {
        controleAutomatico.aplicar(nullptr, &bomba);
        std::cout << "Comando supervisor: bomba em controle automatico\n";
    }
}

int main() {
    const std::filesystem::path outputDir = getOutputDir();
    criarDiretorioOutput(outputDir);

    // Atuadores
    BombaAgua bomba  (TagBomba);
    Varetas   varetas(TagVaretas);

    // Sensores
    SensorNivel    sNivel    (TagNivel, LimiteMaxNivel, LimiteMinNivel);
    SensorTemp     sTemp     (TagTemp, LimiteMaxTemp, LimiteMinTemp);
    SensorRadiacao sRad (TagRadiacao, LimiteMaxRadiacao, LimiteMinRadiacao, &varetas);
    SensorPressao  sPressao  (TagPressao, LimiteMaxPressao, LimiteMinPressao);

    // Estratégias
    ControleNivel       CtrlNivel  (NIVEL_MIN_PCT, NIVEL_MAX_PCT);
    ControleTemperatura CtrlTemp   (TEMP_MAX_K);
    ControlePressao     CtrlPress  (0.0f, 180.0f);
    ControleBombaManual CtrlBombaManual;
    ControleBombaAutomatico CtrlBombaAutomatico(
        CtrlNivel,
        CtrlTemp,
        CtrlPress,
        &sNivel,
        &sTemp,
        &sPressao
    );
    ControleQueima      CtrlQueima (DOSE_MIN_MSV, DOSE_MAX_MSV, &sTemp, TEMP_MAX_K);

    EstrategiaManual estrManual;
    EstrategiaAutomatica estrAuto(CtrlBombaAutomatico, CtrlQueima);


    // Alarmes
    AlarmeNivel AlarmeNivel(LimiteMinAlarmeNivel, LimiteMaxAlarmeNivel, sNivel.getTag());
    AlarmeTemperatura AlarmeTemp(LimiteMinAlarmeTemp, LimiteMaxAlarmeTemp, sTemp.getTag());
    AlarmeRadiacao AlarmeRad(LimiteMaxAlarmeRadiacao, sRad.getTag());
    AlarmePressao AlarmePressaoInstancia(LimiteMinAlarmePressao, LimiteMaxAlarmePressao, sPressao.getTag());

    int ciclo = 0;

    // Loop principal de simulação, representando os ciclos de operação do sistema.
    while (true) {
        std::cout << "\n=== CICLO " << ciclo << " ===\n";

        printSeparador();

        // Leitura dos sensores
        sNivel.ler();
        sTemp.ler();
        sRad.ler();
        sPressao.ler();

        AlarmeNivel.verificar(&sNivel);
        AlarmeTemp.verificar(&sTemp);
        AlarmeRad.verificar(&sRad);
        AlarmePressaoInstancia.verificar(&sPressao);
        
        // Aplicar estratégias
        aplicarComandoSupervisorBomba(bomba, CtrlBombaManual, CtrlBombaAutomatico);
        CtrlQueima.aplicar(&sRad, &varetas);
        aplicarComandoSupervisor(varetas, estrManual, estrAuto, sRad);
        
        // Print das leituras
        std::cout << "[" << sNivel.getTimestamp()    << "] "
                  << sNivel.getTag()    << " = " << sNivel.getValorAtual()    << " " << sNivel.getUnidadeMedida()    << "\n";
        std::cout << "[" << sTemp.getTimestamp()     << "] "
                  << sTemp.getTag()     << " = " << sTemp.getValorAtual()     << " " << sTemp.getUnidadeMedida()     << "\n";
        std::cout << "[" << sRad.getTimestamp() << "] "
                  << sRad.getTag() << " = " << sRad.getValorAtual() << " " << sRad.getUnidadeMedida()
                  << "  (dose acumulada: " << sRad.getDoseAcumulada() << " mSv)\n";
        std::cout << "[" << sPressao.getTimestamp()  << "] "
                  << sPressao.getTag()  << " = " << sPressao.getValorAtual()  << " " << sPressao.getUnidadeMedida()  << "\n";

        printSeparador();
        // Print do estado dos atuadores
        std::cout << "Bomba  [" << bomba.getTag()   << "]: " << (bomba.isLigado()   ? "LIGADA"   : "DESLIGADA") << "\n";
        std::cout << "Varetas[" << varetas.getTag() << "]: " << (varetas.isLigado() ? "RETIRADAS" : "INSERIDAS") << "\n";
        
        printSeparador();
        // Print do estado dos alarmes
        std::cout << "Alarme [" << AlarmeNivel.getTag()         << "]: " << AlarmeNivel.getStatusAlarme()         << "\n";
        std::cout << "Alarme [" << AlarmeTemp.getTag()   << "]: " << AlarmeTemp.getStatusAlarme()   << "\n";
        std::cout << "Alarme [" << AlarmeRad.getTag()      << "]: " << AlarmeRad.getStatusAlarme()      << "\n";
        std::cout << "Alarme [" << AlarmePressaoInstancia.getTag()       << "]: " << AlarmePressaoInstancia.getStatusAlarme()       << "\n";

        // Gravar leituras em JSON Lines
        std::ofstream ofs((outputDir / "readings.jl").string(), std::ios::app);
        if (ofs) {
            ofs << JsonExporter::gerarJsonLeitura(sNivel) << '\n';
            ofs << JsonExporter::gerarJsonLeitura(sTemp) << '\n';
            ofs << JsonExporter::gerarJsonLeitura(sRad) << '\n';
            ofs << JsonExporter::gerarJsonLeitura(sPressao) << '\n';
            ofs << JsonExporter::gerarJsonAlarme(AlarmeNivel) << '\n';
            ofs << JsonExporter::gerarJsonAlarme(AlarmeTemp) << '\n';
            ofs << JsonExporter::gerarJsonAlarme(AlarmeRad) << '\n';
            ofs << JsonExporter::gerarJsonAlarme(AlarmePressaoInstancia) << '\n';
            ofs << JsonExporter::gerarJsonAtuadorBomba(bomba, sNivel.getTimestamp()) << '\n';
            ofs << JsonExporter::gerarJsonAtuadorVaretas(varetas, sNivel.getTimestamp()) << '\n';
            ofs.close();
        } else {
            std::cerr << "Erro ao abrir arquivo " << (outputDir / "readings.jl").string() << " para escrita\n";
        }

        ciclo++;
        pausarEntreCiclos();
    }

    std::cout << "\n=== FIM DOS TESTES ===\n";
    return 0;
}
