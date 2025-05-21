#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ASTNode* parse_expression(Token* tokens, int* index);
ASTNode* parse_term(Token* tokens, int* index);
ASTNode* parse_factor(Token* tokens, int* index);
ASTNode* parse_statement(Token* tokens, int* index);

// ✅ Parser Entry: Parses list of statements into a sequence node
ASTNode* parse(Token* tokens, int* index) {
    ASTNode* statements[256];  // Fixed size
    int count = 0;

    while (tokens[*index].type != TOKEN_EOF) {
        statements[count++] = parse_statement(tokens, index);
    }

    return create_sequence_node(statements, count);
}

// ✅ Handles assignment and print
ASTNode* parse_statement(Token* tokens, int* index) {
    if (tokens[*index].type == TOKEN_IDENTIFIER &&
        tokens[*index + 1].type == TOKEN_OPERATOR &&
        strcmp(tokens[*index + 1].value, "=") == 0) {

        char name[64];
        strcpy(name, tokens[*index].value);
        *index += 2;
        ASTNode* value = parse_expression(tokens, index);

        if (tokens[*index].type == TOKEN_SYMBOL && strcmp(tokens[*index].value, ";") == 0) {
            (*index)++;
        } else {
            fprintf(stderr, "Expected ';' after assignment\n");
            exit(1);
        }

        return create_assignment_node(name, value);

    } else if (tokens[*index].type == TOKEN_KEYWORD &&
               strcmp(tokens[*index].value, "print") == 0) {

        (*index)++;
        ASTNode* expr = parse_expression(tokens, index);

        if (tokens[*index].type == TOKEN_SYMBOL && strcmp(tokens[*index].value, ";") == 0) {
            (*index)++;
        } else {
           fprintf(stderr, "Syntax Error: Expected ';' after assignment near '%s'\n", tokens[*index].value);
           exit(1);

        }

        return create_print_node(expr);

    } else {
        fprintf(stderr, "Syntax error near '%s'\n", tokens[*index].value);
        exit(1);
    }
}

// ✅ Handles `+` and `-`
ASTNode* parse_expression(Token* tokens, int* index) {
    ASTNode* node = parse_term(tokens, index);

    while (tokens[*index].type == TOKEN_OPERATOR &&
           (strcmp(tokens[*index].value, "+") == 0 || strcmp(tokens[*index].value, "-") == 0)) {
        char op = tokens[*index].value[0];
        (*index)++;
        ASTNode* right = parse_term(tokens, index);
        node = create_binary_op_node(op, node, right);
    }

    return node;
}

// ✅ Handles `*` and `/`
ASTNode* parse_term(Token* tokens, int* index) {
    ASTNode* node = parse_factor(tokens, index);

    while (tokens[*index].type == TOKEN_OPERATOR &&
           (strcmp(tokens[*index].value, "*") == 0 || strcmp(tokens[*index].value, "/") == 0)) {
        char op = tokens[*index].value[0];
        (*index)++;
        ASTNode* right = parse_factor(tokens, index);
        node = create_binary_op_node(op, node, right);
    }

    return node;
}

// ✅ Handles number, identifier, and `(expression)`
ASTNode* parse_factor(Token* tokens, int* index) {
    if (tokens[*index].type == TOKEN_NUMBER) {
        int value = atoi(tokens[*index].value);
        (*index)++;
        return create_number_node(value);

    } else if (tokens[*index].type == TOKEN_IDENTIFIER) {
        char name[64];
        strcpy(name, tokens[*index].value);
        (*index)++;
        return create_variable_node(name);

    } else if (tokens[*index].type == TOKEN_SYMBOL &&
               strcmp(tokens[*index].value, "(") == 0) {
        (*index)++;
        ASTNode* node = parse_expression(tokens, index);

        if (tokens[*index].type == TOKEN_SYMBOL && strcmp(tokens[*index].value, ")") == 0) {
            (*index)++;
            return node;
        } else {
            fprintf(stderr, "Expected ')' after expression\n");
            exit(1);
        }

    } else {
        fprintf(stderr, "Unexpected token '%s' in expression\n", tokens[*index].value);
        exit(1);
    }
}
