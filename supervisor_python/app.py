import json
from pathlib import Path

import pandas as pd
import streamlit as st


st.set_page_config(
    page_title="Mini-SCADA - Supervisor",
    layout="wide"
)


BASE_DIR = Path(__file__).resolve().parent.parent

POSSIVEIS_ARQUIVOS_LEITURA = [
    BASE_DIR / "output" / "readings.jl",
    BASE_DIR / "dispositivo_cpp" / "output" / "readings.jl",
]

ARQUIVO_COMANDOS = BASE_DIR / "output" / "commands.jl"


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


def encontrar_arquivo_leitura():
    for caminho in POSSIVEIS_ARQUIVOS_LEITURA:
        if caminho.exists():
            return caminho

    return POSSIVEIS_ARQUIVOS_LEITURA[0]


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


arquivo_leitura = encontrar_arquivo_leitura()

df = carregar_jsonl(arquivo_leitura)
df = preparar_dataframe(df)

st.title("Mini-SCADA - Supervisor da Estacao")

st.caption(f"Arquivo monitorado: {arquivo_leitura}")

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
        "status",
        "status_validado",
    ]

    colunas_existentes = [
        coluna for coluna in colunas_leituras
        if coluna in leituras_atuais.columns
    ]

    st.dataframe(
        leituras_atuais[colunas_existentes],
        width='stretch',
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

    st.dataframe(
        atuadores_atuais,
        width='stretch',
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
        width='stretch',
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

    st.dataframe(
        historico_consulta,
        width='stretch',
        hide_index=True,
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
        width='stretch',
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
        width='stretch',
        hide_index=True,
    )
else:
    st.success("Nenhuma falha ou leitura invalida detectada.")