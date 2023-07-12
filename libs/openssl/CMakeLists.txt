include(ExternalProject)

set(WASI_SDK_DIR ${CMAKE_SOURCE_DIR}/wasi-sdk-20.0)

set(OPENSSL_INSTALL_DIR ${DEPS_INSTALL_DIR})
set(OPENSSL_INCLUDE_DIR ${OPENSSL_INSTALL_DIR}/include)

ExternalProject_Add(
    OpenSSL
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src
    BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src
    CONFIGURE_COMMAND
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Configure
            --banner="WebAssembly WASI OpenSSL"
            --prefix=${DEPS_INSTALL_DIR}
            --openssldir=${DEPS_INSTALL_DIR}/ssl
            --libdir=lib
            no-asm
            no-docs
            no-async
            no-egd
            no-ktls
            no-module
            no-posix-io
            no-secure-memory
            no-shared
            no-sock
            no-thread-pool
            no-ui-console
            no-tests
            no-apps
            no-weak-ssl-ciphers
            no-afalgeng
            CROSSCOMPILE=""
            AR=${WASI_SDK_DIR}/bin/llvm-ar
            RANLIB=${WASI_SDK_DIR}/bin/llvm-ranlib
            "CC=${WASI_SDK_DIR}/bin/clang --target=wasm32-wasi-threads"
            "CPPFLAGS=-Dgetpid=getpagesize"
    BUILD_COMMAND
        make -j ${CPU_COUNT}
            CFLAGS=-Ofast
            LDFLAGS=-s
    TEST_COMMAND ""
    INSTALL_COMMAND make install
)

# This needs to exist in advance
file(MAKE_DIRECTORY ${OPENSSL_INCLUDE_DIR})

add_library(OpenSSL::SSL STATIC IMPORTED GLOBAL)
set_property(TARGET OpenSSL::SSL PROPERTY IMPORTED_LOCATION ${OPENSSL_INSTALL_DIR}/lib/libssl.a)
add_dependencies(OpenSSL::SSL OpenSSL)

add_library(OpenSSL::Crypto STATIC IMPORTED GLOBAL)
set_property(TARGET OpenSSL::Crypto PROPERTY IMPORTED_LOCATION ${OPENSSL_INSTALL_DIR}/lib/libcrypto.a)
add_dependencies(OpenSSL::Crypto OpenSSL)