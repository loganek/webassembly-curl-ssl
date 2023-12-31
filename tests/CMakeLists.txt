set(CMAKE_EXECUTABLE_SUFFIX ".wasm")

function(create_example example_name)
    add_executable(${example_name} ${example_name}.c)
    target_link_libraries(${example_name} libcurl)

    target_link_options(${example_name} PRIVATE
        -z stack-size=16777216
        LINKER:--shared-memory,--max-memory=125829120
    )
endfunction()

create_example(simple)
create_example(multi)
create_example(encoding)
create_example(websockets)

set(IWASM_AND_PARAMS
    ${DEPS_INSTALL_DIR}/bin/iwasm
    --dir=${DEPS_INSTALL_DIR}
    --addr-pool=0.0.0.0/0
    --allow-resolve=*)

add_test(
    NAME simple
    COMMAND
        ${IWASM_AND_PARAMS}
        simple.wasm
        ${DEPS_INSTALL_DIR}/ssl/certs/
        https://github.com/
)

add_test(
    NAME multi
    COMMAND ${IWASM_AND_PARAMS}
        multi.wasm
        ${DEPS_INSTALL_DIR}/ssl/certs/
        https://github.com/
        https://amazon.com/
)

add_test(
    NAME encoding
    COMMAND ${IWASM_AND_PARAMS}
        encoding.wasm
        ${DEPS_INSTALL_DIR}/ssl/certs/
        https://github.com/
)

add_test(
    NAME websockets
    COMMAND ${IWASM_AND_PARAMS}
        websockets.wasm
        ${DEPS_INSTALL_DIR}/ssl/certs/
        wss://ws.postman-echo.com/raw
        ECHO
)

add_test(
    NAME curl_exe_get
    COMMAND ${IWASM_AND_PARAMS}
    --stack-size=131072
    ${DEPS_INSTALL_DIR}/bin/curl.wasm
    --capath ${DEPS_INSTALL_DIR}/ssl/certs/
    https://github.com
)

add_test(
    NAME curl_exe_post
    COMMAND ${IWASM_AND_PARAMS}
    --stack-size=131072
    ${DEPS_INSTALL_DIR}/bin/curl.wasm
    --capath ${DEPS_INSTALL_DIR}/ssl/certs/
    -X POST -d 'test' https://httpbin.org/post
)
