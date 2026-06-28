# AI_LOG - Registro de Uso de Inteligência Artificial

O `AI_LOG.md` deve registrar:

| Campo        | O que escrever                              |
|--------------|---------------------------------------------|
| Data         | quando a IA foi usada                       |
| Ferramenta   | agente, chat, IDE ou outra ferramenta       |
| Pedido       | o que foi solicitado                        |
| Aceito       | o que entrou no projeto                     |
| Rejeitado    | o que foi descartado                        |
| Justificativa| por que a equipe aceitou ou rejeitou        |

---

## Registro de Uso

| Data | Ferramenta | Pedido | Aceito | Rejeitado | Justificativa |
|------|------------|--------|--------|-----------|---------------|
|      |            |        |        |           |               |
| 2026-06-28 | Grok | Corrigir conflito de nomes `AcaoVaretas` (tipo vs membro) em `EstrategiaManual` | Renomear o membro para `acaoVaretas` e ajustar referências | ❌ Nenhuma | Solução aceita integralmente e aplicada ao código |
| 2026-06-28 | Grok | Botão "Desligar bomba" não funcionava | ✅ Refatorar `lerUltimoComando()` para ler a última linha do arquivo | ❌ Usar `"w"` (sobrescrever) no `commands.jl` | Preferiu manter `"a"` (append) para preservar histórico no SQLite; ajuste no C++ resolveu o problema |
| 2026-06-28 | Grok | Implementar alerta visual para explosão no Streamlit | ✅ Banner com CSS animado e sirene | ❌ Nenhuma | Funcionalidade implementada conforme solicitado |
| 2026-06-28 | DeepSeek | Erro `invalid literal for int()` no `db_writer.py` | ✅ Tratar arquivos de posição vazios | ❌ Nenhuma | Correção robusta aplicada |
| 2026-06-28 | DeepSeek | Teste `test_validar_leituras` falhando | ✅ Ajustar o teste para esperar 4 inválidas | ❌ Nenhuma | Teste corrigido e passando |
| 2026-06-28 | DeepSeek | Erro de linkagem no GitHub Actions | ✅ Adicionar `src/EstadoReator.cpp` ao YAML e incluir header no teste | ❌ Nenhuma | Correção aplicada com sucesso |
| 2026-06-28 | DeepSeek | Isolar código do Streamlit no `app.py` para testes | ✅ Mover tudo que usa `st` para `if __name__ == "__main__":` | ❌ Nenhuma | Testes Python agora importam `app` sem executar a UI |
| 2026-06-28 | DeepSeek | Incluir `dose_acumulada` no JSON de leitura do `SensorRadiacao` | ✅ Manter a lógica dentro de `gerarJsonLeitura` com `dynamic_cast` | ❌ Modificar a classe base `Sensor` para incluir `dose_acumulada` | Preferiu manter a separação de responsabilidades; cada sensor gerencia seus próprios campos |
| 2026-06-28 | DeepSeek | Novos testes para aumentar cobertura | ✅ Implementar testes para `ControlePressao`, `EstadoReator`, `registrar_comando` e `extracao_dose_acumulada` | ❌ Nenhuma | Testes adicionados sem quebrar o código existente |
| 2026-06-28 | DeepSeek | Sugestão de usar `"w"` (sobrescrever) no `commands.jl` | ❌ Manter `"a"` (append) e ajustar leitura no C++ | ✅ Usar `"w"` no `commands.jl` para evitar comandos conflitantes | Optou por manter o histórico no arquivo e resolver a leitura no C++, o que funcionou melhor |
| 2026-06-28 | Copilot | Adicionar campo `tipo` nos JSONs de leitura para facilitar filtros | ✅ Manter a lógica atual (sem `tipo` para leituras normais) | ❌ Adicionar `"tipo": "leitura"` nos JSONs | A separação já é feita pelo conteúdo do JSON; não houve necessidade de adicionar um campo extra |
| 2026-06-09 | Claude Code | Criação de cabeçalho para o AI_LOG.md | Tabela de referência dos campos e tabela de registro em Markdown | -         | O formato em tabela Markdown atende ao padrão exigido pelo projeto |
| 2026-06-10 | ChatGPT     | Sugestão de estrutura para a classe base `Atuador` com padrão de herança | Criação da classe `Atuador` com atributos `Tag` e `ligado`, métodos virtuais `ligar()` e `desligar()`, e herança em `BombaAgua` e `Varetas` | Uso de `enum class` para estado do atuador | A equipe optou por `bool ligado` por ser mais direto e adequado ao nível da disciplina |
| 2026-06-10 | Claude Code | Implementação do padrão Strategy para as regras de controle da estação | Interface `EstrategiaControle` com método virtual puro `aplicar()` e classes concretas `ControleNivel` e `ControleTemperatura` | Adição de terceira estratégia para controle de vazão | Considerou-se suficiente duas estratégias para demonstrar o padrão; a de vazão ficaria sem uso nos testes atuais |
| 2026-06-12 | Claude Code | Separação de implementações inline nos headers em pares header/source corretos | Headers (`GeradorNumeros`, `Sensores`, `Atuadores`, `EstrategiaControle`) reduzidos a declarações; `.cpp` preenchidos com implementações completas; construtores parametrizados adicionados a todos os sensores; `virtual ~Sensor()` adicionado à classe base | Claude quis implementar funcionalidades que eu não pedi, apenas para preencher linha. | Padrão header/source é exigido pela disciplina e evita problemas de múltipla definição no linker |
| 2026-06-22 | Claude Code | Correcao da escrita dos alarmes no arquivo json | Adicionado timestamp e tag do sensor de forma correta no alarme | Rejeitado a alteracao do arquivo de Sensores | Claude queria alterar um parametro em sensores porem nao deixei pois complicava a logica |
| 2026-06-28 | Gemini | Criar sinalização visual para alarmes críticos no SCADA Streamlit. | Injeção de Banner Fixo/Pulsante no topo com acionamento de Sirene Industrial em loop via HTML. | Alteração global da cor de fundo (piscar a tela toda). | Evita a fadiga visual do operador e garante maior clareza contextual sobre a tag em falha. |
| 2026-06-28 | Gemini | Prototipar e homologar comportamento do áudio e layout de emergência. | Desenvolvimento de ambiente isolado (mock) via st.session_state para simulação de falhas. | Integração direta em produção sem validação prévia. | Necessário para validar e mitigar o bloqueio nativo de autoplay de áudio dos navegadores. |
| 2026-06-28 | Gemini | Acoplar a lógica do alarme aos dados em tempo real do sistema. | Implementação da função exibir_alerta_emergencia vinculada à varredura do arquivo readings.jl. | - | Garante que o painel responda dinamicamente às falhas críticas injetadas pelo backend em C++. |
