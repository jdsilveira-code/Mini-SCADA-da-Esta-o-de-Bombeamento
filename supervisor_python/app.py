import json
from datetime import datetime, timezone
from pathlib import Path

import pandas as pd
import streamlit as st


try:
    from streamlit_autorefresh import st_autorefresh
except ImportError:
    st_autorefresh = None


st.set_page_config(
    page_title="REATOR - Supervisor",
    layout="wide"
)


BASE_DIR = Path(__file__).resolve().parent.parent



ARQUIVO_COMANDOS = BASE_DIR / "output" / "commands.jl"
INTERVALO_ATUALIZACAO_PADRAO_S = 2


LIMITES_SENSORES = {
    "SNV-01": {
        "nome": "Nivel",
        "min": 0,
        "max": 100,
    },
    "STM-01": {
        "nome": "Temperatura",
        "min": 250,
        "max": 400,
    },
    "SRD-01": {
        "nome": "Radiacao",
        "min": 0,
        "max": 50,
    },
    "SVZ-01": {
        "nome": "Vazao",
        "min": 0,
        "max": 200,
    },
}





def carregar_jsonl(caminho):
    registros = []

    if not caminho.exists():
        return pd.DataFrame()

    with open(caminho, "r", encoding="utf-8") as arquivo:
        for numero_linha, linha in enumerate(arquivo, start=1):
            linha = linha.strip()

            if not linha:
                continue

            try:
                item = json.loads(linha)
                item["_linha"] = numero_linha
                registros.append(item)
            except json.JSONDecodeError:
                registros.append({
                    "tipo": "falha",
                    "tag": "JSON",
                    "status": "INVALIDO",
                    "mensagem": linha,
                    "_linha": numero_linha,
                })

    return pd.DataFrame(registros)


def ativar_atualizacao_automatica():
    st.sidebar.subheader("Atualizacao")

    atualizar = st.sidebar.toggle(
        "Atualizar automaticamente",
        value=True,
    )

    intervalo_s = st.sidebar.slider(
        "Intervalo",
        min_value=2,
        max_value=30,
        value=INTERVALO_ATUALIZACAO_PADRAO_S,
        step=1,
        format="%d s",
    )

    if st.sidebar.button("Atualizar agora", use_container_width=True):
        st.rerun()

    if not atualizar:
        st.sidebar.caption("Atualizacao automatica pausada.")
        return

    if st_autorefresh is None:
        st.sidebar.warning(
            "Instale streamlit-autorefresh para atualizar sem recarregar a pagina."
        )
        return

    contador = st_autorefresh(
        interval=intervalo_s * 1000,
        key="atualizacao_automatica",
    )

    st.sidebar.caption(f"Atualizacao automatica ativa.")


def registrar_comando(tag, acao):
    comando = {
        "tipo": "comando",
        "tag": tag,
        "acao": acao,
        "origem": "supervisor",
        "timestamp": datetime.now(timezone.utc).isoformat(),
    }

    ARQUIVO_COMANDOS.parent.mkdir(parents=True, exist_ok=True)

    with open(ARQUIVO_COMANDOS, "a", encoding="utf-8") as arquivo:
        arquivo.write(json.dumps(comando) + "\n")

    st.session_state["ultimo_comando"] = comando


def preparar_dataframe(df):
    if df.empty:
        return df

    if "tipo" not in df.columns:
        df["tipo"] = ""

    if "timestamp" in df.columns:
        df["timestamp"] = pd.to_datetime(df["timestamp"], errors="coerce")

    if "valor" in df.columns:
        df["valor"] = pd.to_numeric(df["valor"], errors="coerce")

    return df


def validar_leituras(df):
    if df.empty or "valor" not in df.columns:
        return df

    df = df.copy()

    def validar_linha(row):
        tag = row.get("tag")
        valor = row.get("valor")

        if pd.isna(valor):
            return "INVALIDA"

        if tag not in LIMITES_SENSORES:
            return row.get("status", "DESCONHECIDO")

        limite = LIMITES_SENSORES[tag]

        if valor < limite["min"] or valor > limite["max"]:
            return "INVALIDA"

        return row.get("status", "OPERACIONAL")

    df["status_validado"] = df.apply(validar_linha, axis=1)

    return df


def ultimos_por_tag(df):
    if df.empty or "tag" not in df.columns:
        return pd.DataFrame()

    if "timestamp" in df.columns:
        return (
            df.sort_values("timestamp")
            .groupby("tag", as_index=False)
            .tail(1)
            .sort_values("tag")
        )

    return df.groupby("tag", as_index=False).tail(1).sort_values("tag")


arquivo_leitura = arquivo_leitura = BASE_DIR / "output" / "readings.jl"

df = carregar_jsonl(arquivo_leitura)
df = preparar_dataframe(df)

st.title("REATOR - Supervisor da Estacao")

st.caption(f"Arquivo monitorado: {arquivo_leitura}")
ativar_atualizacao_automatica()

if df.empty:
    st.warning("Nenhuma leitura encontrada ainda. Execute o programa C++ para gerar o arquivo readings.jl.")
    st.stop()


leituras = df[
    (df["tipo"].fillna("") == "") &
    (df["tag"].isin(LIMITES_SENSORES.keys()))
].copy()

leituras = validar_leituras(leituras)

alarmes = df[df["tipo"].fillna("") == "alarme"].copy()
falhas_json = df[df["tipo"].fillna("") == "falha"].copy()
atuadores = df[df["tipo"].fillna("") == "atuador"].copy()


col1, col2, col3, col4 = st.columns(4)

with col1:
    st.metric("Total de registros", len(df))

with col2:
    st.metric("Leituras", len(leituras))

with col3:
    st.metric("Alarmes", len(alarmes))

with col4:
    invalidas = leituras[leituras["status_validado"] == "INVALIDA"]
    st.metric("Leituras invalidas", len(invalidas))


st.subheader("Leituras atuais")

leituras_atuais = ultimos_por_tag(leituras)

if leituras_atuais.empty:
    st.info("Nenhuma leitura de sensor encontrada.")
else:
    colunas_leituras = [
        "tag",
        "valor",
        "unidade",
        "timestamp",
        
    ]

    colunas_existentes = [
        coluna for coluna in colunas_leituras
        if coluna in leituras_atuais.columns
    ]

    st.dataframe(
        leituras_atuais[colunas_existentes],
        use_container_width=True,
        hide_index=True,
    )


st.subheader("Historico de variaveis")

if leituras.empty:
    st.info("Sem historico de leituras para exibir.")
else:
    tags_disponiveis = sorted(leituras["tag"].dropna().unique())

    tags_padrao = [
        tag for tag in ["SNV-01", "STM-01"]
        if tag in tags_disponiveis
    ]

    if len(tags_padrao) < 2:
        tags_padrao = tags_disponiveis[:2]

    tags_escolhidas = st.multiselect(
        "Variaveis no grafico",
        options=tags_disponiveis,
        default=tags_padrao,
    )

    historico_filtrado = leituras[leituras["tag"].isin(tags_escolhidas)].copy()

    if historico_filtrado.empty:
        st.info("Selecione pelo menos uma variavel para visualizar o grafico.")
    else:
        grafico = historico_filtrado.pivot_table(
            index="timestamp",
            columns="tag",
            values="valor",
            aggfunc="last",
        )

        st.line_chart(grafico)


st.subheader("Estado dos atuadores")

if atuadores.empty:
    st.info(
        "O arquivo JSON atual ainda nao possui registros de atuadores. "
        "Quando o C++ exportar mensagens com tipo='atuador', elas aparecerao aqui."
    )
else:
    atuadores_atuais = ultimos_por_tag(atuadores)

    colunas_atuadores = [
        "tag",
        "timestamp",
        "status",
        "valor",
        "_linha"
    ]

    colunas_existentes = [
    coluna for coluna in colunas_atuadores
    if coluna in atuadores_atuais.columns
    ]


    st.dataframe(
        atuadores_atuais[colunas_existentes],
        use_container_width=True,
        hide_index=True,
    )


st.subheader("Alarmes")

if alarmes.empty:
    st.success("Nenhum alarme registrado.")
else:
    alarmes_ordenados = alarmes.sort_values(
        "timestamp",
        ascending=False,
        na_position="last",
    )

    colunas_alarmes = [
        "tag",
        "status",
        "timestamp",
    ]

    colunas_existentes = [
        coluna for coluna in colunas_alarmes
        if coluna in alarmes_ordenados.columns
    ]

    st.dataframe(
        alarmes_ordenados[colunas_existentes],
        use_container_width=True,
        hide_index=True,
    )


st.subheader("Historico consultavel")

if leituras.empty:
    st.info("Nenhuma leitura disponivel para consulta.")
else:
    tags_para_filtro = sorted(leituras["tag"].dropna().unique())

    tags_filtradas = st.multiselect(
        "Filtrar por sensor",
        options=tags_para_filtro,
        default=tags_para_filtro,
    )

    historico_consulta = leituras[
        leituras["tag"].isin(tags_filtradas)
    ].copy()

    historico_consulta = historico_consulta.sort_values(
        "timestamp",
        ascending=False,
        na_position="last",
    )

    colunas_consulta = [
        "tag",
        "valor",
        "unidade",
        "timestamp",
        "status",
        "status_validado",
    ]

    colunas_existentes_consulta = [
        coluna for coluna in colunas_consulta
        if coluna in historico_consulta.columns
    ]


    st.dataframe(
        historico_consulta[colunas_existentes_consulta],
        use_container_width=True,
        hide_index=True,
    )


st.subheader("Comandos das varetas")

col_cmd1, col_cmd2, col_cmd3 = st.columns(3)

with col_cmd1:
    if st.button("Automatico", use_container_width=True):
        registrar_comando("VAR-01", "AUTOMATICO")
        st.success("Comando enviado: controle automatico das varetas")

with col_cmd2:
    if st.button("Inserir manualmente", use_container_width=True):
        registrar_comando("VAR-01", "INSERIR")
        st.success("Comando enviado: inserir varetas manualmente")

with col_cmd3:
    if st.button("Retirar manualmente", use_container_width=True):
        registrar_comando("VAR-01", "RETIRAR")
        st.success("Comando enviado: retirar varetas manualmente")

ultimo_comando = st.session_state.get("ultimo_comando")

if ultimo_comando:
    st.info(
        "Ultimo comando gerado: "
        f"{ultimo_comando['acao']} em {ultimo_comando['tag']} "
        f"as {ultimo_comando['timestamp']}"
    )


st.subheader("Historico de comandos")

comandos = carregar_jsonl(ARQUIVO_COMANDOS)
comandos = preparar_dataframe(comandos)

if comandos.empty:
    st.info("Nenhum comando registrado ainda.")
else:
    comandos = comandos.sort_values(
        "timestamp",
        ascending=False,
        na_position="last",
    )

    st.dataframe(
        comandos,
        use_container_width=True,
        hide_index=True,
    )


st.subheader("Falhas e leituras invalidas")

problemas = []

if not falhas_json.empty:
    problemas.append(falhas_json)

if not invalidas.empty:
    problemas.append(invalidas)

if problemas:
    df_problemas = pd.concat(problemas, ignore_index=True)
    st.error("Foram encontradas falhas ou leituras invalidas.")

    st.dataframe(
        df_problemas,
        use_container_width=True,
        hide_index=True,
    )
else:
    st.success("Nenhuma falha ou leitura invalida detectada.")
