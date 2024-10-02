#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// NodeType enum to differentiate between different kinds of AST nodes
typedef enum { 
    NodeType_Program,
    NodeType_VarDeclList, 
    NodeType_VarDecl, 
    NodeType_SimpleExpr,
    NodeType_SimpleID,
    NodeType_Expr, 
    NodeType_StmtList,
    NodeType_AssignStmt,
    NodeType_BinOp, 
    NodeType_WriteStmt,
} NodeType;

// Forward declaration of ASTNode to use in struct definitions
struct ASTNode;

// Structure for AST nodes
typedef struct ASTNode {
    NodeType type;
    union NodeData {
        struct ProgramNode {
            struct ASTNode* varDeclList;
            struct ASTNode* stmtList;
        } program;

        struct VarDeclListNode {
            struct ASTNode* varDecl;
            struct ASTNode* varDeclList;
        } varDeclList;

        struct VarDeclNode {
            char* varType;
            char* varName;
        } varDecl;

        struct SimpleExprNode {
            int number;
            // char* number;
        } simpleExpr;

        struct SimpleIDNode {
            char* name;
        } simpleID;

        struct ExprNode {
            char* operator;
            struct ASTNode* left;
            struct ASTNode* right;
        } expr;

        struct StmtListNode {
            struct ASTNode* stmt;
            struct ASTNode* stmtList;
        } stmtList;

        struct AssignStmtNode {
            char* operator; // e.g., '='
            char* varName;
            struct ASTNode* expr;
        } assignStmt;

        struct BinOpNode {
            char* operator;
            struct ASTNode* left;
            struct ASTNode* right;
        } binOp;

        struct WriteStmtNode {
            char* varName;
        } writeStmt;
    } data;
} ASTNode;

// Function prototypes for AST handling
ASTNode* createNode(NodeType type);
void freeAST(ASTNode* node);
void traverseAST(ASTNode* node, int level, bool* drawVertical, bool isLast);
void printIndent(bool* drawVertical, int level);

#endif // AST_H

