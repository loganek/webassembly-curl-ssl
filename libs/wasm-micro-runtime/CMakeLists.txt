include(ExternalProject)

ExternalProject_Add(
    wasm-micro-runtime
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/product-mini/platforms/linux
    CMAKE_ARGS
        -DWAMR_BUILD_INTERP=1
        -DWAMR_DISABLE_WRITE_GS_BASE=1
        -DWAMR_BUILD_AOT=0
        -DWAMR_BUILD_JIT=0
        -DWAMR_BUILD_FAST_JIT=0
        -DWAMR_BUILD_LIBC_BUILTIN=1
        -DWAMR_BUILD_FAST_INTERP=1
        -DWAMR_BUILD_LIBC_WASI=1
        -DWAMR_BUILD_LIB_WASI_THREADS=1
        -DWAMR_BUILD_DUMP_CALL_STACK=1
        -DWAMR_BUILD_CUSTOM_NAME_SECTION=1
        -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR}
)

add_library(libsocket_wasi_ext STATIC IMPORTED GLOBAL)
set_target_properties(libsocket_wasi_ext PROPERTIES
    IMPORTED_LOCATION ${DEPS_INSTALL_DIR}/lib/libsocket_wasi_ext.a
)
add_dependencies(libsocket_wasi_ext wasm-micro-runtime)
