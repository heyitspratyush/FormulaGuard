#include <stdio.h>

#include "lexer.h"
#include "parser.h"


int main() {

    printf("FormulaGuard Compiler\n\n");


    const char *formula = "=A1";


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


    Parser parser;

    initParser(&parser, tokens, tokenCount);


    
    expect(&parser, TOKEN_EQUAL);


    
    Token *result = parsePrimary(&parser);


    if (result != NULL) {

        printf("\nParsed token: %s (%s)\n",
               result->text,
               tokenTypeName(result->type));
    }


    return 0;
}