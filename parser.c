#include <stdio.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "codegen.h"
#include "symbol_table.h"

static Token current_token;

void eat(TokenType type) {
    if (current_token.type == type) {
        current_token = get_next_token();
    } else {
        printf("Syntax Error\n");
        current_token = get_next_token();
    }
}

// expression --> ID + ID | NUM | ID
void parse_expression(char *out) {
    char left[50];

    // left operand
    if (current_token.type == TOKEN_ID || current_token.type == TOKEN_NUM) {
        strcpy(left, current_token.value);
        current_token = get_next_token();
    } else {
        printf("Invalid expression\n");
        return;
    }

    // no operator → simple assignment
    if (current_token.type != TOKEN_PLUS) {
        strcpy(out, left);
        return;
    }

    // handle +
    eat(TOKEN_PLUS);

    char right[50];
    if (current_token.type == TOKEN_ID || current_token.type == TOKEN_NUM) {
        strcpy(right, current_token.value);
        current_token = get_next_token();
    } else {
        printf("Invalid expression\n");
        return;
    }

    static int temp_id = 1;
    sprintf(out, "t%d", temp_id++);

    gen_add(out, left, right);
}

// statement --> int ID = expression ;
void parse_statement() {
    eat(TOKEN_INT);

    char var[50];
    strcpy(var, current_token.value);
    eat(TOKEN_ID);

    add_symbol(var);
    declare_variable(var);

    eat(TOKEN_ASSIGN);

    char result[50];
    parse_expression(result);

    if (strcmp(result, var) != 0) {
        gen_mov(var, result);
    }

    eat(TOKEN_SEMI);
}

void parse_print() {
    eat(TOKEN_PRINT);
    eat(TOKEN_LPAREN);

    char var[50];
    strcpy(var, current_token.value);
    eat(TOKEN_ID);

    eat(TOKEN_RPAREN);
    eat(TOKEN_SEMI);

    gen_print(var);
}

// program --> multiple statements
void parse() {
    init_codegen();

    current_token = get_next_token();

    while (current_token.type != TOKEN_EOF) {
        if (current_token.type == TOKEN_INT) {
            parse_statement();
        } 
        else if (current_token.type == TOKEN_PRINT) {
            parse_print();
        } 
        else {
            printf("Unknown statement\n");
            current_token = get_next_token();
        }
    }

    finish_codegen();
}