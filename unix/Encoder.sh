
if test -f ./Encoder; then 
    ./Encoder
fi else
    if command -v gcc >/dev/null 2>&1; then 
        make
        ./Encoder
    fi else
        if command -v clang >/dev/null 2>&1; then 
            clang ../src/decode_text.c ../src/encode_text.c ../src/generate_key.c ../src/encode.c ../src/parse.c -o ./Encoder
            ./Encoder
        fi else
            sudo apt install gcc
            ./Encoder
        fi
    fi
fi
