#include <iostream>
#include <iomanip>
#include "../include/EstrategiaControle.hpp"
#include "../include/Atuadores.hpp"
#include "../include/Sensores.hpp"

static void printSeparador() {
    std::cout << std::string(50, '-') << "\n";
}

int main() {
    // Sensores
    SensorNivel    sNivel    ("SNV-01", 100, 0);
    SensorTemp     sTemp     ("STM-01", 400, 250);
    SensorRadiacao sRadiacao ("SRD-01",  50,   0);
    SensorVazao    sVazao    ("SVZ-01", 200,   0);

    // Atuadores
    BombaAgua bomba  ("BAG-01");
    Varetas   varetas("VAR-01");

    // Estratégias
    ControleNivel       ctrlNivel    (20.0f, 80.0f);
    ControleTemperatura ctrlTemp     (350.0f);
    ControleQueima      ctrlQueima   (10.0f, 200.0f, &sTemp, 350.0f);

    const int CICLOS = 5;

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
