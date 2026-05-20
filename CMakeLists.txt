# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(PICO_EXTRAS_FETCH_FROM_GIT ON)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)

project(ChannelHijackDemo C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(ChannelHijackDemo ChannelHijackDemo.c )

pico_set_program_name(ChannelHijackDemo "ChannelHijackDemo")
pico_set_program_version(ChannelHijackDemo "0.1")

# Generate PIO header
pico_generate_pio_header(ChannelHijackDemo ${CMAKE_CURRENT_LIST_DIR}/blink.pio)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(ChannelHijackDemo 1)
pico_enable_stdio_usb(ChannelHijackDemo 0)

# Add the standard library to the build
target_link_libraries(ChannelHijackDemo
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(ChannelHijackDemo PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

include(FetchContent)

set(EUZEBIA3D_BUILD_APP OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    Euzebia3D
    GIT_REPOSITORY https://github.com/yami-five/Euzebia3D.git
    GIT_TAG        v0.1.1
)
FetchContent_MakeAvailable(Euzebia3D)

# Add any user requested libraries
target_link_libraries(ChannelHijackDemo 
        hardware_spi
        hardware_i2c
        hardware_dma
        hardware_pio
        hardware_interp
        Euzebia3D_libs
        )

pico_add_extra_outputs(ChannelHijackDemo)

