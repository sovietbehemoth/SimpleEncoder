#include "../include/encode.h"

/** Generate a pseudo random string.
 * @param maximum Length of string.
 * @param range Total chars indexed. Safe values set as 128 and 255.
 */ 
char* random_string(int maximum, int range) {

    
    char *str = (char*) malloc(sizeof(char) * maximum+1);


    for (int i = 0; i < maximum; i++) {

        nrand:;
        //Random character.
        int random_i = rand() % range;

        //Unicode characters with a code less than 31 are not printable (or whitespace).
        //'-' is reserved as a separator token in the key.
        if (random_i <= 33 || random_i == '-') goto nrand;
        else str[i] = random_i;

    } 

    return str;
}


static char* key_build(char **key_nodes, int node_size, int range, int length) {

    //Base size, mutable for resizing. Extreme optimization trickery.
    int key_size = (node_size * (range-31)) + ((2 * range));


    char *key = (char*) malloc(sizeof(char) * key_size);
    
    D("KeyBuilder[%p]: Initial allocation (%d bytes)", key, (int) sizeof(char) * key_size); 

    for (int i = 31; i < range; i++) {
        
        //Resize buffer if too large. Plus two is to account for the unicode character itself and the separator.
        if ( (strlen(key) + node_size + 2) > (sizeof(char) * key_size)) {
            //Buffer size increment is set to 100 by default, may be increased to decrease realloc calls.
            key_size += 50;
            key = realloc(key, sizeof(char) * key_size);
            D("KeyBuilder[%p]: Re-sized key buffer (%d -> %d bytes)", key, (int) sizeof(char) * (key_size - 50), (int) sizeof(char) * (key_size));
        }

        //Concatenate key definition to buffer.

        int len = strlen(key);
        key[len] = i;
        strcat(key, key_nodes[i - 31]);
        if (i != range-1) strcat(key, "-");
    } 
    
    D("KeyBuilder[%p]: finished key generation", key);

    //Resize if the size does not match.
    if (strlen(key) < key_size) {
        //Some compilers may require casting to size_t in dynamic allocation call.
        key = realloc(key, (size_t) strlen(key));

        D("KeyBuilder[%p]: re-size of key (%d -> %ld bytes)", key, key_size, strlen(key));
    }


  return key;
}

//Check if node is in key.
bool in_key(char **key_nodes, int length, char *node) {
  for (int i = 0; i < length; i++) {
    if (!strcmp(node, key_nodes[i])) {
      return true;
    }
  } return false;
}

char *generate_key(int range, int node_size) {

    //Number of nodes in the key buffer.
    int count = 0;
    
    //Key buffer object.
    //For node sizes less than 3, an offset is needed. Extreme optimization trickery.
    char **key_nodes = (char**) calloc( (range - 31), node_size < 3 ? (sizeof (char) * node_size) + 5 : sizeof(char) * node_size );
    
    D("KeyGenerator[%p]: (NodeBuffer) Initial allocation (%d x %ld bytes)", key_nodes, range-31, node_size < 3 ? (sizeof (char) * node_size) + 5 : sizeof(char) * node_size);

    for (int i = 31; i < range; i++) {
        char *random_str;
        
        redo:;

        random_str = random_string(node_size, range);
  
        //The chances are ludicrous, but check anyways.
        if (in_key(key_nodes, count, random_str)) {
          free(random_str);
          random_str = NULL;
          goto redo;
        }

        //Copy nodes.
        key_nodes[count] = (char*) malloc(sizeof(char) * strlen(random_str));
        
        D("KeyGenerator[%p]: (node %d) Initial allocation (%ld bytes)", key_nodes[count], count, sizeof(char) * strlen(random_str));

        strcpy(key_nodes[count], random_str);
        count++;
    
    } count++;

    


    char *key = key_build(key_nodes, node_size, range, count);

    return key;
}