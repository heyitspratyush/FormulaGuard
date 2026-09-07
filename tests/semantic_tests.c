#include <stdio.h>
#include <stdlib.h>

#include "../src/ast.h"
#include "../src/semantic.h"


int passed = 0;
int failed = 0;


void checkResult(
    const char *testName,
    SemanticResult result,
    int expectedValid,
    SemanticType expectedType
) {

    if (
        result.valid == expectedValid &&
        result.type == expectedType
    ) {

        printf("[PASS] %s\n", testName);

        passed++;
    }
    else {

        printf(
            "[FAIL] %s\n"
            "       Expected: valid=%d, type=%s\n"
            "       Actual:   valid=%d, type=%s\n",
            testName,
            expectedValid,
            semanticTypeName(expectedType),
            result.valid,
            semanticTypeName(result.type)
        );

        failed++;
    }
}


void testNumber() {

    ASTNode *node =
        createNumberNode("10");

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "Number produces NUMBER",
        result,
        1,
        SEM_NUMBER
    );

    freeAST(node);
}


void testCell() {

    ASTNode *node =
        createCellNode("A1");

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "Cell produces CELL",
        result,
        1,
        SEM_CELL
    );

    freeAST(node);
}


void testRange() {

    ASTNode *start =
        createCellNode("A1");

    ASTNode *end =
        createCellNode("B5");

    ASTNode *node =
        createRangeNode(start, end);

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "Range produces RANGE",
        result,
        1,
        SEM_RANGE
    );

    freeAST(node);
}


void testAddition() {

    ASTNode *left =
        createCellNode("A1");

    ASTNode *right =
        createNumberNode("10");

    ASTNode *node =
        createBinaryOpNode(
            "+",
            left,
            right
        );

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "CELL + NUMBER produces NUMBER",
        result,
        1,
        SEM_NUMBER
    );

    freeAST(node);
}


void testCellAddition() {

    ASTNode *left =
        createCellNode("A1");

    ASTNode *right =
        createCellNode("B1");

    ASTNode *node =
        createBinaryOpNode(
            "+",
            left,
            right
        );

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "CELL + CELL produces NUMBER",
        result,
        1,
        SEM_NUMBER
    );

    freeAST(node);
}


void testMultiplication() {

    ASTNode *left =
        createCellNode("A1");

    ASTNode *right =
        createCellNode("B1");

    ASTNode *node =
        createBinaryOpNode(
            "*",
            left,
            right
        );

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "CELL * CELL produces NUMBER",
        result,
        1,
        SEM_NUMBER
    );

    freeAST(node);
}


void testGreaterThan() {

    ASTNode *left =
        createCellNode("A1");

    ASTNode *right =
        createNumberNode("10");

    ASTNode *node =
        createBinaryOpNode(
            ">",
            left,
            right
        );

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "CELL > NUMBER produces BOOLEAN",
        result,
        1,
        SEM_BOOLEAN
    );

    freeAST(node);
}


void testLessThan() {

    ASTNode *left =
        createCellNode("A1");

    ASTNode *right =
        createCellNode("B1");

    ASTNode *node =
        createBinaryOpNode(
            "<",
            left,
            right
        );

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "CELL < CELL produces BOOLEAN",
        result,
        1,
        SEM_BOOLEAN
    );

    freeAST(node);
}


void testLessEqual() {

    ASTNode *left =
        createCellNode("A1");

    ASTNode *right =
        createCellNode("B1");

    ASTNode *node =
        createBinaryOpNode(
            "<=",
            left,
            right
        );

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "CELL <= CELL produces BOOLEAN",
        result,
        1,
        SEM_BOOLEAN
    );

    freeAST(node);
}


void testGreaterEqual() {

    ASTNode *left =
        createCellNode("A1");

    ASTNode *right =
        createNumberNode("10");

    ASTNode *node =
        createBinaryOpNode(
            ">=",
            left,
            right
        );

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "CELL >= NUMBER produces BOOLEAN",
        result,
        1,
        SEM_BOOLEAN
    );

    freeAST(node);
}


void testNotEqual() {

    ASTNode *left =
        createCellNode("A1");

    ASTNode *right =
        createCellNode("B1");

    ASTNode *node =
        createBinaryOpNode(
            "<>",
            left,
            right
        );

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "CELL <> CELL produces BOOLEAN",
        result,
        1,
        SEM_BOOLEAN
    );

    freeAST(node);
}


void testInvalidRangeArithmetic() {

    ASTNode *start =
        createCellNode("A1");

    ASTNode *end =
        createCellNode("B5");

    ASTNode *range =
        createRangeNode(start, end);

    ASTNode *number =
        createNumberNode("10");

    ASTNode *node =
        createBinaryOpNode(
            "+",
            range,
            number
        );

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "RANGE + NUMBER is invalid",
        result,
        0,
        SEM_ERROR
    );

    freeAST(node);
}
void testValidCellReference() {

    ASTNode *node =
        createCellNode("A1");

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "A1 is a valid cell reference",
        result,
        1,
        SEM_CELL
    );

    freeAST(node);
}


void testValidMultiLetterCellReference() {

    ASTNode *node =
        createCellNode("AA27");

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "AA27 is a valid cell reference",
        result,
        1,
        SEM_CELL
    );

    freeAST(node);
}


void testInvalidZeroRowReference() {

    ASTNode *node =
        createCellNode("A0");

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "A0 is an invalid cell reference",
        result,
        0,
        SEM_ERROR
    );

    freeAST(node);
}
void testValidRange() {

    ASTNode *start =
        createCellNode("A1");

    ASTNode *end =
        createCellNode("B5");

    ASTNode *node =
        createRangeNode(start, end);

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "A1:B5 is a valid range",
        result,
        1,
        SEM_RANGE
    );

    freeAST(node);
}


void testValidMultiLetterRange() {

    ASTNode *start =
        createCellNode("AA1");

    ASTNode *end =
        createCellNode("BC27");

    ASTNode *node =
        createRangeNode(start, end);

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "AA1:BC27 is a valid range",
        result,
        1,
        SEM_RANGE
    );

    freeAST(node);
}


void testInvalidStartCellInRange() {

    ASTNode *start =
        createCellNode("A0");

    ASTNode *end =
        createCellNode("B5");

    ASTNode *node =
        createRangeNode(start, end);

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "A0:B5 is an invalid range",
        result,
        0,
        SEM_ERROR
    );

    freeAST(node);
}


void testInvalidEndCellInRange() {

    ASTNode *start =
        createCellNode("A1");

    ASTNode *end =
        createCellNode("B0");

    ASTNode *node =
        createRangeNode(start, end);

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "A1:B0 is an invalid range",
        result,
        0,
        SEM_ERROR
    );

    freeAST(node);
}

void testSumWithNumber() {

    ASTNode *argument =
        createNumberNode("10");

    ASTNode **arguments =
        malloc(sizeof(ASTNode *));

    arguments[0] = argument;

    ASTNode *node =
        createFunctionNode(
            "SUM",
            arguments,
            1
        );

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "SUM(NUMBER) produces NUMBER",
        result,
        1,
        SEM_NUMBER
    );

    freeAST(node);
    free(arguments);
}
void testSumWithCell() {

    ASTNode *argument =
        createCellNode("A1");

    ASTNode **arguments =
        malloc(sizeof(ASTNode *));

    arguments[0] = argument;

    ASTNode *node =
        createFunctionNode(
            "SUM",
            arguments,
            1
        );

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "SUM(CELL) produces NUMBER",
        result,
        1,
        SEM_NUMBER
    );

    freeAST(node);
    free(arguments);
}
void testSumWithRange() {

    ASTNode *start =
        createCellNode("A1");

    ASTNode *end =
        createCellNode("B5");

    ASTNode *range =
        createRangeNode(
            start,
            end
        );

    ASTNode **arguments =
        malloc(sizeof(ASTNode *));

    arguments[0] = range;

    ASTNode *node =
        createFunctionNode(
            "SUM",
            arguments,
            1
        );

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "SUM(RANGE) produces NUMBER",
        result,
        1,
        SEM_NUMBER
    );

    freeAST(node);
    free(arguments);
}
void testSumMultipleArguments() {

    ASTNode **arguments =
        malloc(sizeof(ASTNode *) * 2);

    arguments[0] =
        createCellNode("A1");

    arguments[1] =
        createNumberNode("10");

    ASTNode *node =
        createFunctionNode(
            "SUM",
            arguments,
            2
        );

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "SUM(CELL, NUMBER) produces NUMBER",
        result,
        1,
        SEM_NUMBER
    );

    freeAST(node);
    free(arguments);
}
void testSumWithBoolean() {

    ASTNode *left =
        createCellNode("A1");

    ASTNode *right =
        createNumberNode("10");

    ASTNode *comparison =
        createBinaryOpNode(
            ">",
            left,
            right
        );

    ASTNode **arguments =
        malloc(sizeof(ASTNode *));

    arguments[0] = comparison;

    ASTNode *node =
        createFunctionNode(
            "SUM",
            arguments,
            1
        );

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "SUM(BOOLEAN) is invalid",
        result,
        0,
        SEM_ERROR
    );

    freeAST(node);
}
void testUnknownFunction() {

    ASTNode *argument =
        createCellNode("A1");

    ASTNode **arguments =
        malloc(sizeof(ASTNode *));

    arguments[0] = argument;

    ASTNode *node =
        createFunctionNode(
            "XYZ",
            arguments,
            1
        );

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "Unknown function is invalid",
        result,
        0,
        SEM_ERROR
    );

    freeAST(node);
    free(arguments);
}
void testIfWithNumberBranches() {

    ASTNode *condition =
        createBinaryOpNode(
            ">",
            createCellNode("A1"),
            createNumberNode("10")
        );

    ASTNode **arguments =
        malloc(sizeof(ASTNode *) * 3);

    arguments[0] = condition;
    arguments[1] = createNumberNode("100");
    arguments[2] = createNumberNode("200");

    ASTNode *node =
        createFunctionNode(
            "IF",
            arguments,
            3
        );

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "IF(BOOLEAN, NUMBER, NUMBER) produces NUMBER",
        result,
        1,
        SEM_NUMBER
    );

    freeAST(node);
    free(arguments);
}


void testIfWithCellBranches() {

    ASTNode *condition =
        createBinaryOpNode(
            ">",
            createCellNode("A1"),
            createNumberNode("10")
        );

    ASTNode **arguments =
        malloc(sizeof(ASTNode *) * 3);

    arguments[0] = condition;
    arguments[1] = createCellNode("B1");
    arguments[2] = createCellNode("C1");

    ASTNode *node =
        createFunctionNode(
            "IF",
            arguments,
            3
        );

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "IF(BOOLEAN, CELL, CELL) produces CELL",
        result,
        1,
        SEM_CELL
    );

    freeAST(node);
    free(arguments);
}


void testIfWithInvalidCondition() {

    ASTNode **arguments =
        malloc(sizeof(ASTNode *) * 3);

    arguments[0] = createCellNode("A1");
    arguments[1] = createNumberNode("100");
    arguments[2] = createNumberNode("200");

    ASTNode *node =
        createFunctionNode(
            "IF",
            arguments,
            3
        );

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "IF with CELL condition is invalid",
        result,
        0,
        SEM_ERROR
    );

    freeAST(node);
    free(arguments);
}


void testIfWithMismatchedBranches() {

    ASTNode *condition =
        createBinaryOpNode(
            ">",
            createCellNode("A1"),
            createNumberNode("10")
        );

    ASTNode **arguments =
        malloc(sizeof(ASTNode *) * 3);

    arguments[0] = condition;
    arguments[1] = createNumberNode("100");
    arguments[2] = createCellNode("B1");

    ASTNode *node =
        createFunctionNode(
            "IF",
            arguments,
            3
        );

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "IF with mismatched branch types is invalid",
        result,
        0,
        SEM_ERROR
    );

    freeAST(node);
    free(arguments);
}


void testIfWithTooFewArguments() {

    ASTNode *condition =
        createBinaryOpNode(
            ">",
            createCellNode("A1"),
            createNumberNode("10")
        );

    ASTNode **arguments =
        malloc(sizeof(ASTNode *) * 2);

    arguments[0] = condition;
    arguments[1] = createNumberNode("100");

    ASTNode *node =
        createFunctionNode(
            "IF",
            arguments,
            2
        );

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "IF with too few arguments is invalid",
        result,
        0,
        SEM_ERROR
    );

    freeAST(node);
    free(arguments);
}


void testIfWithTooManyArguments() {

    ASTNode *condition =
        createBinaryOpNode(
            ">",
            createCellNode("A1"),
            createNumberNode("10")
        );

    ASTNode **arguments =
        malloc(sizeof(ASTNode *) * 4);

    arguments[0] = condition;
    arguments[1] = createNumberNode("100");
    arguments[2] = createNumberNode("200");
    arguments[3] = createNumberNode("300");

    ASTNode *node =
        createFunctionNode(
            "IF",
            arguments,
            4
        );

    SemanticResult result =
        analyzeAST(node);

    checkResult(
        "IF with too many arguments is invalid",
        result,
        0,
        SEM_ERROR
    );

    freeAST(node);
    free(arguments);
}



int main() {

    printf("FormulaGuard Semantic Tests\n\n");


    testNumber();

    testCell();

    testValidCellReference();

    testValidMultiLetterCellReference();

    testInvalidZeroRowReference();

    testRange();

    testAddition();

    testCellAddition();

    testMultiplication();

    testGreaterThan();

    testLessThan();

    testLessEqual();

    testGreaterEqual();

    testNotEqual();

    testInvalidRangeArithmetic();

    testValidRange();

    testValidMultiLetterRange();

    testInvalidStartCellInRange();

    testInvalidEndCellInRange();

    testSumWithNumber();

    testSumWithCell();

    testSumWithRange();

    testSumMultipleArguments();

    testSumWithBoolean();

    testUnknownFunction();

    testIfWithNumberBranches();

    testIfWithCellBranches();

    testIfWithInvalidCondition();

    testIfWithMismatchedBranches();

    testIfWithTooFewArguments();

    testIfWithTooManyArguments();


    printf(
        "\nResults: %d passed, %d failed\n",
        passed,
        failed
    );


    if (failed == 0) {

        printf("All semantic tests passed.\n");

        return 0;
    }


    printf("Some semantic tests failed.\n");

    return 1;
}