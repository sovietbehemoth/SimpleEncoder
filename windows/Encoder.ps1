

$check = (clang -v)

$clang64_install_endpoint = "https://github.com/llvm/llvm-project/releases/download/llvmorg-13.0.0/LLVM-13.0.0-win32.exe"
$clang32_install_endpoint = "https://github.com/llvm/llvm-project/releases/download/llvmorg-13.0.0/LLVM-13.0.0-win64.exe"

if ($check.Length -eq 0) {
    if ([Environment].Is64BitOperatingSystem) {
        Invoke-WebRequest $clang64_install_endpoint -OutFile clang64.exe
        ./clang64.exe
    } else {
        Invoke-WebRequest $clang32_install_endpoint -OutFile clang32.exe
        ./clang32.exe
    }
}

clang ../src/decode_text.c ../src/encode_text.c ../src/generate_key.c ../src/encode.c ../src/parse.c -o ./Encoder
Clear-Host 
./Encoder