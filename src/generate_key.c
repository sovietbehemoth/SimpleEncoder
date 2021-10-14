#include "../include/encode.h"

/** Generate a pseudo random string.
 * @param maximum Length of string.
 * @param range Total chars indexed. Safe values set as 128 and 255.
 */ 
char* random_string(int maximum, int range) {

    
    char *str = (char*) malloc(sizeof(char) * maximum);


    for (int i = 0; i < maximum; i++) {

        nrand:;
        //Random character.
        int random_i = rand() % range;

        //Unicode characters with a code less than 32 are not printable (or whitespace).
        //'-' is reserved as a separator token in the key.
        if (random_i <= 32 || random_i == '-') goto nrand;
        else str[i] = random_i;

    } 

    return str;
}


static char* key_build(char **key_nodes, int node_size, int range) {

    //Base size, mutable for resizing.
    int key_size = 100;


    char *key = (char*) malloc(sizeof(char) * key_size);

    for (int i = 33; i < range; i++) {
        
        //Resize buffer if too large. Plus two is to account for the unicode character itself and the separator.
        if ( (strlen(key) + node_size + 2) > (sizeof(char) * key_size)) {
            //Buffer size increment is set to 100 by default, may be increased to decrease realloc calls.
            key_size += 100;
            key = realloc(key, sizeof(char) * key_size);
        }

        //Concatenate key definition to buffer.

        int len = strlen(key);
        key[len] = i;
        strcat(key, key_nodes[i - 33]);
        if (i != range-1) strcat(key, "-");
    }


    //Resize if the size does not match.
    if (strlen(key) < key_size) {
        //Some compilers may require casting to size_t in dynamic allocation call.
        key = realloc(key, (size_t) strlen(key));
    }


    return key;
}


char *generate_key(int range, int node_size) {

    //Number of nodes in the key buffer.
    int count = 0;

    //Key buffer object.
    char **key_nodes = (char**) calloc( range - 32, sizeof (char) * node_size );
    
    for (int i = 33; i < range; i++) {
        //Copy nodes.
        key_nodes[count] = (char*) malloc(sizeof(char) * node_size);
        strcpy(key_nodes[count], random_string(node_size, range));
        count++;
        //memcpy( key_nodes[count], random_string(node_size, range), sizeof(char) * node_size );
    
    } count++;

    


    char *key = key_build(key_nodes, count, range);

    return key;
}