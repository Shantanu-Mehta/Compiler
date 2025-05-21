#include <stdlib.h>
#include <string.h>
#include <stdio.h>  
#include "ast.h"

ASTNode* create_number_node(int value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_NUMBER;
    node->data.number = value;
    return node;
}

ASTNode* create_variable_node(const char* name) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_VARIABLE;
    strcpy(node->data.var_name, name);
    return node;
}

ASTNode* create_binary_op_node(char op, ASTNode* left, ASTNode* right) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_BINARY_OP;
    node->data.binary.op = op;
    node->data.binary.left = left;
    node->data.binary.right = right;
    return node;
}

ASTNode* create_assignment_node(const char* var, ASTNode* value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_ASSIGNMENT;
    strcpy(node->data.assignment.var, var);
    node->data.assignment.value = value;
    return node;
}

ASTNode* create_print_node(ASTNode* expr) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_PRINT;
    node->data.print_expr = expr;
    return node;
}

ASTNode* create_sequence_node(ASTNode** statements, int count) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_SEQUENCE;
    node->data.sequence.statements = statements;
    node->data.sequence.count = count;
    return node;
}

void print_ast(ASTNode* node, int level) {
    if (!node) return;

    for (int i = 0; i < level; i++) printf("  ");

    switch (node->type) {
        case AST_NUMBER:
            printf("Number: %d\n", node->data.number);
            break;
        case AST_VARIABLE:
            printf("Variable: %s\n", node->data.var_name);
            break;
        case AST_ASSIGNMENT:
            printf("Assignment to %s\n", node->data.assignment.var);
            print_ast(node->data.assignment.value, level + 1);
            break;
        case AST_BINARY_OP:
            printf("Binary Op: %c\n", node->data.binary.op);
            print_ast(node->data.binary.left, level + 1);
            print_ast(node->data.binary.right, level + 1);
            break;
        case AST_PRINT:
            printf("Print:\n");
            print_ast(node->data.print_expr, level + 1);
            break;
        case AST_SEQUENCE:
            printf("Sequence of %d statements\n", node->data.sequence.count);
            for (int i = 0; i < node->data.sequence.count; i++)
                print_ast(node->data.sequence.statements[i], level + 1);
            break;
        default:
            printf("Unknown AST node\n");
    }
}
