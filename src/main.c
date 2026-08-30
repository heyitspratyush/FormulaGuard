#include <stdio.h>

#include "lexer.h"
#include "parser.h"
#include "ast.h"


int main() {

    printf("FormulaGuard Compiler\n\n");


   // const char *formula = "=(A1+B1)*C1";
    //const char *formula = "=A1+B1*C1";
    const char *formula = "=SUM(A1,B1,C1)";

    Token tokens[100];


    int tokenCount = lex(formula, tokens);


    if (tokenCount == -1) {

        return 1;
    }


    printf("Tokens:\n\n");


    for (int i = 0; i < tokenCount; i++) {

        printf(
            "%-15s %s\n",
            tokenTypeName(tokens[i].type),
            tokens[i].text
        );
    }



    Parser parser;

    initParser(
        &parser,
        tokens,
        tokenCount
    );


  

    if (!expect(&parser, TOKEN_EQUAL)) {

        return 1;
    }


    
 

    ASTNode *root = parseExpression(&parser);


    if (root == NULL) {

        printf("\nParsing failed.\n");

        return 1;
    }



    printf("\nAST:\n\n");

    printAST(root, 0);


    return 0;
}