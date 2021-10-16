#include "../include/encode.h"

//Convert text to key encoded text.
char *text_encoder(char *key, char *buffer) {
  //Length stored for node collection.
  int length;

  //Unused, required for read_key() function.
  int node_length;

  //Mutable size used for allocation.
  int size = strlen(buffer);

  //Tracks number of chars indexed.
  int j = 0;

  char **vkey = read_key(key, &length, &node_length);
  char *nbuffer = (char*) malloc(sizeof(char) * size);

  D("Encoder[%p]: (DCSTR) Initial allocation (%ld bytes)", nbuffer, sizeof(char) * size);

  for (int i = 0;; i++) {

    //All chars have been encoded.
    if (j >= strlen(buffer)) {
      break;
    }

    //Between supported char set.
    if (buffer[i] > 31 && buffer[i] <= 126) {

      //Resize if too large.
      if (strlen(nbuffer) + strlen(vkey[buffer[i]-31]) > size) {
        size += strlen(vkey[buffer[i]-31]) + 1;
        nbuffer = realloc(nbuffer, sizeof(char)*size);

        D("Encoder[%p]: Re-size of DCSTR (%ld -> %ld bytes)", nbuffer, sizeof(char) * (size - (strlen(vkey[buffer[i]-31]) + 1)), sizeof(char) * size);
      }

      strcat(nbuffer, vkey[buffer[i]-31]);
      j++;
    } else {
      strcat(nbuffer, " ");
      j++;
    }
  }
  
  return nbuffer;
}