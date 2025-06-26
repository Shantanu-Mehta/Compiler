#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"
#include "parser.h"
#include "ast.h"
#include "symbol_table.h"
#include "interpreter.h"

#define INITIAL_SIZE 4096

int main() {
    char *input = malloc(INITIAL_SIZE);
    if (!input) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    size_t capacity = INITIAL_SIZE;
    size_t length = 0;
    int c;

    while ((c = getchar()) != EOF) {
        if (length + 1 >= capacity) {
            capacity *= 2;
            char *new_input = realloc(input, capacity);
            if (!new_input) {
                fprintf(stderr, "Memory allocation failed during resizing\n");
                free(input);
                return 1;
            }
            input = new_input;
        }
        input[length++] = (char)c;
    }
    input[length] = '\0';

    
    int token_count = 0;
    Token *tokens = tokenize(input, &token_count);
    if (!tokens) {
        fprintf(stderr, "Tokenization failed\n");
        free(input);
        return 1;
    }

    printf("\n=== TOKENS ===\n");
    for (int i = 0; i < token_count; i++) {
        printf("Type: %s, Value: %s\n", token_type_to_string(tokens[i].type), tokens[i].value);
    }

    
    int index = 0;
    ASTNode *ast = parse(tokens, &index);
    if (!ast) {
        fprintf(stderr, "Parsing failed\n");
        free(tokens);
        free(input);
        return 1;
    }

    printf("\n=== PARSE TREE ===\n");
    print_ast(ast, 0);
    
    


    
    printf("\n=== THREE-ADDRESS CODE (TAC) ===\n");
   generate_tac(ast);


    
  
printf("\n=== FINAL OUTPUT ===\n");
int result = eval(ast);  





    
    free_ast(ast);
    free(tokens);
    free(input);

    return 0;
}
