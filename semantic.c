#include "semantic.h"
#include "symbolTable.h"
#include "codeGenerator.h"
#include "operandStack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global head of the TAC list
TAC* tacHead = NULL;
TAC* tacTail = NULL;

int tempVarCount = 0;

// Semantic analysis function
void semanticAnalysis(ASTNode* node, SymbolTable* symTab) {
    if (node == NULL) return;

    switch (node->type) {
        case NodeType_Program:
            printf("Performing semantic analysis on program\n");
            printf("SEMANTIC: Recognized NodeType_Program\n");
            semanticAnalysis(node->data.program.varDeclList, symTab);
            semanticAnalysis(node->data.program.stmtList, symTab);
            break;

        case NodeType_VarDeclList:
            printf("SEMANTIC: Recognized NodeType_VarDeclList\n");
            semanticAnalysis(node->data.varDeclList.varDecl, symTab);
            semanticAnalysis(node->data.varDeclList.varDeclList, symTab);
            break;

        case NodeType_VarDecl:
            printf("SEMANTIC: Recognized NodeType_VarDecl\n");
            // addSymbol(symTab, node->data.varDecl.varName, node->data.varDecl.varType); //Unnecessary, symbol table handled by parser.y rule
            break;

        case NodeType_StmtList:
            printf("SEMANTIC: Recognized NodeType_StmtList\n");
            semanticAnalysis(node->data.stmtList.stmt, symTab);
            semanticAnalysis(node->data.stmtList.stmtList, symTab);
            break;

        case NodeType_AssignStmt:
            printf("SEMANTIC: Recognized NodeType_AssignStmt\n");
            if (lookupSymbol(symTab, node->data.assignStmt.varName) == NULL) {
                fprintf(stderr, "Semantic error: Variable '%s' not declared\n", node->data.assignStmt.varName);
            }
            semanticAnalysis(node->data.assignStmt.expr, symTab);  // Analyze the right-hand side expression
            break;

        case NodeType_Expr:
            printf("SEMANTIC: Recognized NodeType_Expr\n");
            printf("SEMANTIC: Entering left...\n");
            semanticAnalysis(node->data.expr.left, symTab);
            printf("SEMANTIC: Entering right...\n");
            semanticAnalysis(node->data.expr.right, symTab);
            printf("SEMANTIC: Exiting NodeType_Expr case\n");
            break;

        case NodeType_BinOp:
            printf("SEMANTIC: Recognized NodeType_BinOp\n");
            if (lookupSymbol(symTab, node->data.binOp.left->data.varDecl.varName) == NULL) {
                fprintf(stderr, "Semantic error: Variable '%s' not declared\n", node->data.binOp.left->data.varDecl.varName);
            }
            if (lookupSymbol(symTab, node->data.binOp.right->data.varDecl.varName) == NULL) {
                fprintf(stderr, "Semantic error: Variable '%s' not declared\n", node->data.binOp.right->data.varDecl.varName);
            }
            semanticAnalysis(node->data.binOp.left, symTab);
            semanticAnalysis(node->data.binOp.right, symTab);
            break;

        case NodeType_SimpleID:
            printf("SEMANTIC: Recognized NodeType_SimpleID\n");
            if (lookupSymbol(symTab, node->data.simpleID.name) == NULL) {
                fprintf(stderr, "Semantic error: Variable '%s' not declared\n", node->data.simpleID.name);
            }
            break;

        case NodeType_SimpleExpr:
            printf("SEMANTIC: Recognized NodeType_SimpleExpr\n");
            // No checks necessary for numbers
            break;

        case NodeType_WriteStmt:
            printf("SEMANTIC: Recognized NodeType_WriteStmt\n");
            if (lookupSymbol(symTab, node->data.writeStmt.varName) == NULL) {
                fprintf(stderr, "Semantic error: Variable '%s' not declared\n", node->data.writeStmt.varName);
            }
            break;

        default:
            fprintf(stderr, "Unknown Node Type (type: %d)\n", node->type);
            break;
    }

    if (node->type == NodeType_Expr || node->type == NodeType_SimpleExpr || node->type == NodeType_SimpleID) {
        TAC* tac = generateTACForExpr(node);
        printTAC(tac);
    } else if (node->type == NodeType_WriteStmt) {
        TAC* tac = generateTACForWrite(node);
        //Process or store the generated TAC
        printTAC(tac);
    } else if (node->type == NodeType_AssignStmt) { //Assign final temp value to original symbol
        TAC* tac = generateTACForAssign(node);
    }
}

// Generate TAC for expressions
TAC* generateTACForExpr(ASTNode* expr) {
    if (!expr) return NULL;

    TAC* instruction = (TAC*)malloc(sizeof(TAC));
    if (!instruction) return NULL;

    switch (expr->type) {
        case NodeType_Expr: {   //Complex expression (2+ operands)
            printf("Generating TAC for binary expression\n");
            
            //Pop 2 operands from the stack
            Operand* arg2Operand = popOperand();    //IMPORTANT: Pop arg2 first! We need to preserve non-associative operations
            Operand* arg1Operand = popOperand();
            instruction->arg1 = strdup(arg1Operand->operandID);
            instruction->arg2 = strdup(arg2Operand->operandID);
            freeOperand(arg1Operand);
            freeOperand(arg2Operand);

            instruction->op = strdup(expr->data.expr.operator);  // Get operator
            instruction->result = createTempVar();
            
            //Push operand to stack to use in ongoing BinOp
            Operand* newOperand = createOperandStruct(instruction->result);
            pushOperand(newOperand);
            printOperandStack();
            break;
        }

        case NodeType_SimpleExpr: { //Constant
            printf("Generating TAC for simple expression\n");
            char buffer[20];
            snprintf(buffer, 20, "%d", expr->data.simpleExpr.number);
            instruction->arg1 = strdup(buffer);
            instruction->op = strdup("assign");
            instruction->result = createTempVar();
            
            //Push operand to stack to use in ongoing BinOp
            Operand* newOperand = createOperandStruct(instruction->result);
            pushOperand(newOperand);
            printOperandStack();
            break;
        }

        case NodeType_SimpleID: { //Variable ID
            printf("Generating TAC for simple ID\n");
            instruction->arg1 = strdup(expr->data.simpleID.name);
            instruction->op = strdup("load");
            instruction->result = createTempVar();
            
            //Push operand to stack to use in ongoing BinOp
            Operand* newOperand = createOperandStruct(instruction->result);
            pushOperand(newOperand);
            printOperandStack();
            break;
        }

        default:
            free(instruction);
            return NULL;
    }

    instruction->next = NULL; //Likely unnecessary, but I'm unwilling to risk unknowingly breaking something right now
    appendTAC(&tacHead, &tacTail, instruction);
    return instruction;
}

//Generate TAC for write statement
TAC* generateTACForWrite(ASTNode* expr) {
    if (!expr) return NULL;

    TAC* loadInstr = (TAC*)malloc(sizeof(TAC)); //Loads the variable from 
    if (!loadInstr) return NULL;
    TAC* writeInstr = (TAC*)malloc(sizeof(TAC));
    if (!writeInstr) return NULL;

    printf("Generating TAC for write statement\n");
    printf("    Load var statement");
    loadInstr->arg1 = strdup(expr->data.writeStmt.varName);
    loadInstr->op = strdup("load");
    loadInstr->arg2 = NULL;
    loadInstr->result = createTempVar();


    writeInstr->arg1 = strdup(loadInstr->result);
    writeInstr->op = strdup("write");
    writeInstr->arg2 = NULL;
    writeInstr->result = NULL;

    writeInstr->next = NULL; // Make sure to null-terminate the new instruction

    // Append to the global TAC list
    // appendTAC(&tacHead, instruction);
    appendTAC(&tacHead, &tacTail, loadInstr);
    appendTAC(&tacHead, &tacTail, writeInstr);

    return writeInstr;
}

//Generate TAC for assigning a constant to a temp var
TAC* generateTACForAssign(ASTNode* assignStmt) {
    if (!assignStmt) return NULL;

    TAC* instruction = (TAC*)malloc(sizeof(TAC));
    if (!instruction) return NULL;

    printf("Generating TAC for variable assignment\n");
    //Get temporary variable from assigned expr
    Operand* arg1Operand = popOperand();
    instruction->arg1 = strdup(arg1Operand->operandID);
    instruction->op = strdup("store");

    instruction->arg2 = NULL;
    instruction->result = strdup(assignStmt->data.assignStmt.varName);

    //instruction->next = NULL; // Make sure to null-terminate the new instruction

    // Append to the global TAC list
    // appendTAC(&tacHead, instruction);
    appendTAC(&tacHead, &tacTail, instruction);

    return instruction;
}

// Create temporary variables for TAC
char* createTempVar() {
    // static int count = 0;
    char* tempVar = malloc(30);  // Enough space for "t" + number
    if (!tempVar) return NULL;
    sprintf(tempVar, "t%d", tempVarCount++);
    return tempVar;
}

//Get number of Temp vars
//  required for MIPS header
int getTempVarCount() {
    return tempVarCount;
}

// Append a new TAC instruction to the list
void appendTAC(TAC** head, TAC** tail, TAC* newInstruction) {
    if (!*head) {
        *head = newInstruction;
        *tail = newInstruction;
        newInstruction->prev = NULL;
        newInstruction->next = NULL;
    } else {
        (*tail)->next = newInstruction;
        newInstruction->prev = *tail;
        newInstruction->next = NULL;
        (*tail) = newInstruction;
    }
}

// Print TAC to the console
void printTAC(TAC* tac) {
    if (!tac) return;
    printf("%s = %s %s %s\n", tac->result, tac->arg1 ? tac->arg1 : "(null)", tac->op, tac->arg2 ? tac->arg2 : "(null)");
}

// Print TAC to a file
void printTACToFile(const char* filename, TAC* tac) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }
    TAC* current = tac;
    while (current) {
        fprintf(file, "%s = %s %s %s\n", current->result, current->arg1 ? current->arg1 : "(null)", current->op, current->arg2 ? current->arg2 : "(null)");
        current = current->next;
    }
    fclose(file);
    printf("TAC written to %s\n", filename);
}

//Create TAC from scratch
TAC* createTAC(char* result, char* arg1, char* op, char* arg2)
{
    TAC* newTAC = (TAC*)malloc(sizeof(TAC));
    if(arg1)    {newTAC->arg1 = strdup(arg1);}      else {newTAC->arg1 = NULL;};
    if(arg2)    {newTAC->arg2 = strdup(arg2);}      else {newTAC->arg2 = NULL;};
    if(op)      {newTAC->op = strdup(op);}          else {newTAC->op = NULL;};
    if(result)  {newTAC->result = strdup(result);}  else {newTAC->result = NULL;};
    newTAC->prev = NULL;
    newTAC->next = NULL;
    return newTAC;
}

//Destory a TAC and re-link list
void removeTAC(TAC** del)
{
    //Re-route neighboring TACs
    if (tacHead == (*del)) {
        //Base case: Removing head TAC
        printf("tacHead removed! Assigning new tacHead...\n");
        tacHead = (*del)->next;
    } else {
        (*del)->prev->next = (*del)->next;
    }
    if (tacTail == (*del)) {
        //Removing tail TAC
        printf("tacTail removed! Assigning new tacTail...\n");
        tacTail = (*del)->prev;
    } else {
        (*del)->next->prev = (*del)->prev;
    }
    // void printTACToFile(const char* filename, (*del));  //Before deletion
    freeTAC(&(*del));
    // printf("pointer after deletion: %d\n", (*del)); //After deletion
}

//Free the memory of a TAC. Executed automatically upon removal/replacement
void freeTAC(TAC** del)
{
    if ((*del)->arg1) free((*del)->arg1);
    if ((*del)->arg2) free((*del)->arg2);
    if ((*del)->op) free((*del)->op);
    if ((*del)->result) free((*del)->result);
    free((*del));
}

//Remove a TAC from list, and insert a different TAC in its place
void replaceTAC(TAC** oldTAC, TAC** newTAC)
{
    //Re-route neighboring TACs
    if (tacHead == (*oldTAC)) {
        //Base case: Removing head TAC
        // printf("tacHead removed! Assigning new tacHead...\n");
        tacHead = (*newTAC);
    } else {
        (*oldTAC)->prev->next = (*newTAC);
    }
    if (tacTail == (*oldTAC)) {
        //Removing tail TAC
        // printf("tacTail removed! Assigning new tacTail...\n");
        tacTail = (*newTAC);
    } else {
        (*oldTAC)->next->prev = (*newTAC);
    }

    //Assign prev/next of newTAC
    (*newTAC)->prev = (*oldTAC)->prev;
    (*newTAC)->next = (*oldTAC)->next;

    freeTAC(oldTAC);
}