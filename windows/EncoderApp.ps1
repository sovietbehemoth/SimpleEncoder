
$repo = "https://github.com/sovietbehemoth/SimpleEncoder/archive/refs/heads/main.zip"
$user = ((Get-WMIObject -ClassName Win32_ComputerSystem).Username -split "\\")[1]

if ((Test-Path -Path C:\Users\$user\Downloads\Encoder) -eq $false) {
    Set-Location C:\Users\$user\Downloads

    Invoke-WebRequest $repo -OutFile ./Encoder.zip

    Expand-Archive ./Encoder.zip -DestinationPath ./EncoderEXP

    Set-Location ./EncoderEXP/SimpleEncoder-main/windows/


    #Compile.


    $check = (clang --help)

    $clang64_install_endpoint = "https://github.com/llvm/llvm-project/releases/download/llvmorg-13.0.0/LLVM-13.0.0-win32.exe"
    $clang32_install_endpoint = "https://github.com/llvm/llvm-project/releases/download/llvmorg-13.0.0/LLVM-13.0.0-win64.exe"

    if ($check.Length -eq 0) {
        if ([Environment].Is64BitOperatingSystem) {
            Invoke-WebRequest $clang64_install_endpoint -OutFile clang64.exe
            ./clang64.exe
            Remove-Item -Path -Confirm:$false ./clang64.exe
        } else {
            Invoke-WebRequest $clang32_install_endpoint -OutFile clang32.exe
            ./clang32.exe
            Remove-Item -Path -Confirm:$false ./clang32.exe
        } 
    }

    clang ./src/decode_text.c ./src/encode_text.c ./src/generate_key.c ./src/encode.c ./src/parse.c -o ../../Encoder
    
    Set-Location C:\Users\$user\

    Remove-Item -Path C:\Users\$user\Downloads\Encoder.zip
    Remove-Item -Confirm:$false -Path C:\Users\$user\Downloads\EncoderEXP 
}

C:\$user\Downloads\Encoder.exe