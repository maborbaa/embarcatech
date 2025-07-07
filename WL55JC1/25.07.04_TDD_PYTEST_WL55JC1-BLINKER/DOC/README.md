# Projeto: Blinker TDD para Firmware STM32

Este projeto demonstra a aplicação de Test-Driven Development (TDD) no desenvolvimento de firmware para microcontroladores STM32 (NUCLEO-WL55JC1). O objetivo é criar um módulo de controle de LED (`Blinker`) cuja lógica de negócio é inteiramente verificada por testes de unidade em Python, no ambiente de desenvolvimento, antes de ser integrada ao hardware final.

Este repositório serve como um exemplo prático de como estruturar um projeto embarcado para máxima testabilidade e portabilidade, separando a lógica de negócio das dependências de hardware.

## Índice
1. [Objetivos do Projeto](#1-objetivos-do-projeto)
2. [Requisitos](#2-requisitos)
3. [Arquitetura de Software](#3-arquitetura-de-software)
4. [Estrutura de Diretórios](#4-estrutura-de-diretórios)
5. [Estratégia de Teste](#5-estratégia-de-teste)
6. [Ferramentas e Fluxo de Trabalho](#6-ferramentas-e-fluxo-de-trabalho)
7. [Como Iniciar](#7-como-iniciar)

---

### 1. Objetivos do Projeto
* Desenvolver um módulo de firmware (`Blinker`) em C, reutilizável e portável.
* Aplicar a metodologia TDD para guiar o desenvolvimento da lógica do módulo.
* Garantir a qualidade e a robustez do código através de uma suíte de testes automatizados.
* Estabelecer um fluxo de trabalho que permita o desenvolvimento rápido no PC e a integração segura no hardware alvo.

### 2. Requisitos

#### 2.1. Requisitos Funcionais (RF)
* **RF.1:** O módulo deve ser inicializável, e seu estado inicial deve corresponder ao LED DESLIGADO.
* **RF.2:** O módulo deve prover uma função de atualização (`blinker_tick()`) para avançar sua máquina de estados.
* **RF.3:** Uma chamada a `blinker_tick()` no estado DESLIGADO deve transicionar o estado para LIGADO e invocar a interface de hardware para acender o LED.
* **RF.4:** Uma chamada a `blinker_tick()` no estado LIGADO deve transicionar o estado para DESLIGADO e invocar a interface de hardware para apagar o LED.

#### 2.2. Requisitos Não-Funcionais (RNF)
* **RNF.1 (Portabilidade):** O código da lógica (`src/blinker.c`) não deve ter dependências diretas da HAL (Hardware Abstraction Layer) da STM (NUCLEO-WL55JC1).
* **RNF.2 (Performance do Teste):** A suíte de testes de unidade completa deve executar em menos de 2 segundos.
* **RNF.3 (Qualidade):** A lógica de negócio no módulo `Blinker` deve atingir 100% de cobertura de código (code coverage).

### 3. Arquitetura de Software

O projeto adota uma arquitetura em camadas para garantir a separação de responsabilidades e a testabilidade, diagrama abaxo.

```
+-------------------------------------------------------------+ 
| Camada de Aplicação (main.c no CubeIDE)                     | 
+-------------------------------------------------------------+ 
^ 
| (inclui blinker.h) 
v 
+-------------------------------------------------------------+ 
| Camada de Lógica de Negócio (blinker.c) - PORTÁTIL          |  <-- Testada pelo Python 
+-------------------------------------------------------------+ 
^ 
| (inclui hal_gpio.h) 
v 
+-------------------------------------------------------------+ 
| Camada de Abstração de Hardware (Nossa Interface)           | 
+-------------------------------------------------------------+ 
^ 
| (implementa a interface usando a HAL da ST) 
v 
+-------------------------------------------------------------+ 
| Camada de Driver/HAL (Bibliotecas da ST - CubeIDE)          | 
+-------------------------------------------------------------+ 
^ 
| (controla) 
v 
+-------------------------------------------------------------+ 
| Hardware Físico (Placa Nucleo-WL55JC1)                      | 
+-------------------------------------------------------------+
```

### 4. Estrutura de Diretórios

```
tdd-nucleo-blinker/
├── cubeide_project/      # Projeto gerado pelo STM32CubeIDE, implementação real da HAL.
├── src/                  # Lógica de negócio pura e portável, sem dependências de hardware.
│   ├── blinker.c
│   └── blinker.h
├── drivers/              # Definição das nossas interfaces de abstração de hardware.
│   └── hal_gpio.h
└── tests/                # Testes em Python, ambiente virtual e relatórios.
├── test_blinker.py
└── .venv/
```

### 5. Estratégia de Teste

A verificação do módulo é realizada em dois níveis distintos:

1.  **Testes de Unidade (Host - PC):** Foco principal do TDD.
    * **Onde:** Executados no ambiente de desenvolvimento (VSCODE).
    * **O quê:** Testam a `Camada de Lógica de Negócio` (`blinker.c`) em completo isolamento.
    * **Como:** O código C é compilado como uma biblioteca compartilhada (`.so`/`.dll`). O `Pytest` executa os testes em Python, que chamam as funções C. As chamadas à `Camada de Abstração de Hardware` são interceptadas por **Mocks** em Python.

2.  **Teste de Integração (Alvo - Placa):**
    * **Onde:** Executado no hardware real (NUCLEO-WL55JC1).
    * **O quê:** Valida a integração entre a `Lógica de Negócio` (já testada) e a implementação real da `Camada de Abstração` que usa a HAL da ST.
    * **Como:** O projeto completo é compilado no CubeIDE e gravado na placa. O teste consiste em observar o comportamento físico (o LED pisca).

### 6. Ferramentas e Fluxo de Trabalho

* **IDE de Firmware:** STM32CubeIDE (para configuração do hardware e build final).
* **IDE de Teste/Lógica:** Visual Studio Code.
* **Linguagem de Teste:** Python 3.11+ (gerenciado com `pyenv` e `venv`).
* **Framework de Teste:** Pytest.
* **Compilador C (no Host):** GCC.
* **Build System (opcional):** Make.

### 7. Como Iniciar

1.  **Hardware:** Configure o projeto no CubeIDE para a placa NUCLEO-WL55JC1, habilitando o pino do LED como saída.
2.  **Ambiente de Teste:**
    * Navegue até a pasta `tdd-nucleo-blinker`.
    * Configure a versão local do Python: `pyenv local 3.11.9`.
    * Crie e ative o ambiente virtual: `python -m venv .venv` e `source .venv/bin/activate`.
    * Instale as dependências: `pip install pytest`.
3.  **Executando os Testes:**
    * Com o ambiente virtual ativo, execute `pytest -v` dentro da pasta raiz do projeto.