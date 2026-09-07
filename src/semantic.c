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
int isValidCellReference(const char *value) {

    if (value == NULL || value[0] == '\0') {
        return 0;
    }

    int i = 0;

    /*
     * Column portion:
     * one or more letters.
     */
    while (value[i] != '\0' &&
           (
               (value[i] >= 'A' && value[i] <= 'Z') ||
               (value[i] >= 'a' && value[i] <= 'z')
           )) {

        i++;
    }

    /*
     * At least one column letter
     * must exist.
     */
    if (i == 0) {
        return 0;
    }

    /*
     * Row portion:
     * one or more digits.
     */
    int rowStart = i;

    while (value[i] != '\0' &&
           value[i] >= '0' &&
           value[i] <= '9') {

        i++;
    }

    /*
     * There must be at least one digit.
     */
    if (i == rowStart) {
        return 0;
    }

    /*
     * Nothing other than column letters
     * and row digits is allowed.
     */
    if (value[i] != '\0') {
        return 0;
    }

    /*
     * Check that the row is not zero.
     */
    int row = 0;

    for (int j = rowStart; j < i; j++) {

        row = row * 10 + (value[j] - '0');

        if (row > 0) {
            break;
        }
    }

    if (row == 0) {
        return 0;
    }

    return 1;
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
    
            if (!isValidCellReference(node->value)) {
                result.valid = 0;
                return result;
            }

            result.type = SEM_CELL;

            return result;


        case AST_RANGE: {

            /*
            * A range must have both
            * a start and an end cell.
            */
            if (node->left == NULL ||
                node->right == NULL) {

                result.valid = 0;

                return result;
            }


            /*
            * Both endpoints must be
            * AST_CELL nodes.
            */
            if (node->left->type != AST_CELL ||
                node->right->type != AST_CELL) {

                result.valid = 0;

                return result;
            }


            /*
            * Run semantic analysis on
            * both cell references.
            *
            * This reuses isValidCellReference()
            * through the AST_CELL case.
            */
            SemanticResult startResult =
                analyzeAST(node->left);

            SemanticResult endResult =
                analyzeAST(node->right);


            /*
            * If either endpoint is invalid,
            * the entire range is invalid.
            */
            if (!startResult.valid ||
                !endResult.valid) {

                result.valid = 0;

                return result;
            }


            /*
            * Both endpoints are valid cells,
            * therefore the range is valid.
            */
            result.type = SEM_RANGE;

            return result;
        }


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
            if (
                strcmp(node->value, "<") == 0 ||
                strcmp(node->value, ">") == 0 ||
                strcmp(node->value, "<=") == 0 ||
                strcmp(node->value, ">=") == 0 ||
                strcmp(node->value, "<>") == 0
            ) {

                if (
                    !isScalarType(leftResult.type) ||
                    !isScalarType(rightResult.type)
                ) {

                    result.valid = 0;

                    return result;
                }


                result.type = SEM_BOOLEAN;

                return result;
            }
            result.valid = 0;

            return result;

        }

          


        case AST_FUNCTION: {

            /*
            * A function must have at least
            * one argument.
            */
            if (node->children == NULL ||
                node->childCount <= 0) {

                result.valid = 0;

                return result;
            }


            /*
            * Check whether this is the
            * supported SUM function.
            */
            if (strcmp(node->value, "SUM") == 0) {

                /*
                * Analyze every SUM argument.
                */
                for (int i = 0;
                    i < node->childCount;
                    i++) {

                    SemanticResult argumentResult =
                        analyzeAST(node->children[i]);


                    /*
                    * An invalid argument makes
                    * the entire SUM invalid.
                    */
                    if (!argumentResult.valid) {

                        result.valid = 0;

                        return result;
                    }


                    /*
                    * SUM accepts NUMBER, CELL,
                    * or RANGE arguments.
                    */
                    if (
                        argumentResult.type != SEM_NUMBER &&
                        argumentResult.type != SEM_CELL &&
                        argumentResult.type != SEM_RANGE
                    ) {

                        result.valid = 0;

                        return result;
                    }
                }


                /*
                * SUM always produces a NUMBER.
                */
                result.type = SEM_NUMBER;

                return result;
            }
            if (strcmp(node->value, "IF") == 0) {

                /*
                * IF requires exactly
                * three arguments:
                *
                * IF(condition, true_value, false_value)
                */
                if (node->childCount != 3) {

                    result.valid = 0;

                    return result;
                }


                /*
                * Analyze the condition.
                */
                SemanticResult conditionResult =
                    analyzeAST(node->children[0]);

                if (!conditionResult.valid) {

                    result.valid = 0;

                    return result;
                }


                /*
                * The condition must
                * produce BOOLEAN.
                */
                if (conditionResult.type != SEM_BOOLEAN) {

                    result.valid = 0;

                    return result;
                }


                /*
                * Analyze the true branch.
                */
                SemanticResult trueResult =
                    analyzeAST(node->children[1]);

                if (!trueResult.valid) {

                    result.valid = 0;

                    return result;
                }


                /*
                * Analyze the false branch.
                */
                SemanticResult falseResult =
                    analyzeAST(node->children[2]);

                if (!falseResult.valid) {

                    result.valid = 0;

                    return result;
                }


                /*
                * Both branches must
                * produce the same type.
                */
                if (trueResult.type != falseResult.type) {

                    result.valid = 0;

                    return result;
                }


                /*
                * IF produces the type
                * of its branches.
                */
                result.type = trueResult.type;

                return result;
            }


            /*
            * Other functions are not yet
            * supported by the semantic analyzer.
            */
            result.valid = 0;

            return result;
        }


        default:

            result.valid = 0;

            return result;
    }
}