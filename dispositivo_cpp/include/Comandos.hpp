#ifndef COMANDO_HPP
#define COMANDO_HPP

#include "estrategia_controle.hpp"
#include <iostream>

// Classe central que representa a estacao de bombeamento.
// Agrega sensores, atuadores e aplica a estrategia de controle.
class EstacaoBombeamento {
    private:
        SensorNivel   sensorNivel;
        SensorTemp    sensorTemp;
        SensorVazao   sensorVazao;
        SensorRadiacao sensorRadiacao;

        BombaAgua bomba;
        Varetas   varetas;

        EstrategiaControle* estrategia{nullptr};

    public:
        EstacaoBombeamento()
            : bomba("P-01"), varetas("VL-01") {
            bomba.vincularSensor(&sensorNivel);
        }

        void setEstrategia(EstrategiaControle* nova) {
            estrategia = nova;
        }

        // Executa um ciclo completo: leitura dos sensores + controle
        void ciclo() {
            sensorNivel.ler();
            sensorTemp.ler();
            sensorVazao.ler();
            sensorRadiacao.ler();

            if (estrategia != nullptr) {
                estrategia->aplicar(&sensorNivel, &bomba);
            }
        }

        void exibirLeituras() {
            std::cout << "Nivel: "    << sensorNivel.getValorAtual()    << " %\n";
            std::cout << "Temp: "     << sensorTemp.getValorAtual()     << " C\n";
            std::cout << "Vazao: "    << sensorVazao.getValorAtual()    << " m3/h\n";
            std::cout << "Radiacao: " << sensorRadiacao.getValorAtual() << " mSv/h\n";
            std::cout << "Bomba: "    << (bomba.isLigado() ? "LIGADA" : "DESLIGADA") << "\n";
        }

        // Getters para acesso externo
        BombaAgua& getBomba()   { return bomba; }
        Varetas&   getVarietas(){ return varetas; }

        SensorNivel&    getSensorNivel()    { return sensorNivel; }
        SensorTemp&     getSensorTemp()     { return sensorTemp; }
        SensorVazao&    getSensorVazao()    { return sensorVazao; }
        SensorRadiacao& getSensorRadiacao() { return sensorRadiacao; }
};

#endif // COMANDO_HPP
