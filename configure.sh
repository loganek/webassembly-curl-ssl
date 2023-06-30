#!/bin/bash

WASI_SDK_VER=20.0
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

# Download WASI SDK
WASI_SDK_DIR=${SCRIPT_DIR}/wasi-sdk-${WASI_SDK_VER}
WASI_SDK_ARCHIVE=wasi-sdk-${WASI_SDK_VER}-linux.tar.gz
if [ ! -e wasi-sdk-${WASI_SDK_VER}-linux.tar.gz ]; then
    wget https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-20/${WASI_SDK_ARCHIVE}
fi

if [ ! -d ${WASI_SDK_DIR} ]; then
    tar xzvf ${WASI_SDK_ARCHIVE}
fi

cmake \
    -S. \
    -Bbuild \
    -DCMAKE_TOOLCHAIN_FILE=${WASI_SDK_DIR}/share/cmake/wasi-sdk-pthread.cmake \
    -DWASI_SDK_PREFIX=${WASI_SDK_DIR} \
    -DCMAKE_INSTALL_PREFIX=install
