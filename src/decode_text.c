#include "../include/encode.h"

int indexof(char *str, char *ind, int start) {
  int i, j = 0;
  for (i = start; i < strlen(str); i++) {
    if (str[i] == ind[j]) {
      j++;
      if (j == strlen(ind)) {
        return (i-j) + 1;
      }
    } else {
      j = 0;
    }
  } return -1;
}

char *replace(char *str, char *ind, char *repl, int start, int *matches) {
  size_t dynamic_size = strlen(str);  
  
  char *nstr = (char*) malloc(sizeof(char) * dynamic_size);


  int find = indexof(str, ind, 0);
  for (int i = start; i < strlen(str); i++) {
    if (i == find) {
      if (strlen(repl) + strlen(nstr) > dynamic_size) {
        dynamic_size += strlen(repl);
        nstr = realloc(nstr, sizeof(char) * dynamic_size);
      }
    
      *matches++;
      strcat(nstr, repl);
      find = indexof(str, ind, i+strlen(ind));
    } else {
      if (dynamic_size+1 > strlen(nstr)) {
        dynamic_size += 5;
        nstr = realloc(nstr, sizeof(char) * dynamic_size);
      }

      int newp = strlen(nstr);
      nstr[newp] = str[i];
    }
    
  }

  //shrink memory chunk to fit.
  nstr = realloc(nstr, sizeof(char) * strlen(nstr));

  return nstr;
}


char **read_key(char *key, int *len) {

    int size = 20;
    int length = 0;

    bool istartof_node = false;

    char **decoded_key = (char**) calloc(128, sizeof(char) * size);



    bool is_start = false;

    for (int i = 0; i < strlen(key); i++) {
        

        if (length == 127) {
            break;
        }

        if (i == 0) {
            
            decoded_key[length] = (char*) malloc(sizeof(char) * size);
        }

        if (key[i] == '-') {
            
            if (strlen(key) >= i + 1) {
                if (key[i+1] == '-') {
                    decoded_key[length] = (char*) malloc(sizeof(char) * size);
                    strcpy(decoded_key[length], "CRTLTERM");
                    length++;
                    decoded_key[length] = (char*) malloc(sizeof(char) * size);
                    i++;
                    continue;
                }
            }


            //printf("%c(%d): %s\n", length+33, length+33, decoded_key[length]);
            length++;
            decoded_key[length] = (char*) malloc(sizeof(char) * size);
            is_start = true;
        } else {
            if (is_start) {
                is_start = false;
            } else {
                int len = strlen(decoded_key[length]);
                decoded_key[length][len] = key[i];
            };
        }



    } length++;
    
    *len = length;

    return decoded_key;
}


char *text_decoder(char *key, char *buffer) {
    int length;

    char **vkey = read_key(key, &length);

    int matches = 0;
    int psize = 0;

    for (int i = 0; i < length; i++) {
        char cast[2];
        cast[0] = i+33;
        cast[1] = 0;
        //puts(vkey[i]);
        psize += (strlen(vkey[i])) * matches;
        buffer = replace(buffer, cast, vkey[i], psize, &matches);
        //psize += (strlen(vkey[i])) * matches;
    
    }

    puts(buffer);
    

}