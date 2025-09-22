# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 1.5.1)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
# CMakeLists.txt

cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(wifimqtt C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PICO_BOARD pico_w)
if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# Initialize the SDK
pico_sdk_init()

add_compile_options(
    -Wall
    -Wno-format          # gcc trata int32_t como long int
    -Wno-unused-function # funções de exemplo que não são chamadas
    -Wno-maybe-uninitialized
)

if (PICO_CYW43_SUPPORTED) # set by BOARD=pico_w
    # defina SSID e senha por aqui ou por -D no CMake
    set(WIFI_SSID "Desktop_F4211500" CACHE INTERNAL "WiFi SSID")
    set(WIFI_PASSWORD "053103430P250422C" CACHE INTERNAL "WiFi password")

    if (NOT TARGET pico_cyw43_arch)
        message("Skipping build as support is not available")
    else()
        if ("${WIFI_SSID}" STREQUAL "")
            message("Skipping build as WIFI_SSID is not defined")
        elseif ("${WIFI_PASSWORD}" STREQUAL "")
            message("Skipping build as WIFI_PASSWORD is not defined")
        else()
            add_executable(wifimqtt
                wifimqtt.c
                embarca.c
                main.c
            )

            # stdio via USB ON, UART OFF
            pico_enable_stdio_usb(wifimqtt 1)
            pico_enable_stdio_uart(wifimqtt 0)

            target_compile_definitions(wifimqtt PRIVATE
                WIFI_SSID=\"${WIFI_SSID}\"
                WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
                NO_SYS=1
                LWIP_PICO_MALLOC=1  
                # usa seu mbedtls_config.h local (opcional, mas útil)
                PICO_MBEDTLS_CONFIG_FILE=\"mbedtls_config.h\"
            )

            target_include_directories(wifimqtt PRIVATE
                ${CMAKE_CURRENT_LIST_DIR}
                ${CMAKE_CURRENT_LIST_DIR}/.. # para o lwipopts comum, se aplicável
            )

            target_link_libraries(wifimqtt
                pico_stdlib
                pico_lwip_mbedtls
                pico_mbedtls
                pico_lwip_mqtt
                hardware_pwm
                hardware_gpio
                hardware_adc
                pico_cyw43_arch_lwip_threadsafe_background
            )

            # Habilita a saída USB para o printf
            pico_enable_stdio_usb(wifimqtt 1)
            pico_enable_stdio_uart(wifimqtt 0)

            # silencia o warn_unused_result do mbedtls_ssl_flush_output
            target_compile_options(wifimqtt PRIVATE -Wno-unused-result)

            # gera .uf2/.bin/.hex para o alvo
            pico_add_extra_outputs(wifimqtt)
        endif()
    endif()
endif()
