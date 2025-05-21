#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "tokenizer.h"

int evaluate_expression(Token* tokens, int* index);
int evaluate_condition(Token* tokens, int* index);
void evaluate_tokens(Token* tokens, int token_count);

#endif
