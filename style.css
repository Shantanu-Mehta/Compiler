#include "symbol_table.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_SYMBOLS 100
#define ALLOW_AUTO_DECLARE 0  

typedef struct {
    char name[100];
    int value;
    int declared;
} Symbol;

static Symbol symbols[MAX_SYMBOLS];
static int symbol_count = 0;


void declare_variable(const char* name) {
    for (int i = 0; i < symbol_count; ++i) {
        if (strcmp(symbols[i].name, name) == 0) {
            fprintf(stderr, "Semantic Error: Variable '%s' already declared.\n", name);
            exit(EXIT_FAILURE);
        }
    }

    if (symbol_count >= MAX_SYMBOLS) {
        fprintf(stderr, "Symbol table overflow.\n");
        exit(EXIT_FAILURE);
    }

    Symbol* sym = &symbols[symbol_count++];
    strncpy(sym->name, name, sizeof(sym->name) - 1);
    sym->name[sizeof(sym->name) - 1] = '\0';
    sym->value = 0;
    sym->declared = 1;
}


void set_variable(const char* name, int value) {
    for (int i = 0; i < symbol_count; ++i) {
        if (strcmp(symbols[i].name, name) == 0) {
            symbols[i].value = value;
            return;
        }
    }

#if ALLOW_AUTO_DECLARE
    declare_variable(name);
    set_variable(name, value);
#else
    fprintf(stderr, "Semantic Error: Variable '%s' not declared before assignment.\n", name);
    exit(EXIT_FAILURE);
#endif
}


int get_variable(const char* name) {
    for (int i = 0; i < symbol_count; ++i) {
        if (strcmp(symbols[i].name, name) == 0) {
            return symbols[i].value;
        }
    }

    fprintf(stderr, "Semantic Error: Variable '%s' used without declaration.\n", name);
    exit(EXIT_FAILURE);
}


void reset_symbol_table() {
    symbol_count = 0;
}


void print_symbol_table() {
    printf("=== SYMBOL TABLE ===\n");
    for (int i = 0; i < symbol_count; ++i) {
        printf("  %s = %d (declared: %s)\n",
               symbols[i].name,
               symbols[i].value,
               symbols[i].declared ? "yes" : "no");
    }
    printf("====================\n");
}
