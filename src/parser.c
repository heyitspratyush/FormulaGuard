#include <stdio.h>

#include "parser.h"
#include "ast.h"


void initParser(
    Parser *parser,
    Token tokens[],
    int tokenCount
) {

    parser->tokens = tokens;

    parser->current = 0;

    parser->tokenCount = tokenCount;
}


Token *peek(Parser *parser) {

    if (parser->current >= parser->tokenCount) {
        return NULL;
    }

    return &parser->tokens[parser->current];
}


Token *advance(Parser *parser) {

    if (parser->current >= parser->tokenCount) {
        return NULL;
    }

    return &parser->tokens[parser->current++];
}


int expect(
    Parser *parser,
    TokenType type
) {

    Token *token = peek(parser);

    if (token == NULL) {

        printf("Parser Error: unexpected end of input\n");

        return 0;
    }


    if (token->type != type) {

        printf(
            "Parser Error: expected %s but found %s\n",
            tokenTypeName(type),
            tokenTypeName(token->type)
        );

        return 0;
    }


    advance(parser);

    return 1;
}




ASTNode *parsePrimary(Parser *parser) {

    Token *token = peek(parser);


    if (token == NULL) {
        return NULL;
    }


    if (token->type == TOKEN_CELL) {

        advance(parser);

        return createCellNode(token->text);
    } 

    if (token->type == TOKEN_NUMBER) {

        advance(parser);

        return createNumberNode(token->text);
    }

    if (token->type == TOKEN_LPAREN) {

        advance(parser);


        ASTNode *node = parseExpression(parser);


        if (node == NULL) {

            return NULL;
        }


        if (!expect(parser, TOKEN_RPAREN)) {

            return NULL;
        }


        return node;
    }


    return NULL;
}




ASTNode *parseTerm(Parser *parser) {

    ASTNode *left = parsePrimary(parser);


    if (left == NULL) {
        return NULL;
    }


    while (
        peek(parser) != NULL &&
        (peek(parser)->type == TOKEN_MULTIPLY ||
         peek(parser)->type == TOKEN_DIVIDE)
    ) {

        Token *operator = advance(parser);


        ASTNode *right = parsePrimary(parser);


        if (right == NULL) {

            printf(
                "Parser Error: expected value after %s\n",
                operator->text
            );

            return NULL;
        }


        left = createBinaryOpNode(
            operator->text,
            left,
            right
        );


        if (left == NULL) {
            return NULL;
        }
    }


    return left;
}




ASTNode *parseExpression(Parser *parser) {

    ASTNode *left = parseTerm(parser);


    if (left == NULL) {
        return NULL;
    }


    while (
        peek(parser) != NULL &&
        (peek(parser)->type == TOKEN_PLUS ||
         peek(parser)->type == TOKEN_MINUS)
    ) {

        Token *operator = advance(parser);


        ASTNode *right = parseTerm(parser);


        if (right == NULL) {

            printf(
                "Parser Error: expected value after %s\n",
                operator->text
            );

            return NULL;
        }


        left = createBinaryOpNode(
            operator->text,
            left,
            right
        );


        if (left == NULL) {
            return NULL;
        }
    }


    return left;
}