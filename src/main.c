#include <stdio.h>
#include <ctype.h>
#include <string.h>

typedef enum {
    TOKEN_EQUAL,
    TOKEN_CELL,
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_FUNCTION,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_COLON,
    TOKEN_LESS,
    TOKEN_GREATER,
    TOKEN_LESS_EQUAL,
    TOKEN_GREATER_EQUAL,
    TOKEN_NOT_EQUAL
} TokenType;

struct Token {
    TokenType type;
    char text[50];
};

const char *tokenTypeName(TokenType type) {

    switch (type) {
        case TOKEN_EQUAL: return "EQUAL";
        case TOKEN_CELL: return "CELL";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_MULTIPLY: return "MULTIPLY";
        case TOKEN_DIVIDE: return "DIVIDE";
        case TOKEN_FUNCTION: return "FUNCTION";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_COLON: return "COLON";
        case TOKEN_LESS: return "LESS";
        case TOKEN_GREATER: return "GREATER";
        case TOKEN_LESS_EQUAL: return "LESS_EQUAL";
        case TOKEN_GREATER_EQUAL: return "GREATER_EQUAL";
        case TOKEN_NOT_EQUAL: return "NOT_EQUAL";
        default: return "UNKNOWN";
    }
}

int main() {

    printf("FormulaGuard Compiler\n\n");

    const char *formula = "=SUM(A1:B10)>=100";

    struct Token tokens[100];
    int tokenCount = 0;

    while (*formula != '\0') {

        if (isspace(*formula)) {
            formula++;
        }

        else if (*formula == '=') {

            tokens[tokenCount].type = TOKEN_EQUAL;
            tokens[tokenCount].text[0] = '=';
            tokens[tokenCount].text[1] = '\0';

            tokenCount++;
            formula++;
        }

        else if (isalpha(*formula)) {

            int i = 0;

            while (isalpha(*formula)) {
                tokens[tokenCount].text[i] = *formula;
                i++;
                formula++;
            }

            if (isdigit(*formula)) {

                while (isdigit(*formula)) {
                    tokens[tokenCount].text[i] = *formula;
                    i++;
                    formula++;
                }

                tokens[tokenCount].text[i] = '\0';
                tokens[tokenCount].type = TOKEN_CELL;

                tokenCount++;
            }

            else if (*formula == '(') {

                tokens[tokenCount].text[i] = '\0';
                tokens[tokenCount].type = TOKEN_FUNCTION;

                tokenCount++;
            }

            else {
                printf("Lexer Error: invalid identifier\n");
                return 1;
            }
        }

        else if (isdigit(*formula)) {

            int i = 0;

            while (isdigit(*formula)) {
                tokens[tokenCount].text[i] = *formula;
                i++;
                formula++;
            }

            if (*formula == '.') {

                tokens[tokenCount].text[i] = '.';
                i++;
                formula++;

                while (isdigit(*formula)) {
                    tokens[tokenCount].text[i] = *formula;
                    i++;
                    formula++;
                }
            }

            tokens[tokenCount].text[i] = '\0';
            tokens[tokenCount].type = TOKEN_NUMBER;

            tokenCount++;
        }

        else if (*formula == '+') {

            tokens[tokenCount].type = TOKEN_PLUS;
            tokens[tokenCount].text[0] = '+';
            tokens[tokenCount].text[1] = '\0';

            tokenCount++;
            formula++;
        }

        else if (*formula == '-') {

            tokens[tokenCount].type = TOKEN_MINUS;
            tokens[tokenCount].text[0] = '-';
            tokens[tokenCount].text[1] = '\0';

            tokenCount++;
            formula++;
        }

        else if (*formula == '*') {

            tokens[tokenCount].type = TOKEN_MULTIPLY;
            tokens[tokenCount].text[0] = '*';
            tokens[tokenCount].text[1] = '\0';

            tokenCount++;
            formula++;
        }

        else if (*formula == '/') {

            tokens[tokenCount].type = TOKEN_DIVIDE;
            tokens[tokenCount].text[0] = '/';
            tokens[tokenCount].text[1] = '\0';

            tokenCount++;
            formula++;
        }

        else if (*formula == '(') {

            tokens[tokenCount].type = TOKEN_LPAREN;
            tokens[tokenCount].text[0] = '(';
            tokens[tokenCount].text[1] = '\0';

            tokenCount++;
            formula++;
        }

        else if (*formula == ')') {

            tokens[tokenCount].type = TOKEN_RPAREN;
            tokens[tokenCount].text[0] = ')';
            tokens[tokenCount].text[1] = '\0';

            tokenCount++;
            formula++;
        }

        else if (*formula == ':') {

            tokens[tokenCount].type = TOKEN_COLON;
            tokens[tokenCount].text[0] = ':';
            tokens[tokenCount].text[1] = '\0';

            tokenCount++;
            formula++;
        }

        else if (*formula == '<') {

            if (*(formula + 1) == '=') {

                tokens[tokenCount].type = TOKEN_LESS_EQUAL;
                tokens[tokenCount].text[0] = '<';
                tokens[tokenCount].text[1] = '=';
                tokens[tokenCount].text[2] = '\0';

                formula += 2;
            }

            else if (*(formula + 1) == '>') {

                tokens[tokenCount].type = TOKEN_NOT_EQUAL;
                tokens[tokenCount].text[0] = '<';
                tokens[tokenCount].text[1] = '>';
                tokens[tokenCount].text[2] = '\0';

                formula += 2;
            }

            else {

                tokens[tokenCount].type = TOKEN_LESS;
                tokens[tokenCount].text[0] = '<';
                tokens[tokenCount].text[1] = '\0';

                formula++;
            }

            tokenCount++;
        }

        else if (*formula == '>') {

            if (*(formula + 1) == '=') {

                tokens[tokenCount].type = TOKEN_GREATER_EQUAL;
                tokens[tokenCount].text[0] = '>';
                tokens[tokenCount].text[1] = '=';
                tokens[tokenCount].text[2] = '\0';

                formula += 2;
            }

            else {

                tokens[tokenCount].type = TOKEN_GREATER;
                tokens[tokenCount].text[0] = '>';
                tokens[tokenCount].text[1] = '\0';

                formula++;
            }

            tokenCount++;
        }

        else {
            printf("Lexer Error: unexpected character '%c'\n", *formula);
            return 1;
        }
    }

    printf("Tokens:\n\n");

    for (int i = 0; i < tokenCount; i++) {
        printf("%-15s %s\n",
               tokenTypeName(tokens[i].type),
               tokens[i].text);
    }

    return 0;
}