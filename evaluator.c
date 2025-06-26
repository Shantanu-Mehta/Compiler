#include "evaluator.h"
#include "symbol_table.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int eval(const ASTNode* node);


int evaluate_ast(ASTNode* node) {
    if (!node) return 0;

    switch (node->type) {
        case AST_NUMBER:
            return node->data.number;

        case AST_VARIABLE:
            return get_variable(node->data.variable.name);

        case AST_BINARY_OP: {
            int left = evaluate_ast(node->data.binary.left);
            int right = evaluate_ast(node->data.binary.right);
            const char* op = node->data.binary.op;

            if (strcmp(op, "+") == 0) return left + right;
            if (strcmp(op, "-") == 0) return left - right;
            if (strcmp(op, "*") == 0) return left * right;
            if (strcmp(op, "/") == 0) {
                if (right == 0) {
                    fprintf(stderr, "Runtime Error: Division by zero\n");
                    exit(EXIT_FAILURE);
                }
                return left / right;
            }

            fprintf(stderr, "Runtime Error: Unknown binary operator: %s\n", op);
            exit(EXIT_FAILURE);
        }

        case AST_ASSIGNMENT: {
            int value = evaluate_ast(node->data.assignment.value);
            set_variable(node->data.assignment.var, value);
            return value;
        }

        case AST_DECLARATION:
            declare_variable(node->data.declaration.name);
               if (node->data.declaration.initial_value) 
               {
               int value = evaluate_ast(node->data.declaration.initial_value);  
               set_variable(node->data.declaration.name, value);
              }
             return 0;



       case AST_DECLARATION_WITH_INIT: {
            const char* var_name = node->data.declaration.name;
           int value = evaluate_ast(node->data.declaration.initial_value);
           declare_variable(var_name); 
           set_variable(var_name, value);
           return 0;
              }



        case AST_PRINT: {
            int val = evaluate_ast(node->data.print_expr);
            printf("%d\n", val);
            return 0;
        }

        case AST_STRING: {
            printf("%s\n", node->data.string.value);
            return 0;
        }

        case AST_FORMATTED_PRINT: {
             printf(node->data.formatted_print.format, evaluate_ast(node->data.formatted_print.args[0]));
              printf("\n");
          return 0;
                }
 



        case AST_IF: {
    int cond = evaluate_ast(node->data.if_stmt.condition);
    if (cond) {
        evaluate_ast(node->data.if_stmt.then_branch);
    } else if (node->data.if_stmt.else_branch) {
        evaluate_ast(node->data.if_stmt.else_branch);
    }
    return 0;
}


        case AST_WHILE: {
            while (evaluate_ast(node->data.while_stmt.condition)) {
                evaluate_ast(node->data.while_stmt.body);
            }
            return 0;
        }

      case AST_FOR: {
    evaluate_ast(node->data.for_stmt.init);
    while (evaluate_ast(node->data.for_stmt.cond)) {
        evaluate_ast(node->data.for_stmt.body);
        evaluate_ast(node->data.for_stmt.update);
    }
    return 0;
}



    
case AST_SEQUENCE: {
    int result = 0;
    for (int i = 0; i < node->data.sequence.count; ++i) {
        result = eval(node->data.sequence.statements[i]);
    }
    return result;
}



        case AST_RETURN: {
            int val = evaluate_ast(node->data.return_stmt.value);
                
            printf("Return: %d\n", val);
            exit(EXIT_SUCCESS);  
                     }


        default:
            fprintf(stderr, "Runtime Error: Unknown AST node type: %d\n", node->type);
            exit(EXIT_FAILURE);
    }
}
