#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// NodeType enum to differentiate between different kinds of AST nodes
typedef enum { 
    NodeType_Program,       //root
    NodeType_VarDeclList,   //Linked list containing var/arr declaration nodes
    NodeType_VarDecl,       //Declaration of a var
    NodeType_ArrDecl,       //Declaration of an array
    NodeType_IntExpr,       //Int literal
    NodeType_FloatExpr,     //Float literal
    NodeType_SimpleID,      //Variable ID
    NodeType_ArrAccess,     //Array ID + Index
    NodeType_BinOp,         //Binary operator
    NodeType_StmtList,      //Linked list containing statement nodes
    NodeType_AssignStmt,    //Assign a value to a variable
    NodeType_AssignArrStmt, //Assign a value to the index of an array
    NodeType_WriteStmt,     //Print an expression to the console
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

        struct ArrayNode {
            char* varType;
            char* varName;
            int arrSize;
        } arrDecl;

        struct IntExprNode {
            int number;
        } intExpr;

        struct FloatExprNode {
            float number;
        } floatExpr;

        struct SimpleIDNode {
            char* name;
        } simpleID;

        struct ArrAccessNode {
            char* name;
            struct ASTNode* indexExpr;
        } arrAccess;

        struct BinOpNode {
            char* operator;
            struct ASTNode* left;
            struct ASTNode* right;
        } binOp;

        struct StmtListNode {
            struct ASTNode* stmt;
            struct ASTNode* stmtList;
        } stmtList;

        struct AssignStmtNode {
            char* operator; // e.g., '='
            char* varName;
            struct ASTNode* expr;
        } assignStmt;

        struct AssignArrStmtNode {
            char* operator; // e.g., '='
            char* varName;
            struct ASTNode* indexExpr;
            struct ASTNode* expr;
        } assignArrStmt;

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

