#include <stdio.h>

#include "lexer.h"


int main() {

    printf("FormulaGuard Compiler\n\n");


    const char *formula = "=SUM(A1:B10)>=100";


    Token tokens[100];


    int tokenCount = lex(formula, tokens);


    if (tokenCount == -1) {
        return 1;
    }


    printf("Tokens:\n\n");


    for (int i = 0; i < tokenCount; i++) {

        printf("%-15s %s\n",
               tokenTypeName(tokens[i].type),
               tokens[i].text);
    }


    return 0;
}