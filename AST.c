#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "AST.h"

//Used for debug
char* NodeTypeNames[10] = {"NodeType_Program",
    "NodeType_VarDeclList", 
    "NodeType_VarDecl", 
    "NodeType_SimpleExpr",
    "NodeType_SimpleID",
    "NodeType_Expr", 
    "NodeType_StmtList",
    "NodeType_AssignStmt",
    "NodeType_BinOp", 
    "NodeType_WriteStmt"};

// Function to print indentation and branch lines
void printIndent(bool* drawVertical, int level) {
    for (int i = 0; i < level; i++) {
        if (drawVertical[i]) {
            printf("|   ");
        } else {
            printf("    ");
        }
    }
}

// Modified traverseAST function
void traverseAST(ASTNode* node, int level, bool* drawVertical, bool isLast) {
    if (!node) {
        return;
    }
    // fprintf(stderr, "type: %s\n", NodeTypeNames[node->type]);
    printIndent(drawVertical, level - 1);

    // Choose the appropriate branch symbol
    if (level > 0) {
        printf(isLast ? "└── " : "├── ");
    }

    // Display the node type and relevant information
    switch (node->type) {
        case NodeType_Program:
            printf("Program\n");

            // Count the number of children
            int numChildren = 0;
            if (node->data.program.varDeclList) numChildren++;
            if (node->data.program.stmtList) numChildren++;

            int childIndex = 0;
            drawVertical[level] = !isLast;
            if (node->data.program.varDeclList) {
                traverseAST(node->data.program.varDeclList, level + 1, drawVertical, ++childIndex == numChildren);
            }
            if (node->data.program.stmtList) {
                traverseAST(node->data.program.stmtList, level + 1, drawVertical, ++childIndex == numChildren);
            }
            break;

        case NodeType_VarDeclList:
            printf("VarDeclList\n");

            numChildren = 0;
            if (node->data.varDeclList.varDecl) numChildren++;
            if (node->data.varDeclList.varDeclList) numChildren++;

            childIndex = 0;
            drawVertical[level - 1] = !isLast;
            if (node->data.varDeclList.varDecl) {
                traverseAST(node->data.varDeclList.varDecl, level + 1, drawVertical, ++childIndex == numChildren);
            }
            if (node->data.varDeclList.varDeclList) {
                traverseAST(node->data.varDeclList.varDeclList, level + 1, drawVertical, ++childIndex == numChildren);
            }
            break;

        case NodeType_VarDecl:
            printf("VarDecl: %s %s\n", node->data.varDecl.varType, node->data.varDecl.varName);
            break;

        case NodeType_SimpleExpr:
            printf("SimpleExpr: %d\n", node->data.simpleExpr.number);
            break;

        case NodeType_SimpleID:
            printf("SimpleID: %s\n", node->data.simpleID.name);
            break;

        case NodeType_Expr:
            // printf("Expr: %c\n", node->data.expr.operator);
            printf("Expr: %s\n", node->data.expr.operator);
            drawVertical[level - 1] = !isLast;
            traverseAST(node->data.expr.left, level + 1, drawVertical, false);
            traverseAST(node->data.expr.right, level + 1, drawVertical, true);
            break;

        case NodeType_StmtList:

            numChildren = 0;
            if (node->data.stmtList.stmt) numChildren++;
            if (node->data.stmtList.stmtList) numChildren++;
            
            if (node->data.stmtList.stmt != NULL) {
                printf("StmtList\n");
                childIndex = 0;
                drawVertical[level - 1] = !isLast;
                if (node->data.stmtList.stmt) {
                    // fprintf(stderr, "node->data.stmtList.stmt detected\n");
                    traverseAST(node->data.stmtList.stmt, level + 1, drawVertical, ++childIndex == numChildren);
                }
                if (node->data.stmtList.stmtList) {
                    // fprintf(stderr, "node->data.stmtList.stmtList detected\n");
                    traverseAST(node->data.stmtList.stmtList, level + 1, drawVertical, ++childIndex == numChildren);
                }
            }
            else
            {
                printf("StmtList - END\n");
            }
            break;

        case NodeType_AssignStmt:
            printf("AssignStmt: %s %s\n", node->data.assignStmt.varName, node->data.assignStmt.operator);

            drawVertical[level - 1] = !isLast;
            traverseAST(node->data.assignStmt.expr, level + 1, drawVertical, true);
            break;

        case NodeType_BinOp:
            printf("BinOp: %c\n", node->data.binOp.operator);
            drawVertical[level - 1] = !isLast;
            traverseAST(node->data.binOp.left, level + 1, drawVertical, false);
            traverseAST(node->data.binOp.right, level + 1, drawVertical, true);
            break;

        case NodeType_WriteStmt:
            printf("WriteStmt: %s\n", node->data.writeStmt.varName);
            drawVertical[level - 1] = !isLast;
            break;

        default:
            printf("Unknown Node Type (type: %d)\n", node->type);
            break;
    }
}

void freeAST(ASTNode* node) {
    if (!node) return;

    switch (node->type) {
        case NodeType_Program:
            freeAST(node->data.program.varDeclList);
            freeAST(node->data.program.stmtList);
            break;

        case NodeType_VarDeclList:
            freeAST(node->data.varDeclList.varDecl);
            freeAST(node->data.varDeclList.varDeclList);
            break;

        case NodeType_VarDecl:
            free(node->data.varDecl.varType);
            free(node->data.varDecl.varName);
            break;

        case NodeType_SimpleExpr:
            // No dynamic memory to free
            break;

        case NodeType_SimpleID:
            free(node->data.simpleID.name);
            break;

        case NodeType_Expr:
            freeAST(node->data.expr.left);
            freeAST(node->data.expr.right);
            break;

        case NodeType_StmtList:
            freeAST(node->data.stmtList.stmt);
            freeAST(node->data.stmtList.stmtList);
            break;

        case NodeType_AssignStmt:
            free(node->data.assignStmt.operator);
            free(node->data.assignStmt.varName);
            freeAST(node->data.assignStmt.expr);
            break;

        case NodeType_BinOp:
            freeAST(node->data.binOp.left);
            freeAST(node->data.binOp.right);
            break;

        case NodeType_WriteStmt:
            free(node->data.writeStmt.varName);
            break;

        default:
            break;
    }

    free(node);
}

ASTNode* createNode(NodeType type) {
    ASTNode* newNode = (ASTNode*)malloc(sizeof(ASTNode));
    if (newNode == NULL) {
        // Handle memory allocation failure if needed
        return NULL;
    }

    newNode->type = type;

    // Initialize the node based on its type
    switch (type) {
        case NodeType_Program:
            newNode->data.program.varDeclList = NULL;
            newNode->data.program.stmtList = NULL;
            break;

        case NodeType_VarDeclList:
            newNode->data.varDeclList.varDecl = NULL;
            newNode->data.varDeclList.varDeclList = NULL;
            break;

        case NodeType_VarDecl:
            newNode->data.varDecl.varType = NULL;
            newNode->data.varDecl.varName = NULL;
            break;

        case NodeType_SimpleExpr:
            newNode->data.simpleExpr.number = 0;
            break;

        case NodeType_SimpleID:
            newNode->data.simpleID.name = NULL;
            break;

        case NodeType_Expr:
            // newNode->data.expr.operator = '\0';
            newNode->data.expr.operator = NULL;
            newNode->data.expr.left = NULL;
            newNode->data.expr.right = NULL;
            break;

        case NodeType_StmtList:
            newNode->data.stmtList.stmt = NULL;
            newNode->data.stmtList.stmtList = NULL;
            break;

        case NodeType_AssignStmt:
            newNode->data.assignStmt.operator = NULL;
            newNode->data.assignStmt.varName = NULL;
            newNode->data.assignStmt.expr = NULL;
            break;

        case NodeType_BinOp:
            newNode->data.binOp.operator = '\0';
            newNode->data.binOp.left = NULL;
            newNode->data.binOp.right = NULL;
            break;

        case NodeType_WriteStmt:
            newNode->data.writeStmt.varName = NULL;
            break;

        default:
            printf("DEBUG: Invalid NodeType\n");
            break;
    }

    return newNode;
}
