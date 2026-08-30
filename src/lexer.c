#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "lexer.h"


const char *tokenTypeName(TokenType type) {

    switch (type) {

        case TOKEN_EQUAL:
            return "EQUAL";

        case TOKEN_CELL:
            return "CELL";

        case TOKEN_NUMBER:
            return "NUMBER";

        case TOKEN_PLUS:
            return "PLUS";

        case TOKEN_MINUS:
            return "MINUS";

        case TOKEN_MULTIPLY:
            return "MULTIPLY";

        case TOKEN_DIVIDE:
            return "DIVIDE";

        case TOKEN_FUNCTION:
            return "FUNCTION";

        case TOKEN_LPAREN:
            return "LPAREN";

        case TOKEN_RPAREN:
            return "RPAREN";

        case TOKEN_COLON:
            return "COLON";

        case TOKEN_LESS:
            return "LESS";

        case TOKEN_GREATER:
            return "GREATER";

        case TOKEN_LESS_EQUAL:
            return "LESS_EQUAL";

        case TOKEN_GREATER_EQUAL:
            return "GREATER_EQUAL";

        case TOKEN_NOT_EQUAL:
            return "NOT_EQUAL";

        case TOKEN_COMMA:
            return "COMMA";

        case TOKEN_EOF:
            return "EOF";

        default:
            return "UNKNOWN";
    }
}


void addToken(Token tokens[], int *tokenCount,
              TokenType type, const char *text) {

    tokens[*tokenCount].type = type;

    strcpy(tokens[*tokenCount].text, text);

    (*tokenCount)++;
}


int lex(const char *formula, Token tokens[]) {

    int tokenCount = 0;


    while (*formula != '\0') {

        /* Ignore whitespace */
        if (isspace(*formula)) {

            formula++;
        }


        /* Equal */
        else if (*formula == '=') {

            addToken(tokens, &tokenCount,
                     TOKEN_EQUAL, "=");

            formula++;
        }


        /* Cell or function */
        else if (isalpha(*formula)) {

            char text[50];
            int i = 0;


            while (isalpha(*formula)) {

                text[i] = *formula;
                i++;
                formula++;
            }


            /* Cell: A1, B20, AA100 */
            if (isdigit(*formula)) {

                while (isdigit(*formula)) {

                    text[i] = *formula;
                    i++;
                    formula++;
                }

                text[i] = '\0';

                addToken(tokens, &tokenCount,
                         TOKEN_CELL, text);
            }


            /* Function: SUM(, AVERAGE(, etc. */
            else if (*formula == '(') {

                text[i] = '\0';

                addToken(tokens, &tokenCount,
                         TOKEN_FUNCTION, text);
            }


            else {

                printf("Lexer Error: invalid identifier\n");

                return -1;
            }
        }


        /* Number */
        else if (isdigit(*formula)) {

            char text[50];
            int i = 0;


            while (isdigit(*formula)) {

                text[i] = *formula;
                i++;
                formula++;
            }


            /* Decimal number */
            if (*formula == '.') {

                text[i] = '.';
                i++;
                formula++;


                while (isdigit(*formula)) {

                    text[i] = *formula;
                    i++;
                    formula++;
                }
            }


            text[i] = '\0';

            addToken(tokens, &tokenCount,
                     TOKEN_NUMBER, text);
        }


        /* Plus */
        else if (*formula == '+') {

            addToken(tokens, &tokenCount,
                     TOKEN_PLUS, "+");

            formula++;
        }


        /* Minus */
        else if (*formula == '-') {

            addToken(tokens, &tokenCount,
                     TOKEN_MINUS, "-");

            formula++;
        }


        /* Multiply */
        else if (*formula == '*') {

            addToken(tokens, &tokenCount,
                     TOKEN_MULTIPLY, "*");

            formula++;
        }


        /* Divide */
        else if (*formula == '/') {

            addToken(tokens, &tokenCount,
                     TOKEN_DIVIDE, "/");

            formula++;
        }


        /* Left parenthesis */
        else if (*formula == '(') {

            addToken(tokens, &tokenCount,
                     TOKEN_LPAREN, "(");

            formula++;
        }


        /* Right parenthesis */
        else if (*formula == ')') {

            addToken(tokens, &tokenCount,
                     TOKEN_RPAREN, ")");

            formula++;
        }


        /* Colon */
        else if (*formula == ':') {

            addToken(tokens, &tokenCount,
                     TOKEN_COLON, ":");

            formula++;
        }

        /* Comma */

        else if (*formula == ',') {

            addToken(tokens, &tokenCount,
                    TOKEN_COMMA, ",");

            formula++;
        }


        /* Less than / less than or equal / not equal */
        else if (*formula == '<') {

            if (*(formula + 1) == '=') {

                addToken(tokens, &tokenCount,
                         TOKEN_LESS_EQUAL, "<=");

                formula += 2;
            }

            else if (*(formula + 1) == '>') {

                addToken(tokens, &tokenCount,
                         TOKEN_NOT_EQUAL, "<>");

                formula += 2;
            }

            else {

                addToken(tokens, &tokenCount,
                         TOKEN_LESS, "<");

                formula++;
            }
        }


        /* Greater than / greater than or equal */
        else if (*formula == '>') {

            if (*(formula + 1) == '=') {

                addToken(tokens, &tokenCount,
                         TOKEN_GREATER_EQUAL, ">=");

                formula += 2;
            }

            else {

                addToken(tokens, &tokenCount,
                         TOKEN_GREATER, ">");

                formula++;
            }
        }


        /* Invalid character */
        else {

            printf("Lexer Error: unexpected character '%c'\n",
                   *formula);

            return -1;
        }
    }
    addToken(tokens, &tokenCount,
         TOKEN_EOF, "");

    return tokenCount;
}