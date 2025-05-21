#ifndef TOKENIZER_H
#define TOKENIZER_H

#define MAX_TOKENS 1024

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_KEYWORD,
    TOKEN_OPERATOR,
    TOKEN_SYMBOL,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[64];
} Token;

Token* tokenize(const char* input, int* token_count);

const char* token_type_to_string(int type);


#endif
