#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"

static const char *src;
static int pos = 0;

void init_lexer(const char *input) {
    src = input;
    pos = 0;
}

Token get_next_token() {
    Token token;
    
    while (src[pos] == ' ' || src[pos] == '\n' || src[pos] == '\t') pos++;

    if (src[pos] == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }

    // int keyword
    if (strncmp(&src[pos], "int", 3) == 0 && !isalnum(src[pos+3])) {
        pos += 3;
        token.type = TOKEN_INT;
        strcpy(token.value, "int");
        return token;
    }

    if (strncmp(&src[pos], "print", 5) == 0 && !isalnum(src[pos+5])) {
        pos += 5;
        token.type = TOKEN_PRINT;
        return token;
    }

    // identifier
    if (isalpha(src[pos])) {
        int i = 0;
        while (isalnum(src[pos])) {
            token.value[i++] = src[pos++];
        }
        token.value[i] = '\0';
        token.type = TOKEN_ID;
        return token;
    }

    // number
    if (isdigit(src[pos])) {
        int i = 0;
        while (isdigit(src[pos])) {
            token.value[i++] = src[pos++];
        }
        token.value[i] = '\0';
        token.type = TOKEN_NUM;
        return token;
    }

    if (src[pos] == '=') {
        pos++;
        token.type = TOKEN_ASSIGN;
        strcpy(token.value, "=");
        return token;
    }

    if (src[pos] == '+') {
        pos++;
        token.type = TOKEN_PLUS;
        strcpy(token.value, "+");
        return token;
    }

    // '('
    if (src[pos] == '(') {
        pos++;
        token.type = TOKEN_LPAREN;
        return token;
    }

    // ')'
    if (src[pos] == ')') {
        pos++;
        token.type = TOKEN_RPAREN;
        return token;
    }

    if (src[pos] == ';') {
        pos++;
        token.type = TOKEN_SEMI;
        strcpy(token.value, ";");
        return token;
    }

    pos++;
    return get_next_token();
}