cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(MAX31855_RP2040_TEST C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(
                        ${PROJECT_NAME}
                        example.c
                        src/MAX31855_2040.c
                        )

target_link_libraries(
                        ${PROJECT_NAME}
                        pico_stdlib
                        hardware_spi
                        )


add_library(MAX31855 src/MAX31855_2040.c src/MAX31855_2040.h)

target_link_libraries(MAX31855
    pico_stdlib
    hardware_spi
)


pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)


# Create output binaries
pico_add_extra_outputs(${PROJECT_NAME})