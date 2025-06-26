#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "tokenizer.h"

void evaluate_tokens(Token* tokens, int token_count);

int evaluate_expression(Token* tokens, int* index);


int evaluate_condition(Token* tokens, int* index);

#endif 
