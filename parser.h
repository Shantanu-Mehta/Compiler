#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"
#include "ast.h"

ASTNode* parse(Token* tokens, int* index);

#endif
