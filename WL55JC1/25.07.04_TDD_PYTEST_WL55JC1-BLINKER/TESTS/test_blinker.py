# TESTS/test_blinker.py
from ctypes import *
import pytest
from unittest.mock import MagicMock

#Vamos usar o Fixture do pytest para carregar a biblioteca C
@pytest.fixture
def blinker_lib():
    # Carrega a biblioteca compilada
    lib = CDLL("./libblinker.so")

    # Definir os tipos de retorno das funções C
    lib.blinker_get_state.restype = c_int

    LedFuncType = CFUNCTYPE(None, c_int) # Define o tipo de função para o LED

    # CFUNCTYPE(tipo_retorno, tipo_arg1, tipo_arg2, ...)
    lib.blinker_register_led_func.argtypes = [LedFuncType]

    # Entrega a biblioteca para os testes
    yield lib

def test_blinker_estado_inicial_desligado(blinker_lib):
    "RF.1: Estado inicial corresponder ao desligado"
    
    # --- Preparação do Mock ---
    mock_led_set = MagicMock()

    LedFuncType = CFUNCTYPE(None, c_int)
    
    c_mock_func = LedFuncType(mock_led_set)
    
    blinker_lib.blinker_register_led_func(c_mock_func)


    # Ação: Chama a função de inicialização do nosso módulo C
    blinker_lib.blinker_init()

    #Verificação: Pega o estado interno e verifica se é o 0 (BLINKER_STATE_OFF)
    estado = blinker_lib.blinker_get_state()
    assert estado == 0, "O estado inicial deve ser desligado"

def test_tick_desligado_para_ligado(blinker_lib):
    "RF.3: Transição de OFF para ON"

    # a.Cria um Mock para nossa função de hardware
    mock_led_set = MagicMock()
    LedFuncType = CFUNCTYPE(None, c_int)
    c_mock_func = LedFuncType(mock_led_set)
    blinker_lib.blinker_register_led_func(c_mock_func)

    # Garantir que o estados inicial seja OFF
    blinker_lib.blinker_init()
    mock_led_set.reset_mock() # limpa o mock apos o init

    # Ação
    blinker_lib.blinker_tick()

    # Verificação
    # pega o estado atual do modulo C
    estado = blinker_lib.blinker_get_state()
    assert estado == 1 # BLINKER_STATE_ON

    # 2- se a função de hardware foi chamado para ligar o LED
    mock_led_set.assert_called_once_with(1)

def test_tick_de_ligado_para_desligado(blinker_lib):
    """RF-004: Testa a transição de ON para OFF e a chamada do hardware."""
    # --- Preparação ---
    mock_led_set = MagicMock()
    LedFuncType = CFUNCTYPE(None, c_int)
    c_mock_func = LedFuncType(mock_led_set)
    blinker_lib.blinker_register_led_func(c_mock_func)
    
    # Força o estado inicial para ON para este teste
    blinker_lib.blinker_init()
    blinker_lib.blinker_tick() # Primeiro tick: OFF -> ON
    mock_led_set.reset_mock() # Limpa o mock

    # --- Ação ---
    blinker_lib.blinker_tick() # Segundo tick: ON -> OFF

    # --- Verificação ---
    estado = blinker_lib.blinker_get_state()
    assert estado == 0 # Verifica se o estado voltou para OFF

    # Verifica se a função de hardware foi chamada para DESLIGAR o LED
    mock_led_set.assert_called_once_with(0)