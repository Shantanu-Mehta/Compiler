#ifndef AST_H
#define AST_H

#ifdef __cplusplus
extern "C" {
#endif
typedef struct ASTNode ASTNode;


typedef enum {
    AST_NUMBER,
    AST_VARIABLE,
    AST_BINARY_OP,
    AST_ASSIGNMENT,
    AST_PRINT,
    AST_FORMATTED_PRINT,
    AST_SEQUENCE,
    AST_IF,
    AST_WHILE,
    AST_FOR,
    AST_RETURN,
    AST_STRING,
    AST_FUNCTION,
    AST_DECLARATION,
    AST_DECLARATION_WITH_INIT
      
} ASTNodeType;


typedef struct ASTNode {
    ASTNodeType type;
    union {
        int number;

        struct {
            char name[64];
        } variable;

        struct {
            char value[256];
        } string;

        struct {
            char op[3];
            struct ASTNode* left;
            struct ASTNode* right;
        } binary;

        struct {
            char var[64];
            struct ASTNode* value;
        } assignment;

        struct ASTNode* print_expr;

        struct {
            char format[256];
            struct ASTNode** args;   
            int arg_count;          
        } formatted_print;

        struct {
            struct ASTNode** statements;
            int count;
        } sequence;

        struct {
            struct ASTNode* condition;
            struct ASTNode* then_branch;
            struct ASTNode* else_branch;
        } if_stmt;

        struct {
            struct ASTNode* condition;
            struct ASTNode* body;
        } while_stmt;
        struct {
             ASTNode* value;
             } return_stmt;

        struct {
            struct ASTNode* init;
            struct ASTNode* cond;
            struct ASTNode* update;
            struct ASTNode* body;
        } for_stmt;

      struct {
    char name[64];
    struct ASTNode* initial_value;  
} declaration;


        struct {                  
            char name[64];
            struct ASTNode* body;
        } function;

    } data;
} ASTNode;


ASTNode* create_number_node(int value);
ASTNode* create_variable_node(const char* name);
ASTNode* create_binary_op_node(const char* op, ASTNode* left, ASTNode* right);
ASTNode* create_assignment_node(const char* var, ASTNode* value);
ASTNode* create_print_node(ASTNode* expr);
ASTNode* create_formatted_print_node(const char* format, ASTNode** args, int arg_count);
ASTNode* create_sequence_node(ASTNode** statements, int count);
ASTNode* create_if_node(ASTNode* condition, ASTNode* then_branch, ASTNode* else_branch);
ASTNode* create_while_node(ASTNode* condition, ASTNode* body);
ASTNode* create_for_node(ASTNode* init, ASTNode* cond, ASTNode* update, ASTNode* body);
ASTNode* create_string_node(const char* str);
ASTNode* create_declaration_node(const char* var_name, ASTNode* initial_value);
ASTNode* create_function_node(const char* name, ASTNode* body);  // Added function constructor
ASTNode* create_declaration_with_init_node(const char* var_name, ASTNode* value);
ASTNode* create_return_node(ASTNode* value);




void print_ast(ASTNode* node, int level);


void free_ast(ASTNode* node);

#ifdef __cplusplus
}
#endif

#endif 
