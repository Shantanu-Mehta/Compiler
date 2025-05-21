#include "evaluator.h"
#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int evaluate_expression(Token* tokens, int* index) {
    int result = 0;
    char op = '+';

    while (tokens[*index].type != TOKEN_SYMBOL || strcmp(tokens[*index].value, ";") != 0) {
        Token t = tokens[*index];
        if (t.type == TOKEN_NUMBER) {
            int val = atoi(t.value);
            switch (op) {
                case '+': result += val; break;
                case '-': result -= val; break;
                case '*': result *= val; break;
                case '/':
                    if (val == 0) {
                        printf("Error: Division by zero\n");
                        exit(1);
                    }
                    result /= val;
                    break;
            }
        } else if (t.type == TOKEN_IDENTIFIER) {
            int val = get_variable(t.value);
            switch (op) {
                case '+': result += val; break;
                case '-': result -= val; break;
                case '*': result *= val; break;
                case '/':
                    if (val == 0) {
                        printf("Error: Division by zero (identifier value is zero)\n");
                        exit(1);
                    }
                    result /= val;
                    break;
            }
        } else if (t.type == TOKEN_OPERATOR) {
            op = t.value[0];
        }
        (*index)++;
    }

    return result;
}

int evaluate_condition(Token* tokens, int* index) {
    int left = get_variable(tokens[(*index)++].value);
    char* op = tokens[(*index)++].value;
    int right = get_variable(tokens[(*index)++].value);

    if (strcmp(op, "==") == 0) return left == right;
    if (strcmp(op, "!=") == 0) return left != right;
    if (strcmp(op, ">") == 0) return left > right;
    if (strcmp(op, "<") == 0) return left < right;
    if (strcmp(op, ">=") == 0) return left >= right;
    if (strcmp(op, "<=") == 0) return left <= right;

    printf("Unknown operator in condition: %s\n", op);
    exit(1);
}

void evaluate_tokens(Token* tokens, int token_count) {
    int i = 0;

    while (i < token_count && tokens[i].type != TOKEN_EOF) {
        if (tokens[i].type == TOKEN_IDENTIFIER &&
            tokens[i+1].type == TOKEN_OPERATOR &&
            (strcmp(tokens[i+1].value, "=") == 0 ||
             strcmp(tokens[i+1].value, "+=") == 0 ||
             strcmp(tokens[i+1].value, "-=") == 0)) {

            char varname[100];
            strcpy(varname, tokens[i].value);
            char assign_op[3];
            strcpy(assign_op, tokens[i+1].value);
            i += 2;
            int val = evaluate_expression(tokens, &i);
            if (strcmp(assign_op, "=") == 0)
                set_variable(varname, val);
            else if (strcmp(assign_op, "+=") == 0)
                set_variable(varname, get_variable(varname) + val);
            else if (strcmp(assign_op, "-=") == 0)
                set_variable(varname, get_variable(varname) - val);
            i++; // skip ;

        } else if (tokens[i].type == TOKEN_KEYWORD && strcmp(tokens[i].value, "if") == 0) {
            i++;
            int cond = evaluate_condition(tokens, &i);
            i++; // skip '{'
            int block_start = i;
            int brace = 1;
            while (brace != 0) {
                if (strcmp(tokens[i].value, "{") == 0) brace++;
                else if (strcmp(tokens[i].value, "}") == 0) brace--;
                i++;
            }
            int block_end = i - 1;
            if (cond)
                evaluate_tokens(&tokens[block_start], block_end - block_start);

            if (tokens[i].type == TOKEN_KEYWORD && strcmp(tokens[i].value, "else") == 0) {
                i++; i++; 
                block_start = i;
                brace = 1;
                while (brace != 0) {
                    if (strcmp(tokens[i].value, "{") == 0) brace++;
                    else if (strcmp(tokens[i].value, "}") == 0) brace--;
                    i++;
                }
                block_end = i - 1;
                if (!cond)
                    evaluate_tokens(&tokens[block_start], block_end - block_start);
            }

        } else if (tokens[i].type == TOKEN_KEYWORD && strcmp(tokens[i].value, "while") == 0) {
            i++;
            int cond_start = i;
            int brace_pos = i + 3; 
            while (tokens[brace_pos].type != TOKEN_SYMBOL || strcmp(tokens[brace_pos].value, "{") != 0)
                brace_pos++;

            int loop_start = brace_pos + 1;
            int brace = 1;
            int loop_end = loop_start;
            while (brace != 0) {
                if (strcmp(tokens[loop_end].value, "{") == 0) brace++;
                else if (strcmp(tokens[loop_end].value, "}") == 0) brace--;
                loop_end++;
            }

            while (evaluate_condition(tokens, &cond_start)) {
                evaluate_tokens(&tokens[loop_start], loop_end - loop_start - 1);
                cond_start = i + 1;
            }
            i = loop_end;

        } else if (tokens[i].type == TOKEN_KEYWORD && strcmp(tokens[i].value, "print") == 0) {
            i++;
            printf("%d\n", get_variable(tokens[i++].value));
            i++; 
        } else {
            printf("Syntax error near '%s'\n", tokens[i].value);
            exit(1);
        }
    }
}
