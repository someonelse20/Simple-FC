cmake_minimum_required(VERSION 3.13)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(PICO_SDK_PATH ../pico-sdk)

include(pico_sdk_import.cmake)

project(simple-fc-firmware)

pico_sdk_init()

include_directories(${CMAKE_SOURCE_DIR}/inc)

add_library(libfirmware STATIC src/lsm6dsm.c)
target_include_directories(libfirmware PUBLIC ${CMAKE_SOURCE_DIR}/inc)
target_link_libraries(libfirmware pico_stdlib hardware_spi)

add_executable(firmware
	src/main.c
)

target_link_libraries(firmware libfirmware pico_stdlib hardware_spi)
