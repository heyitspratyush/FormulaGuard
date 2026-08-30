#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"


ASTNode *createCellNode(const char *value) {

    ASTNode *node = malloc(sizeof(ASTNode));

    if (node == NULL) {
        return NULL;
    }


    node->type = AST_CELL;

    strcpy(node->value, value);

    node->left = NULL;

    node->right = NULL;

    node->children = NULL;
    node->childCount = 0;


    return node;
}


ASTNode *createNumberNode(const char *value) {

    ASTNode *node = malloc(sizeof(ASTNode));

    if (node == NULL) {
        return NULL;
    }


    node->type = AST_NUMBER;

    strcpy(node->value, value);

    node->left = NULL;

    node->right = NULL;

    node->children = NULL;
    node->childCount = 0;


    return node;
}


ASTNode *createBinaryOpNode(const char *operator,ASTNode *left,ASTNode *right) {

    ASTNode *node = malloc(sizeof(ASTNode));

    if (node == NULL) {
        return NULL;
    }


    node->type = AST_BINARY_OP;

    strcpy(node->value, operator);

    node->left = left;

    node->right = right;


    return node;
}

ASTNode *createFunctionNode(
    const char *functionName,
    ASTNode **arguments,
    int argumentCount
) {

    ASTNode *node = malloc(sizeof(ASTNode));

    if (node == NULL) {

        printf("AST Error: memory allocation failed\n");

        return NULL;
    }

    node->type = AST_FUNCTION;

    strcpy(node->value, functionName);

    node->left = NULL;

    node->right = NULL;

    node->children = arguments;

    node->childCount = argumentCount;

    return node;
}


void printAST(ASTNode *node, int depth) {

    if (node == NULL) {
        return;
    }

    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    if (node->type == AST_CELL) {
        printf("CELL: %s\n", node->value);
    }

    else if (node->type == AST_NUMBER) {
        printf("NUMBER: %s\n", node->value);
    }

    else if (node->type == AST_BINARY_OP) {
        printf("OPERATOR: %s\n", node->value);
    }

   
    else if (node->type == AST_FUNCTION) {

        printf("FUNCTION: %s\n", node->value);

        for (int i = 0; i < node->childCount; i++) {

            printAST(
                node->children[i],
                depth + 1
            );
        }

        return;
    }

    printAST(node->left, depth + 1);

    printAST(node->right, depth + 1);
}

void freeAST(ASTNode *node) {

    if (node == NULL) {
        return;
    }


    freeAST(node->left);

    freeAST(node->right);

    free(node);
}