#include "../include/encode.h"


char *file_read(char *path) {
    char *cmd = (char*) malloc(sizeof(char) * (strlen(path) + 15) );
    char *recv = (char*) malloc(sizeof(char*));
    sprintf(cmd, "ls %s 2> 1", path);
    FILE * checkfile = popen(cmd, "r");
    fscanf(checkfile, "%s", recv);
    pclose(checkfile);

    if (!strcmp(recv, "")) {
        return NULL;
    }

    free(cmd);
    free(recv);

    int max = 0;

    FILE * f1 = fopen(path, "r");
    for (; !feof(f1); max++) {
        int c = fgetc(f1);
        if ((char)c == '\0' || (char) c == EOF) max--;
    } rewind(f1);

    char *buffer = (char*) malloc(sizeof(char) * (size_t) max);
    for (int i = 0; i < max; i++) {
        int h = fgetc(f1);
        if ((char)h != '\0' || (char) h != EOF) buffer[i] = h; 
    } fclose(f1);
    return buffer;
}

int main(int argc, char **argv) {
    
    time_t t;
    srand((unsigned) time(&t));

    char *key = generate_key(127, 10);

    text_decoder(key, "hey");
}

