#include "tokenizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' ||
           c == '<' || c == '>' || c == '!';
}

int is_symbol_char(char c) {
    return c == ';' || c == '{' || c == '}' || c == '(' || c == ')' || c == ',' || c == '[' || c == ']';
}


int is_keyword(const char* str) {
    return strcmp(str, "auto") == 0 ||
            strcmp(str, "print") == 0 || 
           strcmp(str, "break") == 0 ||
           strcmp(str, "case") == 0 ||
           strcmp(str, "char") == 0 ||
           strcmp(str, "const") == 0 ||
           strcmp(str, "continue") == 0 ||
           strcmp(str, "default") == 0 ||
           strcmp(str, "do") == 0 ||
           strcmp(str, "double") == 0 ||
           strcmp(str, "else") == 0 ||
           strcmp(str, "enum") == 0 ||
           strcmp(str, "extern") == 0 ||
           strcmp(str, "float") == 0 ||
           strcmp(str, "for") == 0 ||
           strcmp(str, "goto") == 0 ||
           strcmp(str, "if") == 0 ||
           strcmp(str, "inline") == 0 ||
           strcmp(str, "int") == 0 ||
           strcmp(str, "long") == 0 ||
           strcmp(str, "register") == 0 ||
           strcmp(str, "restrict") == 0 ||
           strcmp(str, "return") == 0 ||
           strcmp(str, "short") == 0 ||
           strcmp(str, "signed") == 0 ||
           strcmp(str, "sizeof") == 0 ||
           strcmp(str, "static") == 0 ||
           strcmp(str, "struct") == 0 ||
           strcmp(str, "switch") == 0 ||
           strcmp(str, "typedef") == 0 ||
           strcmp(str, "union") == 0 ||
           strcmp(str, "unsigned") == 0 ||
           strcmp(str, "void") == 0 ||
           strcmp(str, "volatile") == 0 ||
           strcmp(str, "while") == 0;
}
const char* token_type_to_string(int type) {
    switch(type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_SYMBOL: return "SYMBOL";
        case TOKEN_KEYWORD: return "KEYWORD";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}
    


Token* tokenize(const char* input, int* token_count) {
    Token* tokens = malloc(sizeof(Token) * MAX_TOKENS);
    if (!tokens) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    int count = 0;
    int i = 0;

    while (input[i] != '\0') {
        if (isspace(input[i])) {
            i++;
            continue;
        }

        // Numbers (integers only)
        if (isdigit(input[i])) {
            int j = 0;
            char number[64];
            while (isdigit(input[i])) {
                if (j < 63) number[j++] = input[i];
                i++;
            }
            number[j] = '\0';

            if (count >= MAX_TOKENS) {
                fprintf(stderr, "Error: Too many tokens. Increase MAX_TOKENS.\n");
                exit(1);
            }

            tokens[count].type = TOKEN_NUMBER;
            strcpy(tokens[count].value, number);
            count++;
            continue;
        }

    
        if (isalpha(input[i]) || input[i] == '_') {
            int j = 0;
            char ident[64];
            while (isalnum(input[i]) || input[i] == '_') {
                if (j < 63) ident[j++] = input[i];
                i++;
            }
            ident[j] = '\0';

            if (count >= MAX_TOKENS) {
                fprintf(stderr, "Error: Too many tokens. Increase MAX_TOKENS.\n");
                exit(1);
            }

            tokens[count].type = is_keyword(ident) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
            strcpy(tokens[count].value, ident);
            count++;
            continue;
        }

       
        if ((input[i] == '=' && input[i+1] == '=') ||
            (input[i] == '!' && input[i+1] == '=') ||
            (input[i] == '<' && input[i+1] == '=') ||
            (input[i] == '>' && input[i+1] == '=')) {

            if (count >= MAX_TOKENS) {
                fprintf(stderr, "Error: Too many tokens. Increase MAX_TOKENS.\n");
                exit(1);
            }

            tokens[count].type = TOKEN_OPERATOR;
            tokens[count].value[0] = input[i];
            tokens[count].value[1] = input[i+1];
            tokens[count].value[2] = '\0';
            i += 2;
            count++;
            continue;
        }

        // Single character operator
        if (is_operator_char(input[i])) {
            if (count >= MAX_TOKENS) {
                fprintf(stderr, "Error: Too many tokens. Increase MAX_TOKENS.\n");
                exit(1);
            }

            tokens[count].type = TOKEN_OPERATOR;
            tokens[count].value[0] = input[i];
            tokens[count].value[1] = '\0';
            i++;
            count++;
            continue;
        }

        // Symbols
        if (is_symbol_char(input[i])) {
            if (count >= MAX_TOKENS) {
                fprintf(stderr, "Error: Too many tokens. Increase MAX_TOKENS.\n");
                exit(1);
            }

            tokens[count].type = TOKEN_SYMBOL;
            tokens[count].value[0] = input[i];
            tokens[count].value[1] = '\0';
            i++;
            count++;
            continue;
        }

       
        fprintf(stderr, "Unknown character: %c\n", input[i]);
        exit(1);
    }

    if (count >= MAX_TOKENS) {
        fprintf(stderr, "Error: Too many tokens. Increase MAX_TOKENS.\n");
        exit(1);
    }

    tokens[count].type = TOKEN_EOF;
    strcpy(tokens[count].value, "EOF");
    count++;

    *token_count = count;
    return tokens;
}
