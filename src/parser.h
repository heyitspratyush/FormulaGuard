#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"


typedef struct {

    Token *tokens;
    int tokenCount;
    int current;

} Parser;


void initParser(Parser *parser,
                Token tokens[],
                int tokenCount);

Token *peek(Parser *parser);

Token *advance(Parser *parser);

int match(Parser *parser, TokenType type);

void expect(Parser *parser, TokenType type);

Token *parsePrimary(Parser *parser);


#endif