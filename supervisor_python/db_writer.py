#!/usr/bin/env python3
"""
db_writer.py - Lê novas linhas dos arquivos readings.jl e commands.jl
e as insere apenas no SQLite.
Rode em background: python db_writer.py
"""

import json
import sqlite3
import time
from pathlib import Path
from datetime import datetime

# ---- CONFIGURAÇÕES ----
BASE_DIR = Path(__file__).resolve().parent.parent

# Arquivos de origem
READINGS_JL = BASE_DIR / "output" / "readings.jl"
COMMANDS_JL = BASE_DIR / "output" / "commands.jl"

# Diretório de destino
DATA_DIR = BASE_DIR / "data"
DATA_DIR.mkdir(exist_ok=True)

# SQLite
DB_PATH = DATA_DIR / "scada.db"

# Arquivos de controle de posição (guardam o número da última linha lida)
POS_READINGS = DATA_DIR / "readings_pos.txt"
POS_COMMANDS = DATA_DIR / "commands_pos.txt"

# Intervalo de verificação (segundos)
INTERVALO = 2

# ---- FUNÇÕES AUXILIARES ----

def ler_posicao(arquivo_pos):
    """Retorna a última linha lida, ou 0 se não existir."""
    if arquivo_pos.exists():
        with open(arquivo_pos, "r") as f:
            return int(f.read().strip())
    return 0

def salvar_posicao(arquivo_pos, pos):
    with open(arquivo_pos, "w") as f:
        f.write(str(pos))

def linha_valida(linha):
    """Verifica se a linha é um JSON válido e não está vazia."""
    linha = linha.strip()
    if not linha:
        return False
    try:
        json.loads(linha)
        return True
    except json.JSONDecodeError:
        return False

def processar_arquivo_jl(caminho_jl, arquivo_pos, tabela, colunas):
    """
    Lê novas linhas do arquivo .jl e insere no SQLite.
    Retorna o número de linhas inseridas.
    """
    if not caminho_jl.exists():
        return 0

    pos_inicial = ler_posicao(arquivo_pos)
    linhas_novas = []

    with open(caminho_jl, "r", encoding="utf-8") as f:
        # Pula as linhas já lidas
        for _ in range(pos_inicial):
            next(f, None)
        # Lê as novas
        for linha in f:
            if linha_valida(linha):
                linhas_novas.append(linha.strip())

    if not linhas_novas:
        return 0

    # Converte para lista de dicionários
    dados = [json.loads(linha) for linha in linhas_novas]

    # ---- Salvar no SQLite ----
    conn = sqlite3.connect(DB_PATH)
    cursor = conn.cursor()

    # Cria a tabela se não existir
    colunas_sql = ", ".join([f"{col} TEXT" for col in colunas])
    cursor.execute(f"CREATE TABLE IF NOT EXISTS {tabela} ({colunas_sql})")

    # Insere cada registro
    placeholders = ", ".join(["?"] * len(colunas))
    for item in dados:
        valores = [item.get(col, "") for col in colunas]
        cursor.execute(f"INSERT INTO {tabela} ({', '.join(colunas)}) VALUES ({placeholders})", valores)

    conn.commit()
    conn.close()

    # Atualiza a posição
    nova_pos = pos_inicial + len(linhas_novas)
    salvar_posicao(arquivo_pos, nova_pos)

    return len(linhas_novas)

# ---- DEFINIÇÃO DAS COLUNAS DE CADA TABELA ----
COLUNAS_LEITURAS = [
    "timestamp", "tag", "valor", "unidade", "status", "tipo", "mensagem"
]
COLUNAS_COMANDOS = [
    "timestamp", "tag", "acao", "origem", "tipo"
]

# ---- LOOP PRINCIPAL ----
if __name__ == "__main__":
    print(f"Monitorando arquivos em {BASE_DIR}")
    print(f"SQLite em {DATA_DIR}")
    print(f"Intervalo: {INTERVALO}s\n")

    while True:
        try:
            # Processa leituras
            n_readings = processar_arquivo_jl(
                READINGS_JL, POS_READINGS, "leituras", COLUNAS_LEITURAS
            )
            # Processa comandos
            n_commands = processar_arquivo_jl(
                COMMANDS_JL, POS_COMMANDS, "comandos", COLUNAS_COMANDOS
            )

            if n_readings or n_commands:
                print(f"[{datetime.now().strftime('%H:%M:%S')}] "
                      f"Inseridas: {n_readings} leituras, {n_commands} comandos")

            time.sleep(INTERVALO)

        except KeyboardInterrupt:
            print("\nEncerrando db_writer...")
            break
        except Exception as e:
            print(f"Erro: {e}")
            time.sleep(INTERVALO)
