#include <stdio.h>
#include<string.h>
#include "semantic.h"

const char *semanticTypeName(SemanticType type) {

    switch (type) {

        case SEM_NUMBER:
            return "NUMBER";

        case SEM_CELL:
            return "CELL";

        case SEM_RANGE:
            return "RANGE";

        case SEM_BOOLEAN:
            return "BOOLEAN";

        case SEM_ERROR:
            return "ERROR";

        default:
            return "UNKNOWN";
    }
}

int isScalarType(SemanticType type) {

    return (
        type == SEM_NUMBER ||
        type == SEM_CELL
    );
}

SemanticResult analyzeAST(ASTNode *node) {

    SemanticResult result;

    result.valid = 1;
    result.type = SEM_ERROR;


    if (node == NULL) {

        result.valid = 0;

        return result;
    }


    switch (node->type) {

        case AST_NUMBER:

            result.type = SEM_NUMBER;

            return result;


        case AST_CELL:

            result.type = SEM_CELL;

            return result;


        case AST_RANGE:

            if (node->left == NULL ||
                node->right == NULL) {

                result.valid = 0;

                return result;
            }


            if (node->left->type != AST_CELL ||
                node->right->type != AST_CELL) {

                result.valid = 0;

                return result;
            }


            result.type = SEM_RANGE;

            return result;


        case AST_BINARY_OP: {

            SemanticResult leftResult =
                analyzeAST(node->left);

            SemanticResult rightResult =
                analyzeAST(node->right);


            if (
                strcmp(node->value, "+") == 0 ||
                strcmp(node->value, "-") == 0 ||
                strcmp(node->value, "*") == 0 ||
                strcmp(node->value, "/") == 0
            ) {

                if (
                    !isScalarType(leftResult.type) ||
                    !isScalarType(rightResult.type)
                ) {

                    result.valid = 0;

                    return result;
                }

                result.type = SEM_NUMBER;

                return result;
            }
            result.valid = 0;

    return result;

        }

          


        case AST_FUNCTION: {

            if (node->children == NULL ||
                node->childCount <= 0) {

                result.valid = 0;

                return result;
            }


            for (int i = 0;
                 i < node->childCount;
                 i++) {

                SemanticResult argumentResult =
                    analyzeAST(node->children[i]);


                if (!argumentResult.valid) {

                    result.valid = 0;

                    return result;
                }
            }


            result.type = SEM_ERROR;

            return result;
        }


        default:

            result.valid = 0;

            return result;
    }
}