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


| 2026-06-09 | Claude Code | Criação de cabeçalho para o AI_LOG.md | Tabela de referência dos campos e tabela de registro em Markdown | -         | O formato em tabela Markdown atende ao padrão exigido pelo projeto |

| 2026-06-10 | ChatGPT     | Sugestão de estrutura para a classe base `Atuador` com padrão de herança | Criação da classe `Atuador` com atributos `Tag` e `ligado`, métodos virtuais `ligar()` e `desligar()`, e herança em `BombaAgua` e `Varetas` | Uso de `enum class` para estado do atuador | A equipe optou por `bool ligado` por ser mais direto e adequado ao nível da disciplina |

| 2026-06-10 | Claude Code | Implementação do padrão Strategy para as regras de controle da estação | Interface `EstrategiaControle` com método virtual puro `aplicar()` e classes concretas `ControleNivel` e `ControleTemperatura` | Adição de terceira estratégia para controle de vazão | Considerou-se suficiente duas estratégias para demonstrar o padrão; a de vazão ficaria sem uso nos testes atuais |

| 2026-06-12 | Claude Code | Separação de implementações inline nos headers em pares header/source corretos | Headers (`GeradorNumeros`, `Sensores`, `Atuadores`, `EstrategiaControle`) reduzidos a declarações; `.cpp` preenchidos com implementações completas; construtores parametrizados adicionados a todos os sensores; `virtual ~Sensor()` adicionado à classe base | Claude quis implementar funcionalidades que eu não pedi, apenas para preencher linha. | Padrão header/source é exigido pela disciplina e evita problemas de múltipla definição no linker |

| 2026-06-22 | Claude Code | Correcao da escrita dos alarmes no arquivo json | Adicionado timestamp e tag do sensor de forma correta no alarme | Rejeitado a alteracao do arquivo de Sensores | Claude queria alterar um parametro em sensores porem nao deixei pois complicava a logica |
