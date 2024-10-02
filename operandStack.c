#include "operandStack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Operand* operandStack[OPERAND_STACK_SIZE];
int operandCursor = 0;

Operand* createOperandStruct(char* opString)
{
    Operand* newOperand = malloc(sizeof(Operand));
    newOperand->operandID = strdup(opString);
    return newOperand;
}

void freeOperand(Operand* op)
{
    if (!op)
    {
        fprintf(stderr, "OPERAND_STACK: Prevented double free segmentation fault error!\n");
        return;
    }
    free(op->operandID);
    free(op);
}

void pushOperand(Operand* op) {
    if (operandCursor < OPERAND_STACK_SIZE) //If stack is not full
    {
        operandStack[operandCursor] = op;
        operandCursor++;
    }
    else
    {
        fprintf(stderr, "OPERAND_STACK: Stack overflow! Halting compiler...\n");
        exit(1);
    }
}

Operand* popOperand() {
    if (operandCursor > 0) //If stack is not empty
    {
        Operand* returnOperand;
        operandCursor--;
        returnOperand = operandStack[operandCursor];
        return returnOperand;
    }
    else
    {
        fprintf(stderr, "OPERAND_STACK: Stack underflow! Halting compiler...\n");
        exit(1);
    }
}

void printOperandStack() {
    printf("    ===OPERAND STACK===\n");
    for (int i = 0; i < operandCursor; i++)
    {
        printf("     - %s\n", operandStack[i]->operandID);
    }
    printf("    ===END OF OPERAND STACK===\n");
}