#include "../include/encode.h"

#include <ctype.h>

//Safely scan for user input.
char *scan(char const *prompt, bool useprompt, bool doubleconf) {
    //Preceding prompt.
    if (useprompt == true) printf("%s", prompt);

    //Default size.
    int szmax = (int) sizeof(char*);

    //Input buffer.
    char *in = (char*) malloc(sizeof(char) * szmax);
    
    D("Main[%p]: (input) Initial allocation (%d bytes)", in, (int) sizeof(char) * szmax);

    //Current character indexed by fgetc().
    int ch;
    bool NLC = false;
    
    while (1) {
        ch = fgetc(stdin);

        if (NLC == true && doubleconf) {
            if (ch > 33) { 
                NLC = false;
            } else break;
        }

        if (ch == '\n') {

            if (!doubleconf) break;

            NLC = true;
        } else {
            if ((strlen(in) + 1) > szmax) {
                szmax += 50;
                in = realloc(in, sizeof(char) * (size_t) szmax);
                D("Main[%p]: Re-size of input (%d -> %d bytes)", in, (int) sizeof(char) * (szmax-50), (int) sizeof(char) * szmax);
            }
            
            /*int alen = strlen(in);
            in[alen] = ch;*/

            char db[2];
            db[0] = ch;
            db[1] = '\0';

            strcat(in, db);
        } 
    }

    if (strlen(in) > 0) {
        in = realloc(in, strlen(in) * sizeof(char));
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
        prompt:;
        use_prompt();
    } else {
        int offset = 1;

        if (!strcmp(argv[1], "-debug")) {
            DEBUG = true;
            goto prompt;
        } else if (!strcmp(argv[1], "-debugv") && argc >= 2) {
            offset++;
        } 

        char *key = generate_key(128, 10);
        printf("Key: %s\n", key);
        char *text = text_encoder(key, "HELLO GARBAGER");
        puts(text);
        char *decode = text_decoder(key, text);
        puts(decode);
    }
    
}
