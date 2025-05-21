#include "symbol_table.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_SYMBOLS 100

typedef struct {
    char name[100];
    int value;
} Symbol;

static Symbol table[MAX_SYMBOLS];
static int symbol_count = 0;

void set_variable(const char* name, int value) {
    for (int i = 0; i < symbol_count; ++i) {
        if (strcmp(table[i].name, name) == 0) {
            table[i].value = value;
            return;
        }
    }

    if (symbol_count >= MAX_SYMBOLS) {
        printf("Symbol table overflow. Cannot store variable '%s'\n", name);
        exit(1);
    }

    strcpy(table[symbol_count].name, name);
    table[symbol_count].value = value;
    symbol_count++;
}

int get_variable(const char* name) {
    for (int i = 0; i < symbol_count; ++i) {
        if (strcmp(table[i].name, name) == 0) {
            return table[i].value;
        }
    }

    printf("Error: Undefined variable '%s'\n", name);
    exit(1);
}

void reset_symbol_table() {
    symbol_count = 0;
}
