#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interpreter.h"
#include "symbol_table.h"

int temp_count = 0;
int label_count = 0;

char* new_temp() {
    char* name = malloc(16);
    snprintf(name, 16, "t%d", temp_count++);
    return name;
}

char* new_label() {
    char* label = malloc(16);
    snprintf(label, 16, "L%d", label_count++);
    return label;
}

char* generate_tac(const ASTNode* node) {
    if (!node) return NULL;

    switch (node->type) {
        case AST_NUMBER: {
    char* temp = new_temp();
    printf("%s = %d\n", temp, node->data.number);
    return temp;  
}


        case AST_VARIABLE: {
              return strdup(node->data.variable.name); 
            }


        case AST_ASSIGNMENT: {
            char* rhs = generate_tac(node->data.assignment.value);
            printf("%s = %s\n", node->data.assignment.var, rhs);
            return strdup(node->data.assignment.var);
        }

        case AST_BINARY_OP: {
    char* left = generate_tac(node->data.binary.left);
    char* right = generate_tac(node->data.binary.right);
    char* result = new_temp();
    printf("%s = %s %s %s\n", result, left, node->data.binary.op, right);
    return result;  
}


        case AST_PRINT: {
            char* expr = generate_tac(node->data.print_expr);
            printf("print %s\n", expr);
            return NULL;
        }

        
        case AST_FORMATTED_PRINT: {
          
              char** temps = malloc(sizeof(char*) * node->data.formatted_print.arg_count);
               for (int i = 0; i < node->data.formatted_print.arg_count; i++) { 
                temps[i] = generate_tac(node->data.formatted_print.args[i]);
              }

                printf("print \"%s\"", node->data.formatted_print.format);
                 for (int i = 0; i < node->data.formatted_print.arg_count; i++) {
                    printf(", %s", temps[i]);
                      free(temps[i]);
                    }
                printf("\n");

             free(temps);
            return NULL;
                  }

        case AST_DECLARATION_WITH_INIT: {
            if (node->data.declaration.initial_value) {
             char* val = generate_tac(node->data.declaration.initial_value);
              printf("%s = %s\n", node->data.declaration.name, val);
                 free(val);
               }
                return NULL;
           }

 




        case AST_DECLARATION:
            return NULL;

     case AST_SEQUENCE: {
    for (int i = 0; i < node->data.sequence.count; ++i) {
        generate_tac(node->data.sequence.statements[i]);
    }
    return NULL;
}


        case AST_IF: {
            char* else_label = new_label();
            char* end_label = new_label();
            char* cond = generate_tac(node->data.if_stmt.condition);
            printf("ifFalse %s goto %s\n", cond, else_label);
            generate_tac(node->data.if_stmt.then_branch);
            printf("goto %s\n", end_label);
            printf("%s:\n", else_label);
            if (node->data.if_stmt.else_branch) {
                generate_tac(node->data.if_stmt.else_branch);
            }
            printf("%s:\n", end_label);
            return NULL;
        }

        case AST_WHILE: {
            char* start = new_label();
            char* end = new_label();
            printf("%s:\n", start);
            char* cond = generate_tac(node->data.while_stmt.condition);
            printf("ifFalse %s goto %s\n", cond, end);
            generate_tac(node->data.while_stmt.body);
            printf("goto %s\n", start);
            printf("%s:\n", end);
            return NULL;
        }

      case AST_FOR: {
    generate_tac(node->data.for_stmt.init);
    char* start = new_label();
    char* end = new_label();
    printf("%s:\n", start);
    char* cond = generate_tac(node->data.for_stmt.cond);
    printf("ifFalse %s goto %s\n", cond, end);
    generate_tac(node->data.for_stmt.body);
    generate_tac(node->data.for_stmt.update);
    printf("goto %s\n", start);
    printf("%s:\n", end);
    return NULL;
}





         case AST_RETURN: {
    char* val = generate_tac(node->data.return_stmt.value);
    if (val) {
        printf("return %s\n", val);
        free(val);
    } else {
        printf("return\n");
    }
    return NULL;  
}


        default:
            return NULL;
    }
}

int eval(const ASTNode* node) {
    if (!node) return 0;

    switch (node->type) {
        case AST_NUMBER:
            return node->data.number;

        case AST_VARIABLE:
            return get_variable(node->data.variable.name);

        case AST_ASSIGNMENT: {
            int value = eval(node->data.assignment.value);
            set_variable(node->data.assignment.var, value);
            return value;
        }

        case AST_DECLARATION:
            declare_variable(node->data.declaration.name);
            return 0;
       
        case AST_DECLARATION_WITH_INIT: {
             int value = eval(node->data.declaration.initial_value);
             declare_variable(node->data.declaration.name);
             set_variable(node->data.declaration.name, value);
             return 0;
           }
    
        case AST_BINARY_OP: {
            int left = eval(node->data.binary.left);
            int right = eval(node->data.binary.right);
            if (strcmp(node->data.binary.op, "+") == 0) return left + right;
            if (strcmp(node->data.binary.op, "-") == 0) return left - right;
            if (strcmp(node->data.binary.op, "*") == 0) return left * right;
            if (strcmp(node->data.binary.op, "/") == 0) return right != 0 ? left / right : 0;
            if (strcmp(node->data.binary.op, "%") == 0) return right != 0 ? left % right : 0;
            if (strcmp(node->data.binary.op, "==") == 0) return left == right;
            if (strcmp(node->data.binary.op, "!=") == 0) return left != right;
            if (strcmp(node->data.binary.op, "<") == 0) return left < right;
            if (strcmp(node->data.binary.op, "<=") == 0) return left <= right;
            if (strcmp(node->data.binary.op, ">") == 0) return left > right;
            if (strcmp(node->data.binary.op, ">=") == 0) return left >= right;
        }

        case AST_PRINT: {
            int val = eval(node->data.print_expr);
            printf("%d\n", val);
            return val;
        }

        case AST_FORMATTED_PRINT: {
            // For now we only support one argument, expand if needed
            if (node->data.formatted_print.arg_count == 1) {
                int arg_val = eval(node->data.formatted_print.args[0]);
                printf(node->data.formatted_print.format, arg_val);
                printf("\n");
            } else {
                printf(node->data.formatted_print.format);
                printf("\n");
            }
            return 0;
        }

        case AST_IF: {
            int cond = eval(node->data.if_stmt.condition);
            if (cond) return eval(node->data.if_stmt.then_branch);
            else if (node->data.if_stmt.else_branch)
                return eval(node->data.if_stmt.else_branch);
            return 0;
        }

        case AST_WHILE: {
            while (eval(node->data.while_stmt.condition)) {
                eval(node->data.while_stmt.body);
            }
            return 0;
        }

        case AST_FOR: {
            for (
                eval(node->data.for_stmt.init);
                eval(node->data.for_stmt.cond);
                eval(node->data.for_stmt.update)
            ) {
                eval(node->data.for_stmt.body);
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






        default:
            return 0;
    }
}
