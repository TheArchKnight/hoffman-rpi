cmake_minimum_required(VERSION 3.13...3.27)

# Ensure the SDK submodule is properly set up
if (NOT DEFINED PICO_SDK_PATH)
  set(PICO_SDK_PATH ${CMAKE_CURRENT_LIST_DIR}/lib/pico-sdk)

endif()

# Include the SDK import script
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Define project
project(blink_project C CXX ASM)

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# Define the executable
add_executable(blink blink.c)

# Link standard Pico libraries
target_link_libraries(blink pico_stdlib)

# Enable USB or UART output
pico_enable_stdio_usb(blink 1)
pico_enable_stdio_uart(blink 0)

# Generate binary and UF2 files
pico_add_extra_outputs(blink)

# Enable compile commands for tools like clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
