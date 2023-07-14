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

add_dependencies(curl OpenSSL wasm-micro-runtime zlib)

# This needs to exist in advance
file(MAKE_DIRECTORY ${DEPS_INSTALL_DIR}/include)

add_library(libsocket_wasi_ext STATIC IMPORTED GLOBAL)
set_property(TARGET libsocket_wasi_ext PROPERTY IMPORTED_LOCATION ${DEPS_INSTALL_DIR}/lib/libsocket_wasi_ext.a)
add_dependencies(libsocket_wasi_ext wasm-micro-runtime)

add_library(libcurl STATIC IMPORTED GLOBAL)
set_property(TARGET libcurl PROPERTY IMPORTED_LOCATION ${DEPS_INSTALL_DIR}/lib/libcurl.a)
set_property(TARGET libcurl PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${DEPS_INSTALL_DIR}/include)

add_dependencies(libcurl curl)
