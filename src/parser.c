#include <stdio.h>

#include "parser.h"


void initParser(Parser *parser,
                Token tokens[],
                int tokenCount) {

    parser->tokens = tokens;
    parser->tokenCount = tokenCount;
    parser->current = 0;
}


Token *peek(Parser *parser) {

    return &parser->tokens[parser->current];
}


Token *advance(Parser *parser) {

    if (parser->current < parser->tokenCount) {

        parser->current++;
    }

    return &parser->tokens[parser->current - 1];
}


int match(Parser *parser, TokenType type) {

    if (peek(parser)->type == type) {

        advance(parser);

        return 1;
    }

    return 0;
}


void expect(Parser *parser, TokenType type) {

    if (peek(parser)->type != type) {

        printf("Parser Error: expected %s, found %s\n",
               tokenTypeName(type),
               tokenTypeName(peek(parser)->type));

        return;
    }

    advance(parser);
}


Token *parsePrimary(Parser *parser) {

    if (peek(parser)->type == TOKEN_CELL) {

        return advance(parser);
    }


    if (peek(parser)->type == TOKEN_NUMBER) {

        return advance(parser);
    }


    printf("Parser Error: expected CELL or NUMBER\n");

    return NULL;
}