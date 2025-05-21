
#ifndef AST_H
#define AST_H

typedef enum {
    AST_NUMBER,
    AST_VARIABLE,
    AST_BINARY_OP,
    AST_ASSIGNMENT,
    AST_PRINT,
    AST_SEQUENCE
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    union {
        int number;
        char var_name[64];
        struct {
            char op;
            struct ASTNode* left;
            struct ASTNode* right;
        } binary;
        struct {
            char var[64];
            struct ASTNode* value;
        } assignment;
        struct ASTNode* print_expr;
        struct {
            struct ASTNode** statements;
            int count;
        } sequence;
    } data;
} ASTNode;

ASTNode* create_number_node(int value);
ASTNode* create_variable_node(const char* name);
ASTNode* create_binary_op_node(char op, ASTNode* left, ASTNode* right);
ASTNode* create_assignment_node(const char* var, ASTNode* value);
ASTNode* create_print_node(ASTNode* expr);
ASTNode* create_sequence_node(ASTNode** statements, int count);

// 🔽 Add this declaration
void print_ast(ASTNode* node, int level);

#endif
