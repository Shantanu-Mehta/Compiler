#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ast.h"

static void indent(int level) {
    for (int i = 0; i < level; i++) printf("  ");
}


ASTNode* create_number_node(int value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_NUMBER;
    node->data.number = value;
    return node;
}


ASTNode* create_variable_node(const char* name) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_VARIABLE;
    strncpy(node->data.variable.name, name, sizeof(node->data.variable.name) - 1);
    node->data.variable.name[sizeof(node->data.variable.name) - 1] = '\0';
    return node;
}


ASTNode* create_declaration_node(const char* var_name, ASTNode* initial_value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_DECLARATION;
    strncpy(node->data.declaration.name, var_name, sizeof(node->data.declaration.name) - 1);
    node->data.declaration.name[sizeof(node->data.declaration.name) - 1] = '\0';
    node->data.declaration.initial_value = initial_value;  
    return node;
}


ASTNode* create_declaration_with_init_node(const char* var_name, ASTNode* value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_DECLARATION_WITH_INIT;
    strncpy(node->data.declaration.name, var_name, sizeof(node->data.declaration.name) - 1);
    node->data.declaration.name[sizeof(node->data.declaration.name) - 1] = '\0';
    node->data.declaration.initial_value = value;
    return node;
}

ASTNode* create_return_node(ASTNode* value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_RETURN;
    node->data.return_stmt.value = value;
    return node;
}



ASTNode* create_binary_op_node(const char* op, ASTNode* left, ASTNode* right) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_BINARY_OP;
    strncpy(node->data.binary.op, op, sizeof(node->data.binary.op) - 1);
    node->data.binary.op[sizeof(node->data.binary.op) - 1] = '\0';
    node->data.binary.left = left;
    node->data.binary.right = right;
    return node;
}


ASTNode* create_assignment_node(const char* var, ASTNode* value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_ASSIGNMENT;
    strncpy(node->data.assignment.var, var, sizeof(node->data.assignment.var) - 1);
    node->data.assignment.var[sizeof(node->data.assignment.var) - 1] = '\0';
    node->data.assignment.value = value;
    return node;
}


ASTNode* create_print_node(ASTNode* expr) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_PRINT;
    node->data.print_expr = expr;
    return node;
}


ASTNode* create_formatted_print_node(const char* format, ASTNode** arguments, int arg_count) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_FORMATTED_PRINT;
    strncpy(node->data.formatted_print.format, format, sizeof(node->data.formatted_print.format) - 1);
    node->data.formatted_print.format[sizeof(node->data.formatted_print.format) - 1] = '\0';
    node->data.formatted_print.args = arguments;
    node->data.formatted_print.arg_count = arg_count;
    return node;
}


ASTNode* create_sequence_node(ASTNode** statements, int count) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_SEQUENCE;
    node->data.sequence.statements = statements;
    node->data.sequence.count = count;
    return node;
}


ASTNode* create_if_node(ASTNode* condition, ASTNode* then_branch, ASTNode* else_branch) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_IF;
    node->data.if_stmt.condition = condition;
    node->data.if_stmt.then_branch = then_branch;
    node->data.if_stmt.else_branch = else_branch;
    return node;
}

ASTNode* create_while_node(ASTNode* condition, ASTNode* body) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_WHILE;
    node->data.while_stmt.condition = condition;
    node->data.while_stmt.body = body;
    return node;
}


ASTNode* create_for_node(ASTNode* init, ASTNode* cond, ASTNode* update, ASTNode* body) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_FOR;
    node->data.for_stmt.init = init;
    node->data.for_stmt.cond = cond;
    node->data.for_stmt.update = update;
    node->data.for_stmt.body = body;
    return node;
}


ASTNode* create_string_node(const char* str) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_STRING;
    strncpy(node->data.string.value, str, sizeof(node->data.string.value) - 1);
    node->data.string.value[sizeof(node->data.string.value) - 1] = '\0';
    return node;
}


ASTNode* create_function_node(const char* name, ASTNode* body) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_FUNCTION;
    strncpy(node->data.function.name, name, sizeof(node->data.function.name) - 1);
    node->data.function.name[sizeof(node->data.function.name) - 1] = '\0';
    node->data.function.body = body;
    return node;
}


void print_ast(ASTNode* node, int level) {
    if (!node) return;

    indent(level);
    switch (node->type) {
        case AST_NUMBER:
            printf("Number: %d\n", node->data.number);
            break;
        case AST_VARIABLE:
            printf("Variable: %s\n", node->data.variable.name);
            break;
        case AST_STRING:
            printf("String: \"%s\"\n", node->data.string.value);
            break;
        case AST_DECLARATION:
             printf("Declaration with init: %s\n", node->data.declaration.name);
             if (node->data.declaration.initial_value) {
             indent(level + 1);
             printf("Initial value:\n");
             print_ast(node->data.declaration.initial_value, level + 2);
             }
             break;

        case AST_DECLARATION_WITH_INIT:
             printf("Declaration with init: %s\n", node->data.declaration.name);
             indent(level + 1);
             printf("Initial value:\n");
             print_ast(node->data.declaration.initial_value, level + 2);
            break;
    
        case AST_ASSIGNMENT:
            printf("Assignment to: %s\n", node->data.assignment.var);
            print_ast(node->data.assignment.value, level + 1);
            break;
        case AST_BINARY_OP:
            printf("Binary Op: %s\n", node->data.binary.op);
            print_ast(node->data.binary.left, level + 1);
            print_ast(node->data.binary.right, level + 1);
            break;
        case AST_PRINT:
            printf("Print:\n");
            print_ast(node->data.print_expr, level + 1);
            break;
        case AST_FORMATTED_PRINT:
            printf("Formatted Print: \"%s\"\n", node->data.formatted_print.format);
            for (int i = 0; i < node->data.formatted_print.arg_count; i++) {
                print_ast(node->data.formatted_print.args[i], level + 1);
            }
            break;
        case AST_SEQUENCE:
            printf("Sequence:\n");
            for (int i = 0; i < node->data.sequence.count; i++) {
                print_ast(node->data.sequence.statements[i], level + 1);
            }
            break;
        case AST_IF:
            printf("If:\n");
            indent(level + 1); printf("Condition:\n");
            print_ast(node->data.if_stmt.condition, level + 2);
            indent(level + 1); printf("Then:\n");
            print_ast(node->data.if_stmt.then_branch, level + 2);
            if (node->data.if_stmt.else_branch) {
                indent(level + 1); printf("Else:\n");
                print_ast(node->data.if_stmt.else_branch, level + 2);
            }
            break;
        case AST_WHILE:
            printf("While:\n");
            indent(level + 1); printf("Condition:\n");
            print_ast(node->data.while_stmt.condition, level + 2);
            indent(level + 1); printf("Body:\n");
            print_ast(node->data.while_stmt.body, level + 2);
            break;
        case AST_FOR:
            printf("For:\n");
            indent(level + 1); printf("Init:\n");
            print_ast(node->data.for_stmt.init, level + 2);
            indent(level + 1); printf("Condition:\n");
            print_ast(node->data.for_stmt.cond, level + 2);
            indent(level + 1); printf("Update:\n");
            print_ast(node->data.for_stmt.update, level + 2);
            indent(level + 1); printf("Body:\n");
            print_ast(node->data.for_stmt.body, level + 2);
            break;
        case AST_RETURN:
                  printf("Return:\n");
               if (node->data.return_stmt.value != NULL) {
               print_ast(node->data.return_stmt.value, level + 1);
                } else {
                 printf("  (no return value)\n");
                 }
            break;
    
        case AST_FUNCTION:  // NEW
            printf("Function: %s\n", node->data.function.name);
            indent(level + 1);
            printf("Body:\n");
            print_ast(node->data.function.body, level + 2);
            break;
        default:
            indent(level);
            printf("Unknown AST node type\n");
            break;
    }
}


void free_ast(ASTNode* node) {
    if (!node) return;

    switch (node->type) {
        case AST_BINARY_OP:
            free_ast(node->data.binary.left);
            free_ast(node->data.binary.right);
            break;
        case AST_ASSIGNMENT:
            free_ast(node->data.assignment.value);
            break;
        case AST_PRINT:
            free_ast(node->data.print_expr);
            break;
        case AST_FORMATTED_PRINT:
            for (int i = 0; i < node->data.formatted_print.arg_count; i++) {
                free_ast(node->data.formatted_print.args[i]);
            }
            free(node->data.formatted_print.args);
            break;
        case AST_DECLARATION_WITH_INIT:
              free_ast(node->data.declaration.initial_value);
            break;
    
        case AST_SEQUENCE:
            for (int i = 0; i < node->data.sequence.count; i++) {
                free_ast(node->data.sequence.statements[i]);
            }
            free(node->data.sequence.statements);
            break;
        case AST_IF:
            free_ast(node->data.if_stmt.condition);
            free_ast(node->data.if_stmt.then_branch);
            if (node->data.if_stmt.else_branch)
                free_ast(node->data.if_stmt.else_branch);
            break;
        case AST_WHILE:
            free_ast(node->data.while_stmt.condition);
            free_ast(node->data.while_stmt.body);
            break;
        case AST_FOR:
            free_ast(node->data.for_stmt.init);
            free_ast(node->data.for_stmt.cond);
            free_ast(node->data.for_stmt.update);
            free_ast(node->data.for_stmt.body);
            break;
        case AST_FUNCTION:  
            free_ast(node->data.function.body);
            break;
        case AST_STRING:
        case AST_VARIABLE:
        case AST_DECLARATION:
        case AST_NUMBER:
            
            break;
        default:
            break;
    }

    free(node);
}
