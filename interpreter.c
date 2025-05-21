#include "interpreter.h"
#include "ast.h"
#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int temp_count = 0;


char* new_temp() {
    static char name[10];
    sprintf(name, "t%d", temp_count++);
    return name;
}


char* generate_tac(ASTNode* node) {
    if (!node) return NULL;

    switch (node->type) {
        case AST_NUMBER: {
            static char val[32];
            sprintf(val, "%d", node->data.number);
            return val;
        }
        case AST_VARIABLE:
            return node->data.var_name;

        case AST_BINARY_OP: {
            char* left = generate_tac(node->data.binary.left);
            char* right = generate_tac(node->data.binary.right);
            char* temp = new_temp();
            printf("%s = %s %c %s\n", temp, left, node->data.binary.op, right);
            return temp;
        }
        case AST_ASSIGNMENT: {
            char* val = generate_tac(node->data.assignment.value);
            printf("%s = %s\n", node->data.assignment.var, val);
            return node->data.assignment.var;
        }
        case AST_PRINT: {
            char* val = generate_tac(node->data.print_expr);
            printf("print %s\n", val);
            return NULL;
        }
        case AST_SEQUENCE: {
            for (int i = 0; i < node->data.sequence.count; i++) {
                generate_tac(node->data.sequence.statements[i]);
            }
            return NULL;
        }
        default:
            fprintf(stderr, "Unknown AST node type in TAC generation\n");
            exit(1);
    }
}


int eval(ASTNode* node) {
    switch (node->type) {
        case AST_NUMBER:
            return node->data.number;

        case AST_VARIABLE:
            return get_variable(node->data.var_name);

        case AST_BINARY_OP: {
            int left = eval(node->data.binary.left);
            int right = eval(node->data.binary.right);
            switch (node->data.binary.op) {
                case '+': return left + right;
                case '-': return left - right;
                case '*': return left * right;
                case '/':
                    if (right == 0) {
                        printf("Runtime Error: Division by zero\n");
                        exit(1);
                    }
                    return left / right;
                default:
                    printf("Unknown binary operator: %c\n", node->data.binary.op);
                    exit(1);
            }
        }

        case AST_ASSIGNMENT: {
            int value = eval(node->data.assignment.value);
            set_variable(node->data.assignment.var, value);
            return value;
        }

        case AST_PRINT: {
            int value = eval(node->data.print_expr);
            printf("%d\n", value);
            return value;
        }

        case AST_SEQUENCE: {
            int result = 0;
            for (int i = 0; i < node->data.sequence.count; ++i) {
                result = eval(node->data.sequence.statements[i]);
            }
            return result;
        }

        default:
            printf("Unknown AST node type\n");
            exit(1);
    }

    return 0;
}
