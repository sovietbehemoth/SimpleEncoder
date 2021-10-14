compile:
	@gcc ./src/encode.c ./src/generate_key.c ./src/encode_text.c ./src/decode_text.c -o Encode && ./Encode