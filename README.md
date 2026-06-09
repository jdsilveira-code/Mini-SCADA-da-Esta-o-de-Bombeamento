# Mini-SCADA - Estação de Bombeamento Inteligente

Este projeto consiste em uma aplicação distribuída em duas camadas desenvolvida para simular, monitorar e controlar uma Estação de Bombeamento Inteligente. O sistema integra um dispositivo de campo de alta performance simulado em C++ e um supervisor de interface gráfica em Python com Streamlit.

## Identificação e Assinatura Operacional da Dupla

> **Nota de Rastreabilidade:** Os parâmetros abaixo guiam os testes unitários, limites de alarme e lógicas de falha em todo o código.

- **ID_DUPLA:** [Insira a soma dos dígitos das matrículas aqui]
- **Identificador da Estação:** EB-[Inserir ex: EB-17]
- **Limite Nível Baixo:** [Inserir entre 20% e 35%]%
- **Limite Nível Alto:** [Inserir entre 75% e 90%]%
- **Limite Pressão Alta:** [Inserir entre 5.0 bar e 8.0 bar] bar
- **Falha Simulada Customizada:** [Inserir ex: sensor de nível travado por 10 ciclos]
- **Regra de Controle Extra:** [Inserir ex: bloquear partida quando motor estiver em alerta]

---

## Arquitetura do Sistema e POO

O ecossistema está dividido estritamente em duas camadas funcionais, aplicando conceitos avançados de Programação Orientada a Objetos:

1. **Dispositivo de Campo (`dispositivo_cpp/`):** Desenvolvido em C++17. Contém a modelagem matemática e de estado dos sensores, bombas, gerador de falhas e o motor de regras de automação.
2. **Supervisor Central (`supervisor_python/`):** Desenvolvido em Python 3. Interfacia com os dados gerados via JSON Lines, gerencia a persistência histórica e provê o dashboard operacional para o operador.

### Padrões de Projeto Adotados (Design Patterns)

- **Strategy:** Isolamento das regras de controle, permitindo que a lógica de automação mude dinamicamente ou receba novas regras sem modificar a classe principal da estação.
- **Observer:** Desacoplamento do sistema de alarmes. Os sensores notificam os observadores registrados sem conhecer detalhes da interface ou logs.

---

## Divisão de Responsabilidades (Matriz de Trabalho)

- **Integrante A ([Nome/UserGitHub]):** Engenharia de software C++, desenvolvimento das classes de sensores, atuadores, motor de regras de controle e testes automatizados em C++.
- **Integrante B ([Nome/UserGitHub]):** Engenharia do supervisor em Python/Streamlit, arquitetura de persistência (CSV/SQLite), validação do contrato de dados e testes em Python.
- **Ambos Integrantes:** Modelagem do contrato JSON, arquitetura geral do sistema, documentação técnica e revisões cruzadas de Pull Requests.

---

## Contrato de Comunicação (JSON Lines)

Toda a troca de mensagens ou escrita de arquivos respeita rigorosamente a seguinte estrutura padrão:

```json
{
  "tag": "PT-01",
  "valor": 6.2,
  "unidade": "bar",
  "timestamp": "2026-06-08T10:00:00Z",
  "status": "OPERACIONAL"
}
```
