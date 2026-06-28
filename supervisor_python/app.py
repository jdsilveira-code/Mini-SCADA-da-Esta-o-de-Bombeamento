import json
from datetime import datetime, timezone
from pathlib import Path

import pandas as pd
import streamlit as st

try:
    from streamlit_autorefresh import st_autorefresh
except ImportError:
    st_autorefresh = None

# ---- CONSTANTES GLOBAIS (usadas por funções e testes) ----
BASE_DIR = Path(__file__).resolve().parent.parent
ARQUIVO_COMANDOS = BASE_DIR / "output" / "commands.jl"
INTERVALO_ATUALIZACAO_PADRAO_S = 2

LIMITES_SENSORES = {
    "SNV-03": {"nome": "Nivel", "min": 0, "max": 100},
    "STM-03": {"nome": "Temperatura", "min": 250, "max": 400},
    "SRD-03": {"nome": "Radiacao", "min": 0, "max": 50},
    "SPR-03": {"nome": "Pressao", "min": 0, "max": 200},
}

# ---- FUNÇÕES (testáveis) ----
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
    atualizar = st.sidebar.toggle("Atualizar automaticamente", value=True)
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
    st_autorefresh(
        interval=intervalo_s * 1000,
        key="atualizacao_automatica",
    )
    st.sidebar.caption("Atualizacao automatica ativa.")

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

# ---- PONTO DE ENTRADA (SÓ EXECUTA QUANDO O SCRIPT É RODADO DIRETAMENTE) ----
if __name__ == "__main__":
    st.set_page_config(
        page_title="REATOR - Supervisor",
        layout="wide"
    )

    arquivo_leitura = BASE_DIR / "output" / "readings.jl"
    df = carregar_jsonl(arquivo_leitura)
    df = preparar_dataframe(df)

    st.title("REATOR - Supervisor da Estacao")
    st.caption(f"Arquivo monitorado: {arquivo_leitura}")
    ativar_atualizacao_automatica()

    if df.empty:
        st.warning("Nenhuma leitura encontrada ainda. Execute o programa C++ para gerar o arquivo readings.jl.")
        st.stop()

    # Filtragem de leituras
    leituras = df[
        (df["tipo"].fillna("") == "") &
        (df["tag"].isin(LIMITES_SENSORES.keys()))
    ].copy()
    leituras = validar_leituras(leituras)

    alarmes = df[df["tipo"].fillna("") == "alarme"].copy()
    falhas_json = df[df["tipo"].fillna("") == "falha"].copy()
    atuadores = df[df["tipo"].fillna("") == "atuador"].copy()

    # ---- ALERTA DE EXPLOSÃO (SOMENTE SE HOUVER FALHA CRÍTICA) ----
    if not falhas_json.empty:
        explosao = falhas_json[
            (falhas_json["severidade"] == "CRITICA") &
            (falhas_json["mensagem"].str.contains("REATOR EM FUSÃO", case=False, na=False))
        ]
        if not explosao.empty:
            mensagem_erro = "FALHA CRÍTICA O REATOR EXPLODIU"

            # CSS para o banner fixo no topo
            st.markdown(
                """
                <style>
                .banner-emergencia-scada {
                    position: fixed;
                    top: 0;
                    left: 0;
                    width: 100%;
                    background: linear-gradient(90deg, #7a0000, #ff0000, #7a0000);
                    background-size: 200% 200%;
                    color: white;
                    text-align: center;
                    padding: 18px;
                    font-size: 22px;
                    font-weight: bold;
                    font-family: 'Courier New', Courier, monospace;
                    z-index: 999999;
                    box-shadow: 0px 6px 20px rgba(0,0,0,0.7);
                    animation: pulsarBanner 1.2s ease infinite;
                }

                @keyframes pulsarBanner {
                    0% { background-position: 0% 50%; }
                    50% { background-position: 100% 50%; }
                    100% { background-position: 0% 50%; }
                }

                /* Empurra o app para baixo para o banner não cobrir o título */
                .stApp {
                    margin-top: 55px;
                }
                </style>
                """,
                unsafe_allow_html=True
            )

            # Renderiza o banner
            st.markdown(
                f'<div class="banner-emergencia-scada">⚠️ {mensagem_erro.upper()} ⚠️</div>',
                unsafe_allow_html=True
            )

            # Sirene contínua (loop)
            sirene_url = "https://actions.google.com/sounds/v1/alarms/industrial_alarm_loop.ogg"
            st.markdown(
                f"""
                <audio autoplay loop style="display:none;">
                    <source src="{sirene_url}" type="audio/ogg">
                </audio>
                """,
                unsafe_allow_html=True
            )

    # ---- MÉTRICAS ----
    col1, col2, col3, col4 = st.columns(4)
    with col1:
        st.metric("Total de registros", len(df))
    with col2:
        st.metric("Leituras", len(leituras))
    with col3:
        st.metric("Alarmes", len(alarmes))
    with col4:
        if not leituras.empty and "status_validado" in leituras.columns:
            invalidas = leituras[leituras["status_validado"] == "INVALIDA"]
        else:
            invalidas = pd.DataFrame()
        st.metric("Leituras invalidas", len(invalidas))

    st.subheader("Leituras atuais")
    leituras_atuais = ultimos_por_tag(leituras)
    if leituras_atuais.empty:
        st.info("Nenhuma leitura de sensor encontrada.")
    else:
        colunas_leituras = ["tag", "valor", "unidade", "timestamp"]
        colunas_existentes = [col for col in colunas_leituras if col in leituras_atuais.columns]
        st.dataframe(leituras_atuais[colunas_existentes], use_container_width=True, hide_index=True)

    st.subheader("Historico de variaveis")
    if leituras.empty:
        st.info("Sem historico de leituras para exibir.")
    else:
        tags_disponiveis = sorted(leituras["tag"].dropna().unique())
        tags_padrao = [tag for tag in ["SNV-03", "STM-03"] if tag in tags_disponiveis]
        if len(tags_padrao) < 2:
            tags_padrao = tags_disponiveis[:2]
        tags_escolhidas = st.multiselect("Variaveis no grafico", options=tags_disponiveis, default=tags_padrao)
        historico_filtrado = leituras[leituras["tag"].isin(tags_escolhidas)].copy()
        if historico_filtrado.empty:
            st.info("Selecione pelo menos uma variavel para visualizar o grafico.")
        else:
            grafico = historico_filtrado.pivot_table(index="timestamp", columns="tag", values="valor", aggfunc="last")
            st.line_chart(grafico)

    st.subheader("Estado dos atuadores")
    if atuadores.empty:
        st.info("O arquivo JSON atual ainda nao possui registros de atuadores.")
    else:
        atuadores_atuais = ultimos_por_tag(atuadores)
        colunas_atuadores = ["tag", "timestamp", "valor", "_linha"]
        colunas_existentes = [col for col in colunas_atuadores if col in atuadores_atuais.columns]
        st.dataframe(atuadores_atuais[colunas_existentes], use_container_width=True, hide_index=True)

    st.subheader("Alarmes")
    if alarmes.empty:
        st.success("Nenhum alarme registrado.")
    else:
        alarmes_ordenados = alarmes.sort_values("timestamp", ascending=False, na_position="last")
        colunas_alarmes = ["tag", "status", "timestamp"]
        colunas_existentes = [col for col in colunas_alarmes if col in alarmes_ordenados.columns]
        st.dataframe(alarmes_ordenados[colunas_existentes], use_container_width=True, hide_index=True)

    st.subheader("Historico consultavel")
    if leituras.empty:
        st.info("Nenhuma leitura disponivel para consulta.")
    else:
        tags_para_filtro = sorted(leituras["tag"].dropna().unique())
        tags_filtradas = st.multiselect("Filtrar por sensor", options=tags_para_filtro, default=tags_para_filtro)
        historico_consulta = leituras[leituras["tag"].isin(tags_filtradas)].copy()
        historico_consulta = historico_consulta.sort_values("timestamp", ascending=False, na_position="last")
        colunas_consulta = ["tag", "valor", "unidade", "timestamp", "status"]
        colunas_existentes_consulta = [col for col in colunas_consulta if col in historico_consulta.columns]
        st.dataframe(historico_consulta[colunas_existentes_consulta], use_container_width=True, hide_index=True)

    st.subheader("Comandos das varetas")
    col_cmd1, col_cmd2, col_cmd3 = st.columns(3)
    with col_cmd1:
        if st.button("Automatico", use_container_width=True):
            registrar_comando("VAR-03", "AUTOMATICO")
            st.success("Comando enviado: controle automatico das varetas")
    with col_cmd2:
        if st.button("Inserir manualmente", use_container_width=True):
            registrar_comando("VAR-03", "INSERIR")
            st.success("Comando enviado: inserir varetas manualmente")
    with col_cmd3:
        if st.button("Retirar manualmente", use_container_width=True):
            registrar_comando("VAR-03", "RETIRAR")
            st.success("Comando enviado: retirar varetas manualmente")

    st.subheader("Comandos da bomba")
    col_bomba1, col_bomba2, col_bomba3 = st.columns(3)
    with col_bomba1:
        if st.button("Automático", use_container_width=True):
            registrar_comando("BAG-03", "AUTOMATICO")
            st.success("Comando enviado: controle automatico da bomba")
    with col_bomba2:
        if st.button("Ligar bomba", use_container_width=True):
            registrar_comando("BAG-03", "LIGAR")
            st.success("Comando enviado: ligar bomba manualmente")
    with col_bomba3:
        if st.button("Desligar bomba", use_container_width=True):
            registrar_comando("BAG-03", "DESLIGAR")
            st.success("Comando enviado: desligar bomba manualmente")

    ultimo_comando = st.session_state.get("ultimo_comando")
    if ultimo_comando:
        st.info(f"Ultimo comando gerado: {ultimo_comando['acao']} em {ultimo_comando['tag']} as {ultimo_comando['timestamp']}")

    st.subheader("Historico de comandos")
    comandos = carregar_jsonl(ARQUIVO_COMANDOS)
    comandos = preparar_dataframe(comandos)
    if comandos.empty:
        st.info("Nenhum comando registrado ainda.")
    else:
        comandos = comandos.sort_values("timestamp", ascending=False, na_position="last")
        st.dataframe(comandos, use_container_width=True, hide_index=True)

    st.subheader("Falhas e leituras invalidas")
    problemas = []
    if not falhas_json.empty:
        falhas_clean = falhas_json[["timestamp", "mensagem", "dose_acumulada", "severidade"]].copy()
        falhas_clean["tipo"] = "Falha"
        problemas.append(falhas_clean)
    if 'invalidas' in locals() and not invalidas.empty:
        invalidas_clean = invalidas[["timestamp", "tag", "valor", "unidade", "status_validado"]].copy()
        invalidas_clean["tipo"] = "Inválida"
        problemas.append(invalidas_clean)

    if problemas:
        df_problemas = pd.concat(problemas, ignore_index=True)
        df_problemas = df_problemas.sort_values("timestamp", ascending=False, na_position="last")
        df_problemas.rename(columns={
            "status_validado": "status",
            "dose_acumulada": "dose (mSv)",
            "severidade": "gravidade"
        }, inplace=True)

        st.dataframe(
            df_problemas,
            use_container_width=True,
            hide_index=True,
            column_config={
                "timestamp": "Timestamp",
                "tag": "Tag",
                "valor": "Valor",
                "unidade": "Unidade",
                "mensagem": "Mensagem",
                "dose (mSv)": "Dose (mSv)",
                "gravidade": "Gravidade",
                "status": "Status",
                "tipo": "Tipo"
            }
        )
    else:
        st.success("Nenhuma falha ou leitura invalida detectada.")