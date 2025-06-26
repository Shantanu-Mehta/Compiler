#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"
#include "ast.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Parses the array of tokens starting from the given index.
 * Skips prelude code (e.g., #include, int main()) and begins parsing
 * from the first '{' block.
 *
 * @param tokens Array of tokens produced by the tokenizer.
 * @param index Pointer to the current token index (will be updated).
 * @return Pointer to the root ASTNode of the parsed program.
 */
ASTNode* parse(Token* tokens, int* index);

// --- Optional for testing or REPL use ---
/**
 * Parses a single expression from tokens.
 * Useful for expression evaluation or REPL.
 */
ASTNode* parse_expression(Token* tokens, int* index);

/**
 * Parses a single statement (declaration, assignment, control flow).
 */
ASTNode* parse_statement(Token* tokens, int* index);

/**
 * Parses a block enclosed in braces { ... } and returns a sequence node.
 */
ASTNode* parse_block(Token* tokens, int* index);

#ifdef __cplusplus
}
#endif

#endif 
