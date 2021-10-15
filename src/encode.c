#include "../include/encode.h"

//Safely scan for user input.
char *scan(char const *prompt, bool useprompt) {
    //Preceding prompt.
    if (useprompt == true) printf("%s", prompt);

    //Default size.
    int szmax = (int) sizeof(char*);

    //Input buffer.
    char *in = (char*) malloc(sizeof(char) * szmax);
    
    //Current character indexed by fgetc().
    int ch;
    
    while (1) {
        ch = fgetc(stdin);
        if (ch == '\n') {
            break;
        } else if (strlen(in)+1 < szmax) {
            int alen = strlen(in);
            in[alen] = ch;
        //Resize.
        } else {
            szmax += 50;
            in = realloc(in, sizeof(char) * (size_t) szmax);
        } 
    }

    return in;
}

//Handle invalid keys.
void _No_return_ segv_handle() {
  printf("(%d) Invalid key or program failure.\n", getpid());
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    signal(SIGSEGV, segv_handle);

    //Set rand seed.
    time_t t;
    srand((unsigned) time(&t));

    if (argc == 1) {
      use_prompt();
    } else {
        char *key = generate_key(128, 10);
        printf("Key: %s\n", key);
        char *text = text_encoder(key, argv[1]);
        puts(text);
        char *decode = text_decoder(key, text);
        puts(decode);
    }
    
}

