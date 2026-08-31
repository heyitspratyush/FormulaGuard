#include <stdio.h>
#include "semantic.h"

int analyzeAST(ASTNode *node) {

    if (node == NULL) {
        return 0;
    }

    switch (node->type) {

        case AST_NUMBER:
            return 1;

        case AST_CELL:
            return 1;

        case AST_RANGE:

            if (node->left == NULL ||
                node->right == NULL) {
                return 0;
            }

            if (node->left->type != AST_CELL ||
                node->right->type != AST_CELL) {
                return 0;
            }

            return 1;

        case AST_BINARY_OP:

            if (node->left == NULL ||
                node->right == NULL) {
                return 0;
            }

            if (!analyzeAST(node->left)) {
                return 0;
            }

            if (!analyzeAST(node->right)) {
                return 0;
            }

            return 1;

        case AST_FUNCTION:

            if (node->childCount <= 0 ||
                node->children == NULL) {
                return 0;
            }

            for (int i = 0;
                 i < node->childCount;
                 i++) {

                if (!analyzeAST(node->children[i])) {
                    return 0;
                }
            }

            return 1;

        default:
            return 0;
    }
}