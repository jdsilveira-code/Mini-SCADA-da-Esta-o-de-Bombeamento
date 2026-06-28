# TESTES PARA O SUPERVISOR PYTHON
import json
import pytest
import tempfile
import pandas as pd
from pathlib import Path
import sys
import os

sys.path.append(os.path.dirname(os.path.dirname(__file__)))
import app
import db_writer

# TESTE: carregar_jsonl com arquivo válido
def test_carregar_jsonl_valido():
    with tempfile.NamedTemporaryFile(mode='w', suffix='.jl', delete=False) as f:
        f.write('{"timestamp":"2024-01-01","tag":"SNV-01","valor":10,"unidade":"%","status":"OPERACIONAL"}\n')
        f.write('{"timestamp":"2024-01-02","tag":"STM-01","valor":300,"unidade":"K","status":"OPERACIONAL"}\n')
        caminho = Path(f.name)

    df = app.carregar_jsonl(caminho)
    assert len(df) == 2
    assert "timestamp" in df.columns
    assert df.iloc[0]["tag"] == "SNV-01"
    os.unlink(caminho)

# TESTE: carregar_jsonl com arquivo vazio
def test_carregar_jsonl_vazio():
    with tempfile.NamedTemporaryFile(mode='w', suffix='.jl', delete=False) as f:
        pass
    caminho = Path(f.name)
    df = app.carregar_jsonl(caminho)
    assert df.empty
    os.unlink(caminho)

# TESTE: carregar_jsonl com linha inválida (JSON malformado)
def test_carregar_jsonl_invalido():
    with tempfile.NamedTemporaryFile(mode='w', suffix='.jl', delete=False) as f:
        f.write('{"tag":"OK"}\n')
        f.write('linha inválida sem JSON\n')
        f.write('{"tag":"DEPOIS"}\n')
        caminho = Path(f.name)

    df = app.carregar_jsonl(caminho)
    assert len(df) == 3
    assert df.iloc[1]["tipo"] == "falha"
    os.unlink(caminho)

# TESTE: validar_leituras com valores dentro e fora dos limites
def test_validar_leituras():
    dados = [
        {"tag": "SNV-03", "valor": 50, "status": "OPERACIONAL"},   #valido
        {"tag": "SNV-03", "valor": 150, "status": "OPERACIONAL"},  #invalido
        {"tag": "STM-03", "valor": 300, "status": "OPERACIONAL"},  #valido
        {"tag": "STM-03", "valor": 500, "status": "OPERACIONAL"},  #invalido
        {"tag": "SRD-03", "valor": 25, "status": "OPERACIONAL"},   #valido
        {"tag": "SRD-03", "valor": 60, "status": "OPERACIONAL"},   #invalido
        {"tag": "SPR-03", "valor": 100, "status": "OPERACIONAL"},  #valido
        {"tag": "SPR-03", "valor": 250, "status": "OPERACIONAL"},  #invalido
    ]
    df = pd.DataFrame(dados)
    df_validado = app.validar_leituras(df)
    assert len(df_validado) == 8
    assert df_validado[df_validado["status_validado"] == "INVALIDA"].shape[0] == 4

# TESTE: ultimos_por_tag com dados ordenados
def test_ultimos_por_tag():
    dados = [
        {"tag": "A", "timestamp": "2024-01-01 10:00", "valor": 1},
        {"tag": "A", "timestamp": "2024-01-01 10:05", "valor": 2},
        {"tag": "B", "timestamp": "2024-01-01 10:02", "valor": 10},
        {"tag": "B", "timestamp": "2024-01-01 10:10", "valor": 20},
        {"tag": "C", "timestamp": "2024-01-01 10:01", "valor": 100},
    ]
    df = pd.DataFrame(dados)
    df["timestamp"] = pd.to_datetime(df["timestamp"])
    ultimos = app.ultimos_por_tag(df)
    assert len(ultimos) == 3
    assert ultimos[ultimos["tag"] == "A"].iloc[0]["valor"] == 2
    assert ultimos[ultimos["tag"] == "B"].iloc[0]["valor"] == 20
    assert ultimos[ultimos["tag"] == "C"].iloc[0]["valor"] == 100


# TESTE: db_writer - linha_valida
def test_linha_valida():
    assert db_writer.linha_valida('{"a":1}') == True
    assert db_writer.linha_valida('{"a":1,}') == False
    assert db_writer.linha_valida('') == False
    assert db_writer.linha_valida('texto') == False

# TESTE: db_writer - ler_posicao e salvar_posicao
def test_posicoes(tmp_path):
    pos_file = tmp_path / "pos.txt"
    assert db_writer.ler_posicao(pos_file) == 0
    db_writer.salvar_posicao(pos_file, 10)
    assert db_writer.ler_posicao(pos_file) == 10

# TESTE: extração de dose_acumulada do DataFrame
def test_extracao_dose_acumulada():
    dados = [
        {"tag": "SRD-03", "valor": 30, "dose_acumulada": 150.5},
        {"tag": "SNV-03", "valor": 50},
        {"tag": "SRD-03", "valor": 25, "dose_acumulada": 160.2},
    ]
    df = pd.DataFrame(dados)

    # Simula a lógica de extração do app.py
    dose_acumulada = None
    if not df.empty and "dose_acumulada" in df.columns:
        rad_df = df[df["tag"] == "SRD-03"].dropna(subset=["dose_acumulada"])
        if not rad_df.empty:
            dose_acumulada = rad_df.iloc[-1]["dose_acumulada"]

    assert dose_acumulada == 160.2