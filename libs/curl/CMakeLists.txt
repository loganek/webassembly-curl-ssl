include(ExternalProject)

ExternalProject_Add(
    curl
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/
    PATCH_COMMAND patch -p1 < ${CMAKE_CURRENT_SOURCE_DIR}/curl.patch
    CMAKE_ARGS
        -DWAMR_ROOT_DIR=${CMAKE_SOURCE_DIR}/libs/wasm-micro-runtime/src
        -DCMAKE_TOOLCHAIN_FILE=${WASI_SDK_DIR}/share/cmake/wasi-sdk-pthread.cmake
        -DWASI_SDK_PREFIX=${WASI_SDK_DIR}
        -DCURL_ENABLE_EXPORT_TARGET=ON
        -DENABLE_WEBSOCKETS=ON
        -DCURL_USE_OPENSSL=ON
        -DOPENSSL_ROOT_DIR=${DEPS_INSTALL_DIR}
        -DZLIB_INCLUDE_DIR=${DEPS_INSTALL_DIR}/include
        -DZLIB_LIBRARY=${DEPS_INSTALL_DIR}/lib/libz.a
        -DBUILD_SHARED_LIBS=OFF
        -DBUILD_CURL_EXE=ON
        -DCURL_DISABLE_SMB=ON
        -DCURL_DISABLE_SOCKETPAIR=ON
        -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR}
)

set(CURL_DEPENDENCIES
    OpenSSL::SSL
    OpenSSL::Crypto
    libsocket_wasi_ext
    libzlib
)

add_dependencies(curl ${CURL_DEPENDENCIES})

add_library(libcurl STATIC IMPORTED GLOBAL)
set_target_properties(libcurl PROPERTIES
    IMPORTED_LOCATION ${DEPS_INSTALL_DIR}/lib/libcurl.a
    INTERFACE_INCLUDE_DIRECTORIES ${DEPS_INSTALL_DIR}/include
    INTERFACE_LINK_LIBRARIES "${CURL_DEPENDENCIES}"
)
add_dependencies(libcurl curl)
