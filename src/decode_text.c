#include "../include/encode.h"


/** Read a key and convert it to a pointer collection.
 * @param key Raw key string retrieved from stdin.
 * @param len Length stored here.
 * @param node_length Length of nodes stored here. Only used in decoding.
 */ 
char **read_key(char *key, int *len, int *node_length) {

    //Default base size.
    int size = 20;

    //Length of collection.
    int length = 0;

    //Collection.
    char **decoded_key = (char**) calloc(128, sizeof(char) * size);

    D("KeyReader[%p]: (ParsedKey) Initial allocation (%d x %d bytes)", decoded_key, 128, (int) sizeof(char) * size);

    //Counts separators to calculate the node length.
    int count_sep = 0;

    //Determine if the character is representative of the literal value of the node.
    bool is_start = false;

    for (int i = 0; i < strlen(key); i++) {
        

        //Break at end of 128 char limit.
        if (length == 127) {
            break;
        }


        //Initial allocation.
        if (i == 0) {
            decoded_key[length] = (char*) malloc(sizeof(char) * size);
            D("KeyReader[%p]: (node %d) Initial allocation (%d bytes)", decoded_key[length], length, (int) sizeof(char) * size);
        }

        //Separator token indexed.
        if (key[i] == '-') {

            //Count length of nodes.
            if (count_sep == 0) {
              *node_length = i-1;
            } 

            count_sep++;

            //Maybe useless?
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

            //Begin new node.
            length++;
            decoded_key[length] = (char*) malloc(sizeof(char) * size);
            D("KeyReader[%p]: (node %d) Initial allocation (%d bytes)", decoded_key[length], length, (int) sizeof(char) * size);
            is_start = true;
        } else {
            if (is_start) {
                is_start = false;
            } else {
                //Append to node.
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
  int node_length;
  
  char **key_nodes = read_key(key, &length, &node_length);

  //Final decoded string.
  char *nstr = (char*) malloc((sizeof(char) * strlen(buffer)) / node_length);

  D("Decoder[%p]: (DSTR) Initial allocation (%d bytes)", nstr, (int) (sizeof(char) * strlen(buffer)) / node_length);

  //Current node. Size is constant because all nodes (should be) the same size.
  char *cstr = (char*) malloc(sizeof(char) * node_length);

  D("Decoder[%p]: (node) Main allocation (%d bytes)", cstr, (int) sizeof(char) * node_length);

  //Iterate through buffer tokens.
  for (int i = 0 ; i < strlen(buffer); i += node_length) {

    //Iterate through 10 selective tokens that are determined to be a value.
    for (int j = i; j < node_length+i; j++) {
      int slen = strlen(cstr);
      cstr[slen] = buffer[j];
    } 

    //Find matching node.
    for (int u = 0; u < length; u++) {
      if (!strcmp(cstr, key_nodes[u])) {
        int len = strlen(nstr);
        nstr[len] = u+31;
      }
    }


    //It is crucial to free the memory storing the previous node, this is because
    //without freeing it, it will instead get written over, corrupting the memory.
    free(cstr);
    cstr = (char*) malloc(sizeof(char) * node_length);

    D("Decoder[%p]: (node) Freedom and re-allocation of %d bytes", cstr, (int) sizeof(char) * node_length);
  }

  return nstr;
}