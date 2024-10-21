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

SymbolTable* symTabRef = NULL;

// int tempVarCount = 0;
int tempIntCount = 0;
int tempFloatCount = 0;

// Initialize Semantic Analyzer
void initSemantic(SymbolTable* symbolTable) {
    symTabRef = symbolTable;
}

// Semantic analysis function
void semanticAnalysis(ASTNode* node) {
    if (node == NULL) return;

    switch (node->type) {
        case NodeType_Program:
            printf("Performing semantic analysis on program\n");
            printf("SEMANTIC: Recognized NodeType_Program\n");
            semanticAnalysis(node->data.program.varDeclList);
            semanticAnalysis(node->data.program.stmtList);
            break;

        case NodeType_VarDeclList:
            printf("SEMANTIC: Recognized NodeType_VarDeclList\n");
            semanticAnalysis(node->data.varDeclList.varDecl);
            semanticAnalysis(node->data.varDeclList.varDeclList);
            break;

        case NodeType_VarDecl:
            printf("SEMANTIC: Recognized NodeType_VarDecl\n");
            // addSymbol(symTabRef, node->data.varDecl.varName, node->data.varDecl.varType); //Unnecessary, symbol table handled by parser.y rule
            break;

        case NodeType_StmtList:
            printf("SEMANTIC: Recognized NodeType_StmtList\n");
            semanticAnalysis(node->data.stmtList.stmt);
            semanticAnalysis(node->data.stmtList.stmtList);
            break;

        case NodeType_AssignStmt:
            printf("SEMANTIC: Recognized NodeType_AssignStmt\n");
            if (lookupSymbol(symTabRef, node->data.assignStmt.varName) == NULL) {
                fprintf(stderr, "Semantic error: Variable '%s' not declared\n", node->data.assignStmt.varName);
            }
            semanticAnalysis(node->data.assignStmt.expr);  // Analyze the right-hand side expression
            break;

        case NodeType_BinOp:
            printf("SEMANTIC: Recognized NodeType_BinOp\n");
            printf("SEMANTIC: Entering left...\n");
            semanticAnalysis(node->data.binOp.left);
            printf("SEMANTIC: Entering right...\n");
            semanticAnalysis(node->data.binOp.right);
            printf("SEMANTIC: Exiting NodeType_BinOp case\n");
            break;

        // case NodeType_BinOp:
        //     printf("SEMANTIC: Recognized NodeType_BinOp\n");
        //     if (lookupSymbol(symTabRef, node->data.binOp.left->data.varDecl.varName) == NULL) {
        //         fprintf(stderr, "Semantic error: Variable '%s' not declared\n", node->data.binOp.left->data.varDecl.varName);
        //     }
        //     if (lookupSymbol(symTabRef, node->data.binOp.right->data.varDecl.varName) == NULL) {
        //         fprintf(stderr, "Semantic error: Variable '%s' not declared\n", node->data.binOp.right->data.varDecl.varName);
        //     }
        //     semanticAnalysis(node->data.binOp.left, symTabRef);
        //     semanticAnalysis(node->data.binOp.right, symTabRef);
        //     break;

        case NodeType_SimpleID:
            printf("SEMANTIC: Recognized NodeType_SimpleID\n");
            if (lookupSymbol(symTabRef, node->data.simpleID.name) == NULL) {
                fprintf(stderr, "Semantic error: Variable '%s' not declared\n", node->data.simpleID.name);
            }
            break;

        case NodeType_IntExpr:
            printf("SEMANTIC: Recognized NodeType_IntExpr\n");
            // No checks necessary for numbers
            break;
        
        case NodeType_FloatExpr:
            printf("SEMANTIC: Recognized NodeType_FloatExpr\n");
            // No checks necessary for numbers
            break;

        case NodeType_WriteStmt:
            printf("SEMANTIC: Recognized NodeType_WriteStmt\n");
            if (lookupSymbol(symTabRef, node->data.writeStmt.varName) == NULL) {
                fprintf(stderr, "Semantic error: Variable '%s' not declared\n", node->data.writeStmt.varName);
            }
            break;

        default:
            fprintf(stderr, "Unknown Node Type (type: %d)\n", node->type);
            break;
    }

    if (node->type == NodeType_BinOp || node->type == NodeType_IntExpr || node->type == NodeType_FloatExpr || node->type == NodeType_SimpleID) {
        TAC* tac = generateTACForBinOp(node);
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
TAC* generateTACForBinOp(ASTNode* expr) {
    if (!expr) return NULL;

    TAC* instruction = (TAC*)malloc(sizeof(TAC));
    if (!instruction) return NULL;

    switch (expr->type) {
        case NodeType_BinOp: {   //Complex expression (2+ operands)
            printf("Generating TAC for binary expression\n");            

            //Pop 2 operands from the stack
            Operand* arg2Operand = popOperand();    //IMPORTANT: Pop arg2 first! We need to preserve non-associative operations
            Operand* arg1Operand = popOperand();
            instruction->arg1 = strdup(arg1Operand->operandID);
            printf("arg1Operand->operandID: %s\n",arg1Operand->operandID);
            instruction->arg2 = strdup(arg2Operand->operandID);
            // freeOperand(arg1Operand);
            // freeOperand(arg2Operand);

            // TEMPORARY: Floats and Ints cannot be combined in an expression for now
            // printf("%d %d", arg1Operand->operandType, arg2Operand->operandType);
            if (arg1Operand->operandType != arg2Operand->operandType)
            {
                printf("ERROR: Binary operator does not support int-to-float conversion!\n");
                exit(1);
            }


            // Get the operator, and tag it with the result type of the operation
            //      TODO: Once int-to-float conversion is supported, decide what happens when an int and float are added/multiplied/divided/etc.
            char buffer[20];
            VarType resultType;
            if (arg1Operand->operandType == VarType_Int)
            {
                snprintf(buffer, 20, "%s.int", expr->data.binOp.operator);
                resultType = VarType_Int;
            }
            else if (arg1Operand->operandType == VarType_Float)
            {
                snprintf(buffer, 20, "%s.float", expr->data.binOp.operator);
                resultType = VarType_Float;
            }
            else
            {
                printf("Unrecognized Operand Type: %s; Halting compiler...\n", varTypeToString(arg1Operand->operandType));
                printf("arg1Operand->operandID: %s\n", arg1Operand->operandID);
                exit(1);
            }
            instruction->op = strdup(buffer);  // Get operator
            // printf("instruction->op: %s\n", instruction->op);
            instruction->result = createTempVar(resultType);
            

            //Destroy old operands
            freeOperand(arg1Operand);
            freeOperand(arg2Operand);
            
            //Push operand to stack to use in ongoing BinOp
            Operand* newOperand = createOperandStruct(instruction->result, resultType);
            pushOperand(newOperand);
            printOperandStack();
            break;
        }

        case NodeType_IntExpr: { //Int Constant
            printf("Generating TAC for simple int expression\n");
            char buffer[20];
            snprintf(buffer, 20, "%d", expr->data.intExpr.number);
            instruction->arg1 = strdup(buffer);
            instruction->op = strdup("assign.int");
            instruction->result = createTempVar(VarType_Int);
            
            //Push operand to stack to use in ongoing BinOp
            Operand* newOperand = createOperandStruct(instruction->result, VarType_Int);
            pushOperand(newOperand);
            printOperandStack();
            break;
        }

        case NodeType_FloatExpr: { //Int Constant
            printf("Generating TAC for simple int expression\n");
            char buffer[20];
            snprintf(buffer, 20, "%f", expr->data.floatExpr.number);
            instruction->arg1 = strdup(buffer);
            instruction->op = strdup("assign.float");
            instruction->result = createTempVar(VarType_Float);
            
            //Push operand to stack to use in ongoing BinOp
            Operand* newOperand = createOperandStruct(instruction->result, VarType_Float);
            pushOperand(newOperand);
            printOperandStack();
            break;
        }

        case NodeType_SimpleID: { //Variable ID
            printf("Generating TAC for simple ID\n");
            instruction->arg1 = strdup(expr->data.simpleID.name);
            // instruction->op = strdup("load.int");
            instruction->result = createTempVar(lookupSymbol(symTabRef, expr->data.simpleID.name)->type);
            
            //The following elements of the write expression are type-specific
            // TODO: Nested switch-case is gross, I know. Probably need to split the function up later
            //     Get type
            Symbol* argSymbol = lookupSymbol(symTabRef, expr->data.writeStmt.varName); //Get the symbol of the variable being written

            switch (argSymbol->type) //Type of written argument decides the following...
            {
                case(VarType_Int):
                    instruction->op = strdup("load.int");
                    break;
                case(VarType_Float):
                    instruction->op = strdup("load.float");
                    break;
                case(VarType_Error):
                    printf("SEMANTIC: Unsupported variable type for write statement! Halting...\n");
                    exit(1);
                    break;
                default:
                    printf("SEMANTIC: UNHANDLED VARTYPE! HALTING...\n");
                    exit(1);
                    break;
            }

            //Push operand to stack to use in ongoing BinOp
            Operand* newOperand = createOperandStruct(instruction->result, argSymbol->type);
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
    // printf("    Load var statement");

    //Type-agnostic elements
    loadInstr->arg1 = strdup(expr->data.writeStmt.varName);
    loadInstr->arg2 = NULL;

    writeInstr->arg2 = NULL;
    writeInstr->result = NULL;

    //The following elements of the write expression are type-specific
    
    //     Get type
    Symbol* argSymbol = lookupSymbol(symTabRef, expr->data.writeStmt.varName); //Get the symbol of the variable being written

    switch (argSymbol->type) //Type of written argument decides the following...
    {
        case(VarType_Int):
            loadInstr->op = strdup("load.int");
            loadInstr->result = createTempVar(VarType_Int);
            writeInstr->arg1 = strdup(loadInstr->result);
            writeInstr->op = strdup("write.int");
            break;
        case(VarType_Float):
            loadInstr->op = strdup("load.float");
            loadInstr->result = createTempVar(VarType_Float);
            writeInstr->arg1 = strdup(loadInstr->result);
            writeInstr->op = strdup("write.float");
            break;
        case(VarType_Error):
            printf("SEMANTIC: Unsupported variable type for write statement! Halting...\n");
            exit(1);
            break;
        default:
            printf("SEMANTIC: UNHANDLED VARTYPE! HALTING...\n");
            exit(1);
            break;
    }
    

    writeInstr->next = NULL; // Make sure to null-terminate the new instruction
    
    printTAC(loadInstr);
    printTAC(writeInstr);

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

    //Type-agnostic elements
    instruction->arg1 = strdup(arg1Operand->operandID);

    instruction->arg2 = NULL;
    instruction->result = strdup(assignStmt->data.assignStmt.varName);

    //Type-specific elements
    //The following elements of the write expression are type-specific
    
    //     Get type
    Symbol* argSymbol = lookupSymbol(symTabRef, assignStmt->data.assignStmt.varName); //Get the symbol of the variable being written

    switch (argSymbol->type) //Type of written argument decides the following...
    {
        case(VarType_Int):
            instruction->op = strdup("store.int");
            break;
        case(VarType_Float):
            instruction->op = strdup("store.float");
            break;
        case(VarType_Error):
            printf("SEMANTIC: Unsupported variable type for assign statement! Halting...\n");
            exit(1);
            break;
        default:
            printf("SEMANTIC: UNHANDLED VARTYPE! HALTING...\n");
            exit(1);
            break;
    }

    //instruction->next = NULL; // Make sure to null-terminate the new instruction

    // Append to the global TAC list
    // appendTAC(&tacHead, instruction);
    appendTAC(&tacHead, &tacTail, instruction);

    return instruction;
}

// Create temporary variables for TAC
//  TODO: Clean up temp vars on program termination
char* createTempVar(VarType type) {
    // static int count = 0;
    char* tempVar = malloc(30);  // Enough space for type char + number
    if (!tempVar) return NULL;
    
    /*  Temp Var Types:
    *       *i - int
    *       *f - float
    */
    switch (type)
    {
        case (VarType_Int):
            // sprintf(tempVar, "i%d", tempVarCount++);
            sprintf(tempVar, "i%d", tempIntCount++);
            break;
        case (VarType_Float):
            // sprintf(tempVar, "f%d", tempVarCount++);
            sprintf(tempVar, "f%d", tempFloatCount++);
            break;
        default:
            printf("Unsupported type: %s; Halting compiler...\n", varTypeToString(type));
            printf("%d\n", type);
            exit(1);
            break;
    }
    return tempVar;
}

//Get number of Temp vars
//  required for MIPS header
// int getTempVarCount() {
//     return tempVarCount;
// }
int getTempIntCount() {
    return tempIntCount;
}

float getTempFloatCount() {
    return tempFloatCount;
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