# 🚰 Mini-SCADA - Estação de Bombeamento Inteligente

[cite_start]Este projeto consiste em uma aplicação distribuída em duas camadas desenvolvida para simular, monitorar e controlar uma Estação de Bombeamento Inteligente[cite: 3]. [cite_start]O sistema integra um dispositivo de campo de alta performance simulado em C++ e um supervisor de interface gráfica em Python com Streamlit[cite: 5, 6].

## 👥 Identificação e Assunatura Operacional da Dupla
> [cite_start]**Nota de Rastreabilidade:** Os parâmetros abaixo guiam os testes unitários, limites de alarme e lógicas de falha em todo o código[cite: 27, 99].

* [cite_start]**ID_DUPLA:** [Insira a soma dos dígitos das matrículas aqui] [cite: 20]
* **Identificador da Estação:** EB-[Inserir ex: EB-17] [cite: 24, 26]
* [cite_start]**Limite Nível Baixo:** [Inserir entre 20% e 35%]% [cite: 24]
* [cite_start]**Limite Nível Alto:** [Inserir entre 75% e 90%]% [cite: 24]
* **Limite Pressão Alta:** [Inserir entre 5.0 bar e 8.0 bar] bar [cite: 24]
* [cite_start]**Falha Simulada Customizada:** [Inserir ex: sensor de nível travado por 10 ciclos] [cite: 24, 26]
* [cite_start]**Regra de Controle Extra:** [Inserir ex: bloquear partida quando motor estiver em alerta] [cite: 24, 26]

---

## 🏗️ Arquitetura do Sistema e POO

[cite_start]O ecossistema está dividido estritamente em duas camadas funcionais, aplicando conceitos avançados de Programação Orientada a Objetos[cite: 3, 10]:

1. **Dispositivo de Campo (`dispositivo_cpp/`):** Desenvolvido em C++17. [cite_start]Contém a modelagem matemática e de estado dos sensores, bombas, gerador de falhas e o motor de regras de automação[cite: 5, 10, 48].
2. [cite_start]**Supervisor Central (`supervisor_python/`):** Desenvolvido em Python 3. Interfacia com os dados gerados via JSON Lines, gerencia a persistência histórica e provê o dashboard operacional para o operador[cite: 6, 7].

### Padrões de Projeto Adotados (Design Patterns)
* [cite_start]**Strategy:** Isolamento das regras de controle, permitindo que a lógica de automação mude dinamicamente ou receba novas regras sem modificar a classe principal da estação[cite: 51].
* **Observer:** Desacoplamento do sistema de alarmes. Os sensores notificam os observadores registrados sem conhecer detalhes da interface ou logs[cite: 51].

---

## 🤝 Divisão de Responsabilidades (Matriz de Trabalho)
* [cite_start]**Integrante A ([Nome/UserGitHub]):** Engenharia de software C++, desenvolvimento das classes de sensores, atuadores, motor de regras de controle e testes automatizados em C++[cite: 90].
* [cite_start]**Integrante B ([Nome/UserGitHub]):** Engenharia do supervisor em Python/Streamlit, arquitetura de persistência (CSV/SQLite), validação do contrato de dados e testes em Python[cite: 90].
* **Ambos Integrantes:** Modelagem do contrato JSON, arquitetura geral do sistema, documentação técnica e revisões cruzadas de Pull Requests[cite: 90].

---

## 📊 Contrato de Comunicação (JSON Lines)
Toda a troca de mensagens ou escrita de arquivos respeita rigorosamente a seguinte estrutura padrão[cite: 7, 71]:

```json
{
  "tag": "PT-01",
  "valor": 6.2,
  "unidade": "bar",
  "timestamp": "2026-06-08T10:00:00Z",
  "status": "OPERACIONAL"
}
