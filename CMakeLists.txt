cmake_minimum_required(VERSION 3.13)

# Nome do projeto
project(simulated_pico)

# Compilador específico para ARM Cortex-M0+
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

# Flags de compilação
set(CMAKE_C_FLAGS "-mcpu=cortex-m0plus -mthumb -Wall -Wextra -g")
set(CMAKE_EXE_LINKER_FLAGS "-nostartfiles -Wl,-Tlinker_script.ld")

# Arquivo fonte
add_executable(simulated_pico main.c)
