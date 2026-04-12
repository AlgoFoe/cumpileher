#include <stdio.h>
#include "lexer.h"
#include "parser.h"

int main() {
    char input[] = "int y = 15;"
                   "int x = 90;"
                   "int z = y + x;"
                   "print(z);";

    init_lexer(input);
    parse();

    return 0;
}