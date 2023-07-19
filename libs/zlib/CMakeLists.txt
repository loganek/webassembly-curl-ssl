include(ExternalProject)

set(ZLIB_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/zlib-prefix/src/zlib-build)

ExternalProject_Add(
    zlib
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/
    CMAKE_ARGS
        -DCMAKE_TOOLCHAIN_FILE=${WASI_SDK_PREFIX}/share/cmake/wasi-sdk-pthread.cmake
        -DWASI_SDK_PREFIX=${WASI_SDK_PREFIX}
        -DBUILD_SHARED_LIBS=OFF
        -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR}
        -DCMAKE_SKIP_INSTALL_ALL_DEPENDENCY=ON
    BUILD_COMMAND
        ${CMAKE_COMMAND} --build ${ZLIB_BUILD_DIR} --target zlibstatic
    INSTALL_COMMAND
        # zlib doesn't allow disabling shared library build; install target requires
        # building zlib's shared library so we avoid it.
        ${CMAKE_COMMAND} -E copy ${ZLIB_BUILD_DIR}/libzlibstatic.a ${DEPS_INSTALL_DIR}/lib/libz.a &&
        ${CMAKE_COMMAND} -E copy ${ZLIB_BUILD_DIR}/zconf.h ${CMAKE_CURRENT_SOURCE_DIR}/src/zlib.h ${DEPS_INSTALL_DIR}/include
)

add_library(libzlib STATIC IMPORTED GLOBAL)
set_target_properties(libzlib PROPERTIES
    IMPORTED_LOCATION ${DEPS_INSTALL_DIR}/lib/libz.a
    INTERFACE_INCLUDE_DIRECTORIES ${DEPS_INSTALL_DIR}/include)
add_dependencies(libzlib zlib)
