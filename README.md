# WebAssembly CURL + SSL

The project aims to compile libcurl with SSL support (OpenSSL) to WebAssembly target using WASI interfaces.

The goal of the project is to identify any gaps (performance, features) in WebAssembly/WASI to run HTTPS clients (and other clients) completely on top of WebAssembly runtime. Whereas having a full HTTPS stack implemented in WebAssembly might sound questionable, there are scenarios where the approach makes sense.

The project uses [WebAssembly Micro Runtime (WAMR) WASI socket extension](https://github.com/bytecodealliance/wasm-micro-runtime/blob/main/doc/socket_api.md) until the [wasi-sockets](https://github.com/WebAssembly/wasi-sockets) proposal is not finalized.

## System requirements
The project was only tested on Ubuntu 20.04 and 22.04. It will most likely compile on other Linux distributions. Windows / MacOS probably doesn't work out of the box. Contributions are very welcome.

## Features
- [X] Protocols: DICT FILE FTP FTPS GOPHER GOPHERS HTTP HTTPS IMAP IMAPS MQTT POP3 POP3S RTSP SMTP SMTPS TELNET TFTP
- [X] SSL
- [X] IPv6
- [X] AsynchDNS
- [X] Largefile
- [X] alt-svc
- [X] HSTS
- [X] NTLM
- [X] HTTPS-proxy
- [X] threadsafe
- [X] CURL executable
### Missing features
- [ ] compression (brotli, zlib)
- [ ] c-ares
- [ ] websockets
- [ ] ...

## Custom patches
Dependent projects required patching in order to compile. All the patches can be found in the `libs/*/*.patch`. Some of the changes can be applied to upstream repositories (and we will propose them to maintainers), but some are WAMR-specific and can be removed once the wasi-sockets proposal is implemented.

## Getting started
1. Install dependencies
```bash
sudo apt install -y cmake
```
2. Initialize submodules
```bash
git submodule update --init --recursive --depth 1
```
3. Configure project
This internally downloads WASI SDK and runs `cmake` command to configure the project.
```bash
./configure.sh
```
4. Compile code
```bash
cmake --build build
```

## Access compiled libraries and headers
Run the command:
```bash
cmake --install build
```
The compiled libraries, executables and include headers will be available in the `install/` directory.

## Tests / Examples
Some of the examples using libcurl can be found in [tests](./tests). To run examples using WAMR, execute the command:

```bash
cmake --build build --target test
```

Please note the tests require valid ssl certificates to be located in `build/libs_instal/ssl/certs`. They can be copied from the system path, e.g. on Ubuntu it's (add `-L` flag to resolve symlinks):
```bash
cp -Lr /etc/ssl/certs/* build/libs_install/ssl/certs/
```

## License
This code sample is released under the MIT license. By contributing to the project, you agree to the license and copyright terms therein and release your contribution under these terms.