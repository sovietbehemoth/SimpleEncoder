

#if defined(WIN32) || defined(_WIN32)
    #define _CRT_SECURE_NO_WARNINGS    

    #include <Windows.h>
    #define iswhitespace(ch) isblank(ch)
#else 
    #include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <time.h>   //Time function for srand seed.
#include <assert.h>
#include <math.h> //RNG functions.

#include <signal.h> //Segv handling.

#pragma once

#define bool _Bool
#define true 1
#define false 0

#define BASIC_UNICODE 128
#define ALL_UNICODE 255

#define _No_return_ __attribute__((noreturn))

static bool DEBUG = false;

#define D(fmt, ...) DEBUG == true ? printf(fmt, __VA_ARGS__), putchar('\n'): 0

void _No_return_ use_prompt(void);
char *scan(char const *prompt, bool useprompt, bool doubleconf);

char *generate_key(int range, int node_size);
char *random_string(int maximum, int range);
char **read_key(char *key, int *len, int *node_length);
char *text_decoder(char *key, char *buffer);
char *text_encoder(char *key, char *buffer);
