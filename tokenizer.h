#ifndef TOKENIZER_H
#define TOKENIZER_H

#define MAX_TOKENS 2048
#define MAX_TOKEN_LENGTH 64


typedef enum {
    TOKEN_NUMBER,         
    TOKEN_IDENTIFIER,     
    TOKEN_KEYWORD,        
    TOKEN_OPERATOR,      
    TOKEN_SYMBOL,         
    TOKEN_STRING,         
    TOKEN_PREPROCESSOR,   
    TOKEN_EOF             
} TokenType;


typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LENGTH];
} Token;


Token* tokenize(const char* input, int* token_count);
const char* token_type_to_string(int type);
void print_tokens(Token* tokens, int count);

#endif 
