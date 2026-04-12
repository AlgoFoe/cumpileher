#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_INT,
    TOKEN_ID,
    TOKEN_NUM,
    TOKEN_ASSIGN,
    TOKEN_PLUS,
    TOKEN_PRINT,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_SEMI,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[50];
} Token;

void init_lexer(const char *input);
Token get_next_token();

#endif