#include "tokenizer.h"
#include "ast.h"
#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_STATEMENTS 100


ASTNode* parse_expression(Token* tokens, int* index);
ASTNode* parse_statement(Token* tokens, int* index);
ASTNode* parse_block(Token* tokens, int* index);
ASTNode* parse_if_statement(Token* tokens, int* index);
ASTNode* parse_for_statement(Token* tokens, int* index);


Token current_token(Token* tokens, int index) {
    if (index >= MAX_TOKENS) {
        Token eof_token = { TOKEN_EOF, "" };
        return eof_token;
    }
    return tokens[index];
}

Token next_token(Token* tokens, int* index) {
    if (*index >= MAX_TOKENS) {
        Token eof_token = { TOKEN_EOF, "" };
        return eof_token;
    }
    return tokens[(*index)++];
}

int is_token(Token t, TokenType type, const char* value) {
    return t.type == type && (!value || strcmp(t.value, value) == 0);
}


int get_precedence(const char* op) {
    if (strcmp(op, "=") == 0) return 0;
    if (strcmp(op, "==") == 0 || strcmp(op, "!=") == 0) return 1;
    if (strcmp(op, "<") == 0 || strcmp(op, ">") == 0 || strcmp(op, "<=") == 0 || strcmp(op, ">=") == 0) return 2;
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) return 3;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0 || strcmp(op, "%") == 0) return 4;
    return -1;
}


ASTNode* parse_primary(Token* tokens, int* index) {
    Token t = current_token(tokens, *index);

    if (t.type == TOKEN_EOF) {
        fprintf(stderr, "Syntax Error: Unexpected end of input\n");
        exit(1);
    }

    if (t.type == TOKEN_NUMBER) {
        (*index)++;
        return create_number_node(atoi(t.value));
    }

    if (t.type == TOKEN_STRING) {
        (*index)++;
        return create_string_node(t.value);
    }

    if (t.type == TOKEN_IDENTIFIER) {
        (*index)++;
        return create_variable_node(t.value);
    }

    if (is_token(t, TOKEN_SYMBOL, "(")) {
        (*index)++;
        ASTNode* expr = parse_expression(tokens, index);
        if (!is_token(tokens[*index], TOKEN_SYMBOL, ")")) {
            fprintf(stderr, "Syntax Error: Expected ')' after expression\n");
            exit(1);
        }
        (*index)++;
        return expr;
    }

    fprintf(stderr, "Syntax Error: Unexpected token '%s'\n", t.value);
    exit(1);
}

ASTNode* parse_binary_op(Token* tokens, int* index, int min_prec) {
    ASTNode* left = parse_primary(tokens, index);

    while (tokens[*index].type == TOKEN_OPERATOR &&
           get_precedence(tokens[*index].value) >= min_prec) {
        char op[4];
        strncpy(op, tokens[*index].value, 3);
        op[3] = '\0';
        int prec = get_precedence(op);
        (*index)++;

        ASTNode* right = parse_binary_op(tokens, index, prec + 1);

        if (strcmp(op, "=") == 0) {
            if (left->type != AST_VARIABLE) {
                fprintf(stderr, "Syntax Error: Left-hand side of assignment must be a variable\n");
                exit(1);
            }
            left = create_assignment_node(left->data.variable.name, right);
        } else {
            left = create_binary_op_node(op, left, right);
        }
    }

    return left;
}

ASTNode* parse_expression(Token* tokens, int* index) {
    return parse_binary_op(tokens, index, 0);
}


int is_declaration_keyword(Token t) {
    return is_token(t, TOKEN_KEYWORD, "int") ||
           is_token(t, TOKEN_KEYWORD, "float") ||
           is_token(t, TOKEN_KEYWORD, "char");
}

ASTNode* parse_statement(Token* tokens, int* index) {
    Token t = current_token(tokens, *index);
    if (t.type == TOKEN_EOF) return NULL;

    
    if (is_declaration_keyword(t)) {
        (*index)++;
        ASTNode** declarations = malloc(sizeof(ASTNode*) * 100);
        int count = 0;

        while (1) {
            if (tokens[*index].type != TOKEN_IDENTIFIER) {
                fprintf(stderr, "Syntax Error: Expected identifier\n");
                exit(1);
            }

            char var[64];
            strcpy(var, tokens[*index].value);
            (*index)++;

             if (is_token(tokens[*index], TOKEN_OPERATOR, "=")) 
             {
                 (*index)++;
                 ASTNode* expr = parse_expression(tokens, index);
                 declarations[count++] = create_declaration_with_init_node(var, expr); 
                    } else {
                       declarations[count++] = create_declaration_node(var, NULL);  
                       }

            if (is_token(t, TOKEN_KEYWORD, "return")) {
                (*index)++;
                ASTNode* expr = parse_expression(tokens, index);

                if (!is_token(tokens[*index], TOKEN_SYMBOL, ";")) {
                     fprintf(stderr, "Syntax Error: Expected ';' after return\n");
                     exit(1);
                    }
                (*index)++;

                return create_return_node(expr);  
            }

     
            if (is_token(tokens[*index], TOKEN_SYMBOL, ",")) {
                (*index)++;
            } else if (is_token(tokens[*index], TOKEN_SYMBOL, ";")) {
                (*index)++;
                break;
            } else {
                fprintf(stderr, "Syntax Error: Expected ',' or ';'\n");
                exit(1);
            }
        }

        return create_sequence_node(declarations, count);
    }

       
    if (is_token(t, TOKEN_KEYWORD, "return")) {
        (*index)++;
        ASTNode* expr = parse_expression(tokens, index);

        if (!is_token(tokens[*index], TOKEN_SYMBOL, ";")) {
            fprintf(stderr, "Syntax Error: Expected ';' after return\n");
            exit(1);
        }
        (*index)++;
        return create_return_node(expr);
    }


    
    if (t.type == TOKEN_IDENTIFIER && is_token(tokens[*index + 1], TOKEN_OPERATOR, "=")) {
        char var[64];
        strcpy(var, t.value);
        *index += 2;
        ASTNode* expr = parse_expression(tokens, index);
        if (!is_token(tokens[*index], TOKEN_SYMBOL, ";")) {
            fprintf(stderr, "Syntax Error: Expected ';' after assignment\n");
            exit(1);
        }
        (*index)++;
        return create_assignment_node(var, expr);
    }

    
    if (is_token(t, TOKEN_KEYWORD, "print") || is_token(t, TOKEN_KEYWORD, "printf")) {
        (*index)++;
        if (!is_token(tokens[*index], TOKEN_SYMBOL, "(")) {
            fprintf(stderr, "Syntax Error: Expected '(' after print\n");
            exit(1);
        }
        (*index)++;

        if (tokens[*index].type != TOKEN_STRING) {
            fprintf(stderr, "Syntax Error: Expected string in printf\n");
            exit(1);
        }

        char* format = strdup(tokens[*index].value);
        (*index)++;

        ASTNode** args = NULL;
        int arg_count = 0;

        while (is_token(tokens[*index], TOKEN_SYMBOL, ",")) {
            (*index)++;
            ASTNode* expr = parse_expression(tokens, index);
            args = realloc(args, sizeof(ASTNode*) * (arg_count + 1));
            args[arg_count++] = expr;
        }

        if (!is_token(tokens[*index], TOKEN_SYMBOL, ")")) {
            fprintf(stderr, "Syntax Error: Expected ')' after arguments\n");
            exit(1);
        }
        (*index)++;

        if (!is_token(tokens[*index], TOKEN_SYMBOL, ";")) {
            fprintf(stderr, "Syntax Error: Expected ';' after printf\n");
            exit(1);
        }
        (*index)++;

        return create_formatted_print_node(format, args, arg_count);
    }

    
    if (is_token(t, TOKEN_KEYWORD, "if")) {
        return parse_if_statement(tokens, index);
    }

  
    if (is_token(t, TOKEN_KEYWORD, "for")) {
        return parse_for_statement(tokens, index);
    }

    fprintf(stderr, "Syntax Error: Unknown statement starting with '%s'\n", t.value);
    exit(1);
}


ASTNode* parse_if_statement(Token* tokens, int* index) {
    (*index)++;
    if (!is_token(tokens[*index], TOKEN_SYMBOL, "(")) {
        fprintf(stderr, "Syntax Error: Expected '(' after 'if'\n");
        exit(1);
    }
    (*index)++;
    ASTNode* condition = parse_expression(tokens, index);
    if (!is_token(tokens[*index], TOKEN_SYMBOL, ")")) {
        fprintf(stderr, "Syntax Error: Expected ')' after condition\n");
        exit(1);
    }
    (*index)++;
    ASTNode* then_branch = parse_block(tokens, index);
    ASTNode* else_branch = NULL;

    if (is_token(tokens[*index], TOKEN_KEYWORD, "else")) {
        (*index)++;
        else_branch = parse_block(tokens, index);
    }

    return create_if_node(condition, then_branch, else_branch);
}


ASTNode* parse_for_statement(Token* tokens, int* index) {
    (*index)++;
    if (!is_token(tokens[*index], TOKEN_SYMBOL, "(")) {
        fprintf(stderr, "Syntax Error: Expected '(' after 'for'\n");
        exit(1);
    }
    (*index)++;

    ASTNode* init = NULL;
    if (!is_token(tokens[*index], TOKEN_SYMBOL, ";")) {
        init = parse_statement(tokens, index);
    } else {
        (*index)++;
    }

    ASTNode* cond = NULL;
    if (!is_token(tokens[*index], TOKEN_SYMBOL, ";")) {
        cond = parse_expression(tokens, index);
    }
    if (!is_token(tokens[*index], TOKEN_SYMBOL, ";")) {
        fprintf(stderr, "Syntax Error: Expected ';' after for condition\n");
        exit(1);
    }
    (*index)++;

    ASTNode* update = NULL;
    if (!is_token(tokens[*index], TOKEN_SYMBOL, ")")) {
        update = parse_expression(tokens, index);
    }
    if (!is_token(tokens[*index], TOKEN_SYMBOL, ")")) {
        fprintf(stderr, "Syntax Error: Expected ')' after for update\n");
        exit(1);
    }
    (*index)++;

    ASTNode* body = parse_block(tokens, index);
    return create_for_node(init, cond, update, body);
}


ASTNode* parse_block(Token* tokens, int* index) {
    if (!is_token(tokens[*index], TOKEN_SYMBOL, "{")) {
        fprintf(stderr, "Syntax Error: Expected '{' to start block\n");
        exit(1);
    }
    (*index)++;

    ASTNode** stmts = malloc(sizeof(ASTNode*) * MAX_STATEMENTS);
    int count = 0;

    while (!is_token(tokens[*index], TOKEN_SYMBOL, "}") &&
           tokens[*index].type != TOKEN_EOF) {
        ASTNode* stmt = parse_statement(tokens, index);
        if (stmt) stmts[count++] = stmt;
    }

    if (!is_token(tokens[*index], TOKEN_SYMBOL, "}")) {
        fprintf(stderr, "Syntax Error: Expected '}' to close block\n");
        exit(1);
    }
    (*index)++;
    return create_sequence_node(stmts, count);
}


ASTNode* parse(Token* tokens, int* index) {
    while (!is_token(tokens[*index], TOKEN_SYMBOL, "{") &&
           tokens[*index].type != TOKEN_EOF) {
        (*index)++;
    }

    if (tokens[*index].type == TOKEN_EOF) {
        fprintf(stderr, "Syntax Error: No main block found\n");
        exit(1);
    }

    return parse_block(tokens, index);
}
