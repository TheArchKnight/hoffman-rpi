cmake_minimum_required(VERSION 3.13...3.27)

# Ensure the SDK submodule is properly set up
if (NOT DEFINED PICO_SDK_PATH)
  set(PICO_SDK_PATH ${CMAKE_CURRENT_LIST_DIR}/lib/pico-sdk)
endif()

# Include the SDK import script
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Define project
project(bt_project C CXX ASM)

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# Define the executable
add_executable(bt_app
  src/main.cpp
  src/bt/bt_sender.cpp
  src/mpu6050/mpu6050.cpp
  src/huffman/huffman.cpp
)

# Include directories
target_include_directories(bt_app PRIVATE src/bt src/mpu6050 src/huffman)

# Link standard Pico libraries
target_link_libraries(bt_app
  pico_stdlib
  hardware_uart  # Required for UART communication
  hardware_i2c
)

# Enable USB or UART output
pico_enable_stdio_usb(bt_app 1)
pico_enable_stdio_uart(bt_app 0)

# Generate binary and UF2 files
pico_add_extra_outputs(bt_app)

# Enable compile commands for tools like clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

