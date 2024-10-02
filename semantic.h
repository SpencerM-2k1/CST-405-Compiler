#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "AST.h"         // AST structure definitions
#include "symbolTable.h" // Symbol Table structure definitions

// TAC structure for Intermediate Representation (IR)
//  > Doubly-linked list
typedef struct TAC {
    char* op;   // Operator
    char* arg1; // Argument 1
    char* arg2; // Argument 2
    char* result; // Result
    struct TAC* prev; // Pointer to the previous TAC instruction
    struct TAC* next; // Pointer to the next TAC instruction
} TAC;

extern TAC* tacHead;   // Global head of the TAC list
extern TAC* tacTail;   // Global head of the TAC list

extern int tempVarCount;

void semanticAnalysis(ASTNode* node, SymbolTable* symTab);  // Semantic analysis function
TAC* generateTACForExpr(ASTNode* expr);  // TAC generation for expressions
TAC* generateTACForWrite(ASTNode* writeStmt); // TAC generation for write statement
TAC* generateTACForAssign(ASTNode* assignStmt); // TAC generation for write statement
char* createTempVar();   // Create temporary variables for TAC
char* createOperand(ASTNode* node);   // Create operand strings for TAC
void printTAC(TAC* tac);  // Print TAC to console
void printTACToFile(const char* filename, TAC* tac);  // Print TAC to a file
void appendTAC(TAC** head, TAC** tail, TAC* newInstruction);  // Append TAC to the list

//Manual TAC methods
TAC* createTAC(char* result, char* arg1, char* op, char* arg2); //Create a TAC manually (required for optimizer)
void removeTAC(TAC** del); //Remove TAC from list and discard; close the resulting gap in list by linking neighbors
void freeTAC(TAC** del); //Frees the memory in a TAC
void replaceTAC(TAC** oldTAC, TAC** newTAC); //Remove a TAC from list, and insert another TAC in its place; link with neighbors

#endif // SEMANTIC_H

//TAC OPERATIONS:

//+     - Addition
//-     - Subtraction
//write - print value in variable
//store - save value in temp variable to variable
//load  - load value from variable to temp variable

