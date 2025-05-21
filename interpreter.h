#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"

int eval(ASTNode* node);
char* generate_tac(ASTNode* node);

#endif
