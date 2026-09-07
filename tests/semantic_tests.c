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