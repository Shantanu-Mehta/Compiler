#ifndef INTERPRETER_H
#define INTERPRETER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ast.h"

/**
 * @file interpreter.h
 * @brief Interfaces for evaluating AST and generating Three-Address Code (TAC).
 */

/**
 * @brief Counter for generating unique temporary variable names in TAC.
 * Declared extern here; defined in interpreter.c.
 */
extern int temp_count;

/**
 * @brief Counter for generating unique labels for branching in TAC.
 * Declared extern here; defined in interpreter.c.
 */
extern int label_count;

/**
 * @brief Evaluates the given AST and returns the computed result.
 *
 * Supports:
 *  - Arithmetic expressions (+, -, *, /, %)
 *  - Comparisons (<, >, <=, >=, ==, !=)
 *  - Variable declarations and assignments
 *  - Print and formatted print
 *  - If-else statements
 *  - While and for loops
 *  - Return statements
 *  - Sequences of statements
 *
 * @param node Root of the AST to evaluate.
 * @return Result of evaluation (typically used for expression or return value).
 */
int eval(const ASTNode* node);

/**
 * @brief Generates and prints Three-Address Code (TAC) for the given AST.
 *
 * Recursively traverses the AST and emits TAC-style instructions.
 * This includes:
 *  - Arithmetic operations
 *  - Variable assignments
 *  - Print statements
 *  - Return statements
 *
 * @param node Root of the AST.
 * @return Dynamically allocated string with temporary variable name
 *         holding the result (or NULL if not applicable). Must be freed.
 */
char* generate_tac(const ASTNode* node);

#ifdef __cplusplus
}
#endif

#endif 
