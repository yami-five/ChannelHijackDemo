# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CHANNELHIJACK_PLATFORM "PICO" CACHE STRING "Target platform for ChannelHijackDemo. Supported values: PICO, WINDOWS")
set_property(CACHE CHANNELHIJACK_PLATFORM PROPERTY STRINGS PICO WINDOWS)
string(TOUPPER "${CHANNELHIJACK_PLATFORM}" CHANNELHIJACK_PLATFORM_NORMALIZED)
if(NOT CHANNELHIJACK_PLATFORM_NORMALIZED STREQUAL "PICO" AND NOT CHANNELHIJACK_PLATFORM_NORMALIZED STREQUAL "WINDOWS")
    message(FATAL_ERROR "Unsupported CHANNELHIJACK_PLATFORM='${CHANNELHIJACK_PLATFORM}'. Supported values: PICO, WINDOWS")
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CHANNELHIJACK_PLATFORM_NORMALIZED STREQUAL "PICO")
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
    set(picotoolVersion 2.2.0)
    set(PICO_EXTRAS_FETCH_FROM_GIT ON)
    set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
    if(EXISTS ${picoVscode})
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
else()
    project(ChannelHijackDemo C CXX)
endif()

include(FetchContent)

if(CHANNELHIJACK_PLATFORM_NORMALIZED STREQUAL "WINDOWS")
    # Provide SDL3 package for Euzebia3D Windows build path.
    set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
    set(SDL_TESTS OFF CACHE BOOL "" FORCE)
    set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(SDL_SHARED ON CACHE BOOL "" FORCE)
    set(SDL_STATIC OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG        main
    )
    FetchContent_MakeAvailable(SDL3)
    FetchContent_GetProperties(SDL3)
    if(EXISTS "${sdl3_BINARY_DIR}/SDL3Config.cmake")
        set(SDL3_DIR "${sdl3_BINARY_DIR}" CACHE PATH "Path to SDL3Config.cmake" FORCE)
        list(PREPEND CMAKE_PREFIX_PATH "${sdl3_BINARY_DIR}")
    endif()
endif()

# Euzebia3D configuration passthrough (v0.6+).
set(EUZEBIA3D_PLATFORM "${CHANNELHIJACK_PLATFORM_NORMALIZED}" CACHE STRING "Target platform for Euzebia3D (PICO/WINDOWS)" FORCE)
set_property(CACHE EUZEBIA3D_PLATFORM PROPERTY STRINGS PICO WINDOWS)
set(EUZEBIA3D_USE_PSRAM ON CACHE BOOL "Enable optional PSRAM-backed const asset copies")
set(EUZEBIA3D_TEXTURE_CACHE_ENABLED ON CACHE BOOL "Enable renderer texture cache")
set(EUZEBIA3D_TEXTURE_CACHE_FROM_FLASH OFF CACHE BOOL "Allow texture cache for flash/XIP textures")
set(EUZEBIA3D_TEXTURE_CACHE_FROM_PSRAM ON CACHE BOOL "Allow texture cache for PSRAM/XIP textures")
set(EUZEBIA3D_BUILD_APP OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    Euzebia3D
    GIT_REPOSITORY https://github.com/yami-five/Euzebia3D.git
    GIT_TAG        v0.7.3
)
FetchContent_MakeAvailable(Euzebia3D)

# Use project-local storage assets (storage/gfx.c, etc.) instead of the defaults from Euzebia3D dependency.
if(TARGET Euzebia3D_Storage)
    set(CHANNELHIJACK_STORAGE_DIR "${CMAKE_CURRENT_LIST_DIR}/storage")
    set_property(TARGET Euzebia3D_Storage PROPERTY SOURCES "${CHANNELHIJACK_STORAGE_DIR}/storage.c")
    target_include_directories(Euzebia3D_Storage PUBLIC "${CHANNELHIJACK_STORAGE_DIR}")
endif()

add_executable(ChannelHijackDemo ChannelHijackDemo.c)
target_include_directories(ChannelHijackDemo PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)
target_link_libraries(ChannelHijackDemo PRIVATE Euzebia3D_libs)

if(CHANNELHIJACK_PLATFORM_NORMALIZED STREQUAL "PICO")
    target_compile_definitions(ChannelHijackDemo PRIVATE PLATFORM_PICO=1)

    pico_set_program_name(ChannelHijackDemo "ChannelHijackDemo")
    pico_set_program_version(ChannelHijackDemo "0.1")

    # Generate PIO header
    pico_generate_pio_header(ChannelHijackDemo ${CMAKE_CURRENT_LIST_DIR}/blink.pio)

    # Modify the below lines to enable/disable output over UART/USB
    pico_enable_stdio_uart(ChannelHijackDemo 1)
    pico_enable_stdio_usb(ChannelHijackDemo 0)

    # Add any user requested libraries
    target_link_libraries(ChannelHijackDemo PRIVATE
            pico_stdlib
            hardware_spi
            hardware_i2c
            hardware_dma
            hardware_pio
            hardware_interp
            Euzebia3D_libs
    )

    pico_add_extra_outputs(ChannelHijackDemo)
else()
    target_compile_definitions(ChannelHijackDemo PRIVATE PLATFORM_WINDOWS=1)
endif()
