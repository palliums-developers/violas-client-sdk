# ViolasClientSdk
Violas Client SDK for Android, IOS and PC.

SDK support the follwing programming language 
1. Java for Android
2. Swift for IOS
3. C++ for Linux, MacOS and Windows.



## Build

1. download Libra source code and change file libra/client/src/grpc_client.rs

#### original  

```Rust
pub(crate) fn get_account_blob
```

#### modified  

```rust
pub fn get_account_blob
```


2. open ViolasClientSdk/rust-client-proxy/cargo.toml, replace all reference for 'Libra' with correct path.

3. run "cargo build"

   ​

# Compiling Violas Client on Windows 
1. Install Vistual Studio 2019 Community with C++ and clang
2. Install Rust
3. Install ActivePerl 5.28.1
4. Install Go 1.13.4
5. Download https://github.com/protocolbuffers/protobuf/releases/download/v3.11.1/protoc-3.11.1-win64.zip
	5.1 unzip protoc-3.11.1-win64.zip
	5.2 copy "protoc-3.11.1-win64/bin/protoc.exe" to "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.24.28314\bin\Hostx64\x64"
	5.3 copy folder "protoc-3.11.1-win64\include\google" to "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.24.28314\include"
6. Run "cargo build", grpcio with version 0.4.7 can be compiled successfully, but grpcio with version 0.5.0-alpha.5 cannot be compiled successfully.

