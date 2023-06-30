cmake_minimum_required(VERSION 3.13)

project(WebAssemblyCURLOpenSSL)

set(WASI_SDK_DIR "${CMAKE_SOURCE_DIR}/wasi-sdk-20.0" CACHE STRING "WASI SDK directory")

include(ProcessorCount)
ProcessorCount(CPU_COUNT)
if(CPU_COUNT EQUAL 0)
    set(CPU_COUNT 1)
endif()

set(DEPS_INSTALL_DIR ${CMAKE_BINARY_DIR}/libs_install)

add_subdirectory(libs/wasm-micro-runtime)
add_subdirectory(libs/openssl)
add_subdirectory(libs/curl)

enable_testing()
add_subdirectory(tests)