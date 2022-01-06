#include "../include/encode.h"


char *clean_input(char *input) {
    const int length = strlen(input);
    char *copy = (char*) calloc(length, sizeof(char));

    copy[0] = 31;

    for (int i = 0; i < length; i++) {
        if (input[i] == ' ') {
            int bnum = 0;
            int bpoint = -1;

            for (int j = i; j < length; j++) {
                if (input[j] != ' ') {
                    bpoint = j;
                    break;
                } else bnum++;
            }

            if (bnum > 1) {
                i = bpoint;
                continue;
            }
        }

        int dlen = strlen(copy);

        copy[dlen] = input[i];
    }

    copy = realloc(copy, strlen(copy) * sizeof(char));

    return copy;
}

//Stdin interface.
void _No_return_ use_prompt(void) {
    restart:;

    printf("What would you like to do?\n");
    printf("1. Generate key.\t2. Decode key.\n3. Encode text. \t4. Decode text.\n\n");

    char* response = scan("", false, false);

    if (!strcmp(response, "1")) {
        printf("Would you like to customize it?\n");
        char* custom = scan("(y/n): ", true, false);

        int set_range;
        int node_length;

        if (!strcmp(custom, "y")) {
            redo_unicode:;

            char* unicode_set = scan("Unicode Range (most stable is 127)): ", true, false);
            set_range = atoi(unicode_set);

            if (set_range <= 0 || set_range > 255) {
                printf("Unicode range is 0-255\n");
                goto redo_unicode;
            }

            redo_node_length:;

            char* node_length_i = scan("Key Node Length(hint: the longer this is, the longer (albeit more secure), the key): ", true, false);
            node_length = atoi(node_length_i);

            if (node_length < 8) {
                printf("The current minimum supported length is 8.\n");
                goto redo_node_length;
            }
            
        } else {
            //Default.
            set_range = 127;
            node_length = 10;
        }

        char* key = generate_key(set_range, node_length);

        printf("Key\n~~~~~~~~~~\n");
        puts(key);
        printf("~~~~~~~~~~\n");
    }
    else if (!strcmp(response, "2")) {
        char* key = clean_input( scan("Paste key here: ", true, true) );

        //Key 'iterator'.
        int length;
        int node_length;

        char** spl = read_key(key, &length, &node_length);

        printf("Node Length: %d\n", node_length);
        for (int i = 0; i < length; i++) {
            printf("%c(%d): %s\n", i + 31, i + 31, spl[i]);
        }

        puts(key);
    }
    else if (!strcmp(response, "3")) {
        char* key = clean_input( scan("Paste key here: ", true, true) );
        char* text = scan("Enter text here: ", true, true);

        char* encoded = text_encoder(key, text);

        puts("Text\n~~~~~~~~~~");
        puts(encoded);
        puts("~~~~~~~~~~");
    }
    else if (!strcmp(response, "4")) {

        char *raw = scan("Enter key here: ", true, true);
        char *textf = scan("Enter text here: ", true, true);
        char *keyf = clean_input(raw);


        char *decoded = text_decoder(keyf, textf);

        puts("Text\n~~~~~~~~~~");
        puts(decoded);
        puts("~~~~~~~~~~");

        
        free(raw);
        free(keyf);
        free(textf);
    } else {
        printf("Invalid option.\n");
        exit(0);
    }

    exit(EXIT_SUCCESS);
}
