#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>   //Time function for srand seed.
#include <assert.h>
#include <math.h>

#pragma once

#if defined(WIN32) || defined(_WIN32)
    #include <Windows.h>
    #define sleep(ms) _sleep(ms)
    #define popen(cmd, mode) _popen(cmd, mode)
#else 
    #include <unistd.h>
#endif

#define bool _Bool
#define true 1
#define false 0

#define BASIC_UNICODE 128
#define ALL_UNICODE 255


char *generate_key(int range, int node_size);
char* random_string(int maximum, int range);
char **read_key(char *key, int *len);
char *text_decoder(char *key, char *buffer);