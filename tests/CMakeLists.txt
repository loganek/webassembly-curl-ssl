set(CMAKE_EXECUTABLE_SUFFIX ".wasm")

function(create_example example_name)
    add_executable(${example_name} ${example_name}.c)
    target_link_libraries(${example_name} libcurl libsocket_wasi_ext OpenSSL::SSL OpenSSL::Crypto)

    target_link_options(${example_name} PRIVATE
        -z stack-size=16777216
        LINKER:--shared-memory,--max-memory=125829120
    )
endfunction()

create_example(simple)
create_example(multi)

add_test(
    NAME simple
    COMMAND ${DEPS_INSTALL_DIR}/bin/iwasm
        --dir=${DEPS_INSTALL_DIR}
        --addr-pool=0.0.0.0/0
        --allow-resolve=*
        simple.wasm
        ${DEPS_INSTALL_DIR}/ssl/certs/
        https://github.com/
)

add_test(
    NAME multi
    COMMAND ${DEPS_INSTALL_DIR}/bin/iwasm
        --dir=${DEPS_INSTALL_DIR}
        --addr-pool=0.0.0.0/0
        --allow-resolve=*
        multi.wasm
        ${DEPS_INSTALL_DIR}/ssl/certs/
        https://github.com/
        https://amazon.com/
)
