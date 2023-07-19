cmake_minimum_required(VERSION 3.13)

project(WebAssemblyCURLOpenSSL)

include(ProcessorCount)
ProcessorCount(CPU_COUNT)
if(CPU_COUNT EQUAL 0)
    set(CPU_COUNT 1)
endif()

set(DEPS_INSTALL_DIR ${CMAKE_BINARY_DIR}/libs_install)

add_subdirectory(libs/wasm-micro-runtime)
add_subdirectory(libs/openssl)
add_subdirectory(libs/zlib)
add_subdirectory(libs/curl)

enable_testing()
add_subdirectory(tests)

# This needs to exist in advance
file(MAKE_DIRECTORY ${DEPS_INSTALL_DIR}/include)

install(DIRECTORY
    ${DEPS_INSTALL_DIR}/lib/
    TYPE LIB)
install(DIRECTORY
    ${DEPS_INSTALL_DIR}/include/
    TYPE INCLUDE)
install(DIRECTORY
    ${DEPS_INSTALL_DIR}/bin/
    TYPE BIN)