#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file symbol_table.h
 * @brief Scoped symbol table interface for variable declaration, assignment, retrieval, and block scope management.
 */

/**
 * @brief Enters a new block scope.
 *        Should be called when a new `{}` block starts.
 */
void enter_scope();

/**
 * @brief Exits the current block scope.
 *        Should be called when a block ends (`}`).
 */
void exit_scope();

/**
 * @brief Declares a variable in the current scope.
 *        Throws an error if already declared in the same scope.
 *
 * @param name The name of the variable to declare.
 */
void declare_variable(const char* name);

/**
 * @brief Assigns a value to a declared variable.
 *        Searches from innermost scope to outermost.
 *
 * @param name The name of the variable.
 * @param val  The integer value to assign.
 */
void set_variable(const char* name, int val);

/**
 * @brief Retrieves the value of a declared variable.
 *        Searches from innermost to outermost scope.
 *
 * @param name The name of the variable.
 * @return The current value of the variable.
 */
int get_variable(const char* name);

/**
 * @brief Resets the entire symbol table (all scopes).
 *        Typically used between compilations or test cases.
 */
void reset_symbol_table();

/**
 * @brief Debug helper to print all variables in all scopes.
 */
void print_symbol_table();

#ifdef __cplusplus
}
#endif

#endif // SYMBOL_TABLE_H
