#include <stdio.h>

#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "semantic.h"


int main() {

    printf("FormulaGuard Compiler\n\n");
   
   const char *formula = "=IF(A1>10,SUM(B1:C5),D1)";

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

    ASTNode *root = parseComparison(&parser);

    if (root == NULL) {
        printf("\nParsing failed.\n");
        return 1;
    }
    printf("\nAST:\n\n");
    printAST(root, 0);
    printf("\nSemantic Analysis:\n\n");

    if (analyzeAST(root)) {
        printf("Semantic analysis successful.\n");
    }
    else {
        printf("Semantic error detected.\n");
    }

    freeAST(root);

    return 0;
}