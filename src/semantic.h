#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"



typedef enum {

    SEM_NUMBER,

    SEM_CELL,

    SEM_RANGE,

    SEM_BOOLEAN,

    SEM_ERROR

} SemanticType;


typedef struct {

    int valid;

    SemanticType type;

} SemanticResult;


SemanticResult analyzeAST(ASTNode *node);

const char *semanticTypeName(SemanticType type);

int isScalarType(SemanticType type);


#endif