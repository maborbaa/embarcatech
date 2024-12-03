cmake_minimum_required(VERSION 3.13)
project(simulation)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_C_FLAGS "-mcpu=cortex-m3 -mthumb -Wall -Wextra -g")
set(CMAKE_EXE_LINKER_FLAGS "-nostartfiles -Tlinker_script.ld")

add_executable(simulation main.c)
