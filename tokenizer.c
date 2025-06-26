#include "tokenizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

bool is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' ||
           c == '<' || c == '>' || c == '!' || c == '%';
}

bool is_symbol_char(char c) {
    return c == ';' || c == '{' || c == '}' || c == '(' || c == ')' ||
           c == ',' || c == '[' || c == ']' || c == '.' || c == ':';
}

int is_keyword(const char* str) {
    return strcmp(str, "print") == 0 || strcmp(str, "printf") == 0 ||
           strcmp(str, "if") == 0 || strcmp(str, "else") == 0 ||
           strcmp(str, "for") == 0 || strcmp(str, "while") == 0 ||
           strcmp(str, "return") == 0 || strcmp(str, "int") == 0 ||
           strcmp(str, "float") == 0 || strcmp(str, "char") == 0 ||
           strcmp(str, "void") == 0 || strcmp(str, "main") == 0 ||
           strcmp(str, "include") == 0;
}

const char* token_type_to_string(int type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_SYMBOL: return "SYMBOL";
        case TOKEN_KEYWORD: return "KEYWORD";
        case TOKEN_STRING: return "STRING";
        case TOKEN_PREPROCESSOR: return "PREPROCESSOR";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

void print_tokens(Token* tokens, int count) {
    printf("=== TOKENS ===\n");
    for (int i = 0; i < count; i++) {
        printf("Type: %-12s, Value: %s\n", token_type_to_string(tokens[i].type), tokens[i].value);
    }
    printf("\n");
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

        if (count >= MAX_TOKENS - 1) {
            fprintf(stderr, "Token limit exceeded\n");
            exit(1);
        }

        // Preprocessor directive
        if (input[i] == '#') {
            int start = i;
            while (input[i] != '\n' && input[i] != '\0') i++;
            int length = i - start;
            strncpy(tokens[count].value, input + start, length);
            tokens[count].value[length] = '\0';
            tokens[count].type = TOKEN_PREPROCESSOR;
            count++;
            if (input[i] == '\n') i++;
            continue;
        }

        
        if (input[i] == '/' && input[i + 1] == '/') {
            i += 2;
            while (input[i] != '\n' && input[i] != '\0') i++;
            continue;
        }

        
        if (input[i] == '/' && input[i + 1] == '*') {
            i += 2;
            while (input[i] && !(input[i] == '*' && input[i + 1] == '/')) i++;
            if (input[i]) i += 2;
            continue;
        }

        
        if (input[i] == '"') {
            i++;
            char str[256];
            int j = 0;
            while (input[i] != '"' && input[i] != '\0') {
                if (j < 255) str[j++] = input[i++];
                else i++;
            }
            str[j] = '\0';
            if (input[i] == '"') i++;

            tokens[count].type = TOKEN_STRING;
            strncpy(tokens[count].value, str, MAX_TOKEN_LENGTH - 1);
            tokens[count].value[MAX_TOKEN_LENGTH - 1] = '\0';
            count++;
            continue;
        }

        
        if (isdigit(input[i])) {
            char number[64];
            int j = 0;
            while (isdigit(input[i])) {
                if (j < 63) number[j++] = input[i++];
                else i++;
            }
            number[j] = '\0';

            tokens[count].type = TOKEN_NUMBER;
            strncpy(tokens[count].value, number, MAX_TOKEN_LENGTH - 1);
            tokens[count].value[MAX_TOKEN_LENGTH - 1] = '\0';
            count++;
            continue;
        }

        
        if (isalpha(input[i]) || input[i] == '_') {
            char ident[64];
            int j = 0;
            while (isalnum(input[i]) || input[i] == '_') {
                if (j < 63) ident[j++] = input[i++];
                else break;
            }
            ident[j] = '\0';

            tokens[count].type = is_keyword(ident) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
            strncpy(tokens[count].value, ident, MAX_TOKEN_LENGTH - 1);
            tokens[count].value[MAX_TOKEN_LENGTH - 1] = '\0';
            count++;
            continue;
        }

        
        if ((input[i] == '=' && input[i+1] == '=') ||
            (input[i] == '!' && input[i+1] == '=') ||
            (input[i] == '<' && input[i+1] == '=') ||
            (input[i] == '>' && input[i+1] == '=')) {
            tokens[count].type = TOKEN_OPERATOR;
            tokens[count].value[0] = input[i];
            tokens[count].value[1] = input[i + 1];
            tokens[count].value[2] = '\0';
            i += 2;
            count++;
            continue;
        }

        
        if (is_operator_char(input[i])) {
            tokens[count].type = TOKEN_OPERATOR;
            tokens[count].value[0] = input[i++];
            tokens[count].value[1] = '\0';
            count++;
            continue;
        }

        
        if (is_symbol_char(input[i])) {
            tokens[count].type = TOKEN_SYMBOL;
            tokens[count].value[0] = input[i++];
            tokens[count].value[1] = '\0';
            count++;
            continue;
        }

      
        fprintf(stderr, "Skipping unknown character: %c\n", input[i]);
        i++;
    }

   
    tokens[count].type = TOKEN_EOF;
    strcpy(tokens[count].value, "EOF");
    count++;

    *token_count = count;
    return tokens;
}
