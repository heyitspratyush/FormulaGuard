#ifndef AST_H
#define AST_H

#include "lexer.h"


typedef enum {

    AST_CELL,

    AST_NUMBER,

    AST_BINARY_OP,

    AST_FUNCTION

} ASTNodeType;


typedef struct ASTNode {

    ASTNodeType type;

    char value[50];

    struct ASTNode *left;

    struct ASTNode *right;

    struct ASTNode **children;
    int childCount;

} ASTNode;


ASTNode *createCellNode(const char *value);

ASTNode *createNumberNode(const char *value);

ASTNode *createBinaryOpNode(const char *operator,ASTNode *left,ASTNode *right);

ASTNode *createFunctionNode(
    const char *functionName,
    ASTNode **argument,
    int argumentCount
);


void printAST(ASTNode *node, int depth);

void freeAST(ASTNode *node);


#endif