#include "codeGenerator.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


FILE* outputFile; // Moved outside the test case for global access

DataElement* dataConsts[MAX_CONSTS];
int constCount = 0;

// Array of temporary registers, used for register allocation and tracking which registers are currently in use
MIPSRegister tempIntRegisters[NUM_TEMP_REGISTERS] = {
    {"$t0", false}, {"$t1", false}, {"$t2", false}, {"$t3", false},
    {"$t4", false}, {"$t5", false}, {"$t6", false}, {"$t7", false},
    {"$t8", false}, {"$t9", false}
};

MIPSRegister tempFloatRegisters[NUM_TEMP_REGISTERS] = {
    {"$f0", false}, {"$f1", false}, {"$f2", false}, {"$f3", false},
    {"$f4", false}, {"$f5", false}, {"$f6", false}, {"$f7", false},
    {"$f8", false}, {"$f9", false}
};


// Initialize the code generator and open the file where the output will be saved
void initCodeGenerator(const char* outputFilename) {
    outputFile = fopen(outputFilename, "w"); // Open in write mode
    if (outputFile == NULL) {
        perror("Failed to open output file");
        exit(EXIT_FAILURE);
    }
}

// Translate TAC instructions to MIPS assembly and output to a file
//      True register allocation will have to be disregarded for this project, due to time constraints.
void generateMIPS(TAC* tacInstructions, const SymbolTable* table) {
    TAC* current = tacInstructions;
    
    // Start the MIPS code
    fprintf(outputFile, ".text\n");
    fprintf(outputFile, ".globl main\n");
    fprintf(outputFile, "main:\n");

    while (current != NULL) {
        // TODO(?): This many else-ifs makes me feel like a widely-reviled indie developer
        //      Is there a cleaner way to handle this many string comparisons in C?
        
        
        if (strcmp(current->op, "assign.int") == 0) {           //INT INSTRUCTIONS
            generateIntAssign(current);
        } else if (strcmp(current->op, "+.int") == 0) {
            generateIntAddition(current);
        } else if (strcmp(current->op, "-.int") == 0) {
            generateIntSubtraction(current);
        } else if (strcmp(current->op, "*.int") == 0) {
            generateIntMultiplication(current);
        } else if (strcmp(current->op, "/.int") == 0) {
            generateIntDivision(current);
        } else if (strcmp(current->op, "^.int") == 0) {
            // TODO: Exponent
        } else if (strcmp(current->op, "write.int") == 0) {
            generateIntWrite(current);
        } else if (strcmp(current->op, "store.int") == 0) {
            generateIntStore(current);
        } else if (strcmp(current->op, "load.int") == 0) {
            generateIntLoad(current);
        } else if (strcmp(current->op, "assign.float") == 0) {   //FLOAT INSTRUCTIONS
            generateFloatAssign(current);
        } else if (strcmp(current->op, "+.float") == 0) {   
            generateFloatAddition(current);
        } else if (strcmp(current->op, "-.float") == 0) {
            generateFloatSubtraction(current);
        } else if (strcmp(current->op, "*.float") == 0) {
            generateFloatMultiplication(current);
        } else if (strcmp(current->op, "/.float") == 0) {
            generateFloatDivision(current);
        } else if (strcmp(current->op, "^.float") == 0) {
            // TODO: Exponent
        } else if (strcmp(current->op, "write.float") == 0) {
            generateFloatWrite(current);
        } else if (strcmp(current->op, "store.float") == 0) {
            generateFloatStore(current);
        } else if (strcmp(current->op, "load.float") == 0) {
            generateFloatLoad(current);
        } else if (strcmp(current->op, "store.intIndex") == 0) { //ARRAY INSTRUCTIONS
            generateArrIntStore(current);
        } else if (strcmp(current->op, "load.intIndex") == 0) {
            generateArrIntLoad(current);
        } else if (strcmp(current->op, "store.floatIndex") == 0) {
            generateArrFloatStore(current);
        } else if (strcmp(current->op, "load.floatIndex") == 0) {
            generateArrFloatLoad(current);
        }
        // Add more cases for other operators (*, /) and conditional jumps if necessary

        current = current->next;
    }

    // Exit the program
    fprintf(outputFile, "\tli $v0, 10 #END\n");
    fprintf(outputFile, "\tsyscall\n");

    // Append all declared memory addresses to the end of the file
    //      (Would do this as a header, but it overcomplicates float constant addresses)
    fprintf(outputFile, "\n.data\n"); // You can add variable declarations here if needed
    fprintf(outputFile, "   newline: .asciiz \"\\n\"\n"); //Text 

    //Iterate through the SymbolTable, and initialize each variable
    declareMipsVars(table);

    //Initialize float constants
    printConstsToFile();
}

//  === INTEGER OPERATIONS ===
//      ADDITION
void generateIntAddition(TAC* current)
{
    //ADD (sum 2 vars)
    //  lw $t0, tempVar1 (load word (int))
    //  lw $t1, tempVar2
    //  add $t0, $t0, $t1 (sum registers)
    //  sw $t0, tempVar3 (store word (int))

    //registers required: 2
    int regIndex1, regIndex2;

    // Handle addition: t2 = t0 + t1
    regIndex1 = allocateIntRegister();
    if (regIndex1 == -1) {
        printf("Error: No available registers\n");
        return;
    }
    regIndex2 = allocateIntRegister();
    if (regIndex2 == -1) {
        printf("Error: No available registers\n");
        return;
    }

    printf("\tlw %s, %s #ADD [%s = %s %s %s]\n", tempIntRegisters[regIndex1].name, current->arg1, current->result, current->arg1, current->op, current->arg2); // Load word
    fprintf(outputFile, "\tlw %s, %s #ADD [%s = %s %s %s]\n", tempIntRegisters[regIndex1].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
    printf("\tlw %s, %s\n", tempIntRegisters[regIndex2].name, current->arg2); // Load word
    fprintf(outputFile, "\tlw %s, %s\n", tempIntRegisters[regIndex2].name, current->arg2);
    printf("\tadd %s, %s, %s\n", tempIntRegisters[regIndex1].name, tempIntRegisters[regIndex1].name, tempIntRegisters[regIndex2].name); // Add
    fprintf(outputFile, "\tadd %s, %s, %s\n", tempIntRegisters[regIndex1].name, tempIntRegisters[regIndex1].name, tempIntRegisters[regIndex2].name);
    printf("\tsw %s, %s\n", tempIntRegisters[regIndex1].name, current->result); // Store word
    fprintf(outputFile, "\tsw %s, %s\n", tempIntRegisters[regIndex1].name, current->result);

    deallocateIntRegister(regIndex1);
    deallocateIntRegister(regIndex2);
}

//      SUBTRACTION
void generateIntSubtraction(TAC* current)
{
    //SUB (sum 2 vars)
    //  lw $t0, tempVar1 (load word (int))
    //  lw $t1, tempVar2
    //  sub $t0, $t0, $t1 (sum registers)
    //  sw $t0, tempVar3 (store word (int))

    //registers required: 2
    int regIndex1, regIndex2;

    // Handle addition: t2 = t0 + t1
    regIndex1 = allocateIntRegister();
    if (regIndex1 == -1) {
        printf("Error: No available registers\n");
        return;
    }
    regIndex2 = allocateIntRegister();
    if (regIndex2 == -1) {
        printf("Error: No available registers\n");
        return;
    }

    printf("\tlw %s, %s #SUB [%s = %s %s %s]\n", tempIntRegisters[regIndex1].name, current->arg1, current->result, current->arg1, current->op, current->arg2); // Load word
    fprintf(outputFile, "\tlw %s, %s #SUB [%s = %s %s %s]\n", tempIntRegisters[regIndex1].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
    printf("\tlw %s, %s\n", tempIntRegisters[regIndex2].name, current->arg2);                          // Load word
    fprintf(outputFile, "\tlw %s, %s\n", tempIntRegisters[regIndex2].name, current->arg2);
    printf("\tsub %s, %s, %s\n", tempIntRegisters[regIndex1].name, tempIntRegisters[regIndex1].name, tempIntRegisters[regIndex2].name);      //Subtract
    fprintf(outputFile, "\tsub %s, %s, %s\n", tempIntRegisters[regIndex1].name, tempIntRegisters[regIndex1].name, tempIntRegisters[regIndex2].name); // Store word
    printf("\tsw %s, %s\n", tempIntRegisters[regIndex1].name, current->result); 
    fprintf(outputFile, "\tsw %s, %s\n", tempIntRegisters[regIndex1].name, current->result);

    deallocateIntRegister(regIndex1);
    deallocateIntRegister(regIndex2);
}

//      MULTIPLICATION
void generateIntMultiplication(TAC* current)
{
    //SUB (sum 2 vars)
    //  lw $t0, tempVar1 (load word (int))
    //  lw $t1, tempVar2
    //  sub $t0, $t0, $t1 (sum registers)
    //  sw $t0, tempVar3 (store word (int))

    //registers required: 2
    int regIndex1, regIndex2;

    // Handle addition: t2 = t0 + t1
    regIndex1 = allocateIntRegister();
    if (regIndex1 == -1) {
        printf("Error: No available registers\n");
        return;
    }
    regIndex2 = allocateIntRegister();
    if (regIndex2 == -1) {
        printf("Error: No available registers\n");
        return;
    }

    printf("\tlw %s, %s #MUL [%s = %s %s %s]\n", tempIntRegisters[regIndex1].name, current->arg1, current->result, current->arg1, current->op, current->arg2); // Load word
    fprintf(outputFile, "\tlw %s, %s #MULTIPLY [%s = %s %s %s]\n", tempIntRegisters[regIndex1].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
    printf("\tlw %s, %s\n", tempIntRegisters[regIndex2].name, current->arg2);                          // Load word
    fprintf(outputFile, "\tlw %s, %s\n", tempIntRegisters[regIndex2].name, current->arg2);
    printf("\tmult %s, %s, %s\n", tempIntRegisters[regIndex1].name, tempIntRegisters[regIndex1].name, tempIntRegisters[regIndex2].name);      //Subtract
    fprintf(outputFile, "\tmul %s, %s, %s\n", tempIntRegisters[regIndex1].name, tempIntRegisters[regIndex1].name, tempIntRegisters[regIndex2].name); // Store word
    printf("\tsw %s, %s\n", tempIntRegisters[regIndex1].name, current->result); 
    fprintf(outputFile, "\tsw %s, %s\n", tempIntRegisters[regIndex1].name, current->result);

    deallocateIntRegister(regIndex1);
    deallocateIntRegister(regIndex2);
}

//      DIVISION
void generateIntDivision(TAC* current)
{
    //DIV (sum 2 vars)
    //  lw $t0, tempVar1 (load word (int))
    //  lw $t1, tempVar2
    //  div $t0, $t1 (divide HI/LO, remainder in HI, result in LO)
    //  mflo $t0
    //  sw $t0, tempVar3 (store word (int))

    //NOTE: THIS METHOD DISCARDS REMAINDERS

    //registers required: 2
    int regIndex1, regIndex2;

    // Handle addition: t2 = t0 + t1
    regIndex1 = allocateIntRegister();
    if (regIndex1 == -1) {
        printf("Error: No available registers\n");
        return;
    }
    regIndex2 = allocateIntRegister();
    if (regIndex2 == -1) {
        printf("Error: No available registers\n");
        return;
    }

    printf("\tlw %s, %s #MUL [%s = %s %s %s]\n", tempIntRegisters[regIndex1].name, current->arg1, current->result, current->arg1, current->op, current->arg2); // Load word
    fprintf(outputFile, "\tlw %s, %s #MULTIPLY [%s = %s %s %s]\n", tempIntRegisters[regIndex1].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
    printf("\tlw %s, %s\n", tempIntRegisters[regIndex2].name, current->arg2);                          // Load word
    fprintf(outputFile, "\tlw %s, %s\n", tempIntRegisters[regIndex2].name, current->arg2);
    printf("\tdiv %s, %s\n", tempIntRegisters[regIndex1].name, tempIntRegisters[regIndex2].name);      //Divide
    fprintf(outputFile, "\tdiv %s, %s\n", tempIntRegisters[regIndex1].name, tempIntRegisters[regIndex2].name); 
    printf("\tmflo %s\n", tempIntRegisters[regIndex1].name);      //Retrieve LO (result)
    fprintf(outputFile, "\tmflo %s\n", tempIntRegisters[regIndex1].name);
    printf("\tsw %s, %s\n", tempIntRegisters[regIndex1].name, current->result); // Store word
    fprintf(outputFile, "\tsw %s, %s\n", tempIntRegisters[regIndex1].name, current->result);

    deallocateIntRegister(regIndex1);
    deallocateIntRegister(regIndex2);
}

//      ASSIGN
void generateIntAssign(TAC* current)
{
    //ASSIGN (assign constant value to var)
    //  *DON'T* load temp var, it's getting overwritten anyways
    //  li $t0, const (load immediate (const int))
    //  sw $t0, tempVar (store word (int))

    //registers required: 1
    int regIndex;

    // Handle assignment: t0 = constant or variable
    regIndex = allocateIntRegister();
    if (regIndex == -1) {
        printf("Error: No available registers\n");
        return;
    }
    printf("\tli %s, %s #ASSIGN [%s = %s %s %s]\n", tempIntRegisters[regIndex].name, current->arg1, current->result, current->arg1, current->op, current->arg2); // Load immediate
    fprintf(outputFile, "\tli %s, %s #ASSIGN [%s = %s %s %s]\n", tempIntRegisters[regIndex].name, current->arg1, current->result, current->arg1, current->op, current->arg2); // Load immediate
    printf("\tsw %s, %s\n", tempIntRegisters[regIndex].name, current->result); // Store word
    fprintf(outputFile, "\tsw %s, %s\n", tempIntRegisters[regIndex].name, current->result); // Store word
    deallocateIntRegister(regIndex);
}

//      WRITE
void generateIntWrite(TAC* current)
{
    //WRITE (display var to console)
    //  lw $t0, tempVar (load word (int))
    //  li $v0, 1 (call 1: write int)
    //  *DON'T* save temp var, it hasn't changed

    //registers required: 0 (load directly into reserved output register)
    // int regIndex;

    // Handle write: (null) = x write (null)

    //  lw $t0, tempVar (load word (int))
    printf("\tlw $a0, %s #WRITE [%s = %s %s %s]\n", current->arg1, current->result, current->arg1, current->op, current->arg2); //Move int into output register
    fprintf(outputFile, "\tlw $a0, %s #WRITE [%s = %s %s %s]\n", current->arg1, current->result, current->arg1, current->op, current->arg2);
    //  sw $t0, var (load word (int))
    printf("\tli $v0, 1\n");                //call 1: write int
    fprintf(outputFile, "\tli $v0, 1\n");
    printf("\tsyscall\n");                  //execute
    fprintf(outputFile, "\tsyscall\n"); 
    
    //PLACEHOLDER: Print a new line for output clarity
    printf("\tli $v0, 4\n");                //call 4: write string
    fprintf(outputFile, "\tli $v0, 4\n"); 
    printf("\tla $a0, newline\n");          //Move int into output register
    fprintf(outputFile, "\tla $a0, newline\n"); 
    printf("\tsyscall\n");                  //execute
    fprintf(outputFile, "\tsyscall\n");
    
    // deallocateIntRegister(regIndex);
}

//      STORE
void generateIntStore(TAC* current)
{
    //STORE (store temp var value in named var)
    //  lw $t0, tempVar (load word (int))
    //  sw $t0, var (load word (int))

    //registers required: 1
    int regIndex;

    regIndex = allocateIntRegister();
    if (regIndex == -1) {
        printf("Error: No available registers\n");
        return;
    }

    //  lw $t0, tempVar (load word (int))
    printf("\tlw %s, %s #STORE [%s = %s %s %s]\n", tempIntRegisters[regIndex].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
    fprintf(outputFile, "\tlw %s, %s #STORE [%s = %s %s %s]\n", tempIntRegisters[regIndex].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
    //  sw $t0, var (load word (int))
    printf("\tsw %s, %s\n", tempIntRegisters[regIndex].name, current->result);
    fprintf(outputFile, "\tsw %s, %s\n", tempIntRegisters[regIndex].name, current->result);

    deallocateIntRegister(regIndex);
}

//      LOAD
void generateIntLoad(TAC* current)
{
    //LOAD (load named var value into temp var)
        //  lw $t0, tempVar (load word (int))
        //  sw $t0, var (load word (int))

        //registers required: 1
        int regIndex;

        regIndex = allocateIntRegister();
        if (regIndex == -1) {
            printf("Error: No available registers\n");
            return;
        }

        //  lw $t0, tempVar (load word (int))
        printf("\tlw %s, %s #LOAD [%s = %s %s %s]\n", tempIntRegisters[regIndex].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
        fprintf(outputFile, "\tlw %s, %s #LOAD [%s = %s %s %s]\n", tempIntRegisters[regIndex].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
        //  sw $t0, var (load word (int))
        printf("\tsw %s, %s\n", tempIntRegisters[regIndex].name, current->result);
        fprintf(outputFile, "\tsw %s, %s\n", tempIntRegisters[regIndex].name, current->result);
        
        deallocateIntRegister(regIndex);
}

//  === FLOAT OPERATIONS ===
//      ADDITION
void generateFloatAddition(TAC* current)
{
    //ADD (sum 2 vars)
    //  lw $t0, tempVar1 (load word (int))
    //  lw $t1, tempVar2
    //  add $t0, $t0, $t1 (sum registers)
    //  sw $t0, tempVar3 (store word (int))

    //registers required: 2
    int regIndex1, regIndex2;

    // Handle addition: t2 = t0 + t1
    regIndex1 = allocateFloatRegister();
    if (regIndex1 == -1) {
        printf("Error: No available registers\n");
        return;
    }
    regIndex2 = allocateFloatRegister();
    if (regIndex2 == -1) {
        printf("Error: No available registers\n");
        return;
    }

    printf("\tl.s %s, %s #ADD [%s = %s %s %s]\n", tempFloatRegisters[regIndex1].name, current->arg1, current->result, current->arg1, current->op, current->arg2); // Load word
    fprintf(outputFile, "\tl.s %s, %s #ADD [%s = %s %s %s]\n", tempFloatRegisters[regIndex1].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
    printf("\tl.s %s, %s\n", tempFloatRegisters[regIndex2].name, current->arg2); // Load float
    fprintf(outputFile, "\tl.s %s, %s\n", tempFloatRegisters[regIndex2].name, current->arg2);
    printf("\tadd.s %s, %s, %s\n", tempFloatRegisters[regIndex1].name, tempFloatRegisters[regIndex1].name, tempFloatRegisters[regIndex2].name); // Add
    fprintf(outputFile, "\tadd.s %s, %s, %s\n", tempFloatRegisters[regIndex1].name, tempFloatRegisters[regIndex1].name, tempFloatRegisters[regIndex2].name);
    printf("\ts.s %s, %s\n", tempFloatRegisters[regIndex1].name, current->result); // Store float
    fprintf(outputFile, "\ts.s %s, %s\n", tempFloatRegisters[regIndex1].name, current->result);

    deallocateFloatRegister(regIndex1);
    deallocateFloatRegister(regIndex2);
}

//      SUBTRACTION
void generateFloatSubtraction(TAC* current)
{
    //SUB (sum 2 vars)
    //  lw $t0, tempVar1 (load word (int))
    //  lw $t1, tempVar2
    //  sub $t0, $t0, $t1 (sum registers)
    //  sw $t0, tempVar3 (store word (int))

    //registers required: 2
    int regIndex1, regIndex2;

    // Handle addition: t2 = t0 + t1
    regIndex1 = allocateFloatRegister();
    if (regIndex1 == -1) {
        printf("Error: No available registers\n");
        return;
    }
    regIndex2 = allocateFloatRegister();
    if (regIndex2 == -1) {
        printf("Error: No available registers\n");
        return;
    }

    printf("\tl.s %s, %s #SUB [%s = %s %s %s]\n", tempFloatRegisters[regIndex1].name, current->arg1, current->result, current->arg1, current->op, current->arg2); // Load word
    fprintf(outputFile, "\tl.s %s, %s #SUB [%s = %s %s %s]\n", tempFloatRegisters[regIndex1].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
    printf("\tl.s %s, %s\n", tempFloatRegisters[regIndex2].name, current->arg2);                          // Load word
    fprintf(outputFile, "\tl.s %s, %s\n", tempFloatRegisters[regIndex2].name, current->arg2);
    printf("\tsub.s %s, %s, %s\n", tempFloatRegisters[regIndex1].name, tempFloatRegisters[regIndex1].name, tempFloatRegisters[regIndex2].name);      //Subtract
    fprintf(outputFile, "\tsub.s %s, %s, %s\n", tempFloatRegisters[regIndex1].name, tempFloatRegisters[regIndex1].name, tempFloatRegisters[regIndex2].name); // Store word
    printf("\ts.s %s, %s\n", tempFloatRegisters[regIndex1].name, current->result); 
    fprintf(outputFile, "\ts.s %s, %s\n", tempFloatRegisters[regIndex1].name, current->result);

    deallocateFloatRegister(regIndex1);
    deallocateFloatRegister(regIndex2);
}

//      MULTIPLICATION
void generateFloatMultiplication(TAC* current)
{
    //MUL (sum 2 vars)
    //  l.s $t0, tempVar1 (load word (int))
    //  l.s $t1, tempVar2
    //  mult.s $t0, $t0, $t1 (multiply registers)
    //  s.s $f0, tempVar3 (store word (float))

    //registers required: 2
    int regIndex1, regIndex2;

    // Handle addition: t2 = t0 + t1
    regIndex1 = allocateFloatRegister();
    if (regIndex1 == -1) {
        printf("Error: No available registers\n");
        return;
    }
    regIndex2 = allocateFloatRegister();
    if (regIndex2 == -1) {
        printf("Error: No available registers\n");
        return;
    }

    printf("\tl.s %s, %s #MUL [%s = %s %s %s]\n", tempFloatRegisters[regIndex1].name, current->arg1, current->result, current->arg1, current->op, current->arg2); // Load word
    fprintf(outputFile, "\tl.s %s, %s #MULTIPLY [%s = %s %s %s]\n", tempFloatRegisters[regIndex1].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
    printf("\tl.s %s, %s\n", tempFloatRegisters[regIndex2].name, current->arg2);                          // Load word
    fprintf(outputFile, "\tl.s %s, %s\n", tempFloatRegisters[regIndex2].name, current->arg2);
    printf("\tmul.s %s, %s, %s\n", tempFloatRegisters[regIndex1].name, tempFloatRegisters[regIndex1].name, tempFloatRegisters[regIndex2].name);      //Subtract
    fprintf(outputFile, "\tmul.s %s, %s, %s\n", tempFloatRegisters[regIndex1].name, tempFloatRegisters[regIndex1].name, tempFloatRegisters[regIndex2].name); // Store word
    printf("\ts.s %s, %s\n", tempFloatRegisters[regIndex1].name, current->result); 
    fprintf(outputFile, "\ts.s %s, %s\n", tempFloatRegisters[regIndex1].name, current->result);

    deallocateFloatRegister(regIndex1);
    deallocateFloatRegister(regIndex2);
}

//      DIVISION
void generateFloatDivision(TAC* current)
{
    //DIV (divide by)
    //  l.s $t0, tempVar1 (load word (int))
    //  l.s $t1, tempVar2
    //  div.s $t0, $t0, $t1 (multiply registers)
    //  s.s $f0, tempVar3 (store word (float))

    //registers required: 2
    int regIndex1, regIndex2;

    // Handle addition: t2 = t0 + t1
    regIndex1 = allocateFloatRegister();
    if (regIndex1 == -1) {
        printf("Error: No available registers\n");
        return;
    }
    regIndex2 = allocateFloatRegister();
    if (regIndex2 == -1) {
        printf("Error: No available registers\n");
        return;
    }

    printf("\tl.s %s, %s #MUL [%s = %s %s %s]\n", tempFloatRegisters[regIndex1].name, current->arg1, current->result, current->arg1, current->op, current->arg2); // Load word
    fprintf(outputFile, "\tl.s %s, %s #MULTIPLY [%s = %s %s %s]\n", tempFloatRegisters[regIndex1].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
    printf("\tl.s %s, %s\n", tempFloatRegisters[regIndex2].name, current->arg2);                          // Load word
    fprintf(outputFile, "\tl.s %s, %s\n", tempFloatRegisters[regIndex2].name, current->arg2);
    printf("\tdiv.s %s, %s, %s\n", tempFloatRegisters[regIndex1].name, tempFloatRegisters[regIndex1].name, tempFloatRegisters[regIndex2].name);      //Subtract
    fprintf(outputFile, "\tdiv.s %s, %s, %s\n", tempFloatRegisters[regIndex1].name, tempFloatRegisters[regIndex1].name, tempFloatRegisters[regIndex2].name); // Store word
    printf("\ts.s %s, %s\n", tempFloatRegisters[regIndex1].name, current->result); 
    fprintf(outputFile, "\ts.s %s, %s\n", tempFloatRegisters[regIndex1].name, current->result);

    deallocateFloatRegister(regIndex1);
    deallocateFloatRegister(regIndex2);
}

//      ASSIGN
void generateFloatAssign(TAC* current)
{
    //ASSIGN (assign constant value to var)
    //  *DON'T* load temp var, it's getting overwritten anyways
    //  li $t0, const (load immediate (const int))
    //  sw $t0, tempVar (store word (int))

    //registers required: 1
    int regIndex;

    // Handle assignment: t0 = constant or variable
    regIndex = allocateFloatRegister();
    if (regIndex == -1) {
        printf("Error: No available registers\n");
        return;
    }

    //Create
    DataElement* newConst = createConst("float",("%f",current->arg1));
    // printf("\tl.s %s, %s #ASSIGN [%s = %s %s %s]\n", tempFloatRegisters[regIndex].name, current->arg1, current->result, current->arg1, current->op, current->arg2); // Load immediate
    // fprintf(outputFile, "\tl.s %s, %s #ASSIGN [%s = %s %s %s]\n", tempFloatRegisters[regIndex].name, current->arg1, current->result, current->arg1, current->op, current->arg2); // Load immediate
    printf("\tlwc1 %s, %s #ASSIGN [%s = %s %s %s]\n", tempFloatRegisters[regIndex].name, newConst->varName, current->result, current->arg1, current->op, current->arg2); // Load immediate
    fprintf(outputFile, "\tlwc1 %s, %s #ASSIGN [%s = %s %s %s]\n", tempFloatRegisters[regIndex].name, newConst->varName, current->result, current->arg1, current->op, current->arg2); // Load immediate
    printf("\ts.s %s, %s\n", tempFloatRegisters[regIndex].name, current->result); // Store float
    fprintf(outputFile, "\ts.s %s, %s\n", tempFloatRegisters[regIndex].name, current->result); // Store float
    deallocateFloatRegister(regIndex);
}

//      WRITE
void generateFloatWrite(TAC* current)
{
    //WRITE (display var to console)
    //  lw $t0, tempVar (load word (int))
    //  li $v0, 1 (call 1: write int)
    //  *DON'T* save temp var, it hasn't changed

    //registers required: 0 (load directly into reserved output register)
    // int regIndex;

    // Handle write: (null) = x write (null)

    //  lw $t0, tempVar (load word (int))
    printf("\tl.s $f12, %s #WRITE [%s = %s %s %s]\n", current->arg1, current->result, current->arg1, current->op, current->arg2); //Move int into output register
    fprintf(outputFile, "\tl.s $f12, %s #WRITE [%s = %s %s %s]\n", current->arg1, current->result, current->arg1, current->op, current->arg2);
    //  sw $t0, var (load word (int))
    printf("\tli $v0, 2\n");                //call 1: write float
    fprintf(outputFile, "\tli $v0, 2\n");
    printf("\tsyscall\n");                  //execute
    fprintf(outputFile, "\tsyscall\n");
    
    //PLACEHOLDER: Print a new line for output clarity
    printf("\tli $v0, 4\n");                //call 4: write string
    fprintf(outputFile, "\tli $v0, 4\n"); 
    printf("\tla $a0, newline\n");          //Move int into output register
    fprintf(outputFile, "\tla $a0, newline\n"); 
    printf("\tsyscall\n");                  //execute
    fprintf(outputFile, "\tsyscall\n");
    
    // deallocateIntRegister(regIndex);
}

//      STORE
void generateFloatStore(TAC* current)
{
    //STORE (store temp var value in named var)
    //  lw $t0, tempVar (load word (int))
    //  sw $t0, var (load word (int))

    //registers required: 1
    int regIndex;

    regIndex = allocateFloatRegister();
    if (regIndex == -1) {
        printf("Error: No available registers\n");
        return;
    }

    //  lw $t0, tempVar (load word (int))
    printf("\tl.s %s, %s #STORE [%s = %s %s %s]\n", tempFloatRegisters[regIndex].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
    fprintf(outputFile, "\tl.s %s, %s #STORE [%s = %s %s %s]\n", tempFloatRegisters[regIndex].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
    //  sw $t0, var (load word (int))
    printf("\ts.s %s, %s\n", tempFloatRegisters[regIndex].name, current->result);
    fprintf(outputFile, "\ts.s %s, %s\n", tempFloatRegisters[regIndex].name, current->result);

    deallocateFloatRegister(regIndex);
}

//      LOAD
void generateFloatLoad(TAC* current)
{
    //LOAD (load named var value into temp var)
    //  lw $t0, tempVar (load word (int))
    //  sw $t0, var (load word (int))

    //registers required: 1
    int regIndex;

    regIndex = allocateFloatRegister();
    if (regIndex == -1) {
        printf("Error: No available registers\n");
        return;
    }

    //  lw $t0, tempVar (load word (int))
    printf("\tl.s %s, %s #LOAD [%s = %s %s %s]\n", tempFloatRegisters[regIndex].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
    fprintf(outputFile, "\tl.s %s, %s #LOAD [%s = %s %s %s]\n", tempFloatRegisters[regIndex].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
    //  sw $t0, var (load word (int))
    printf("\ts.s %s, %s\n", tempFloatRegisters[regIndex].name, current->result);
    fprintf(outputFile, "\ts.s %s, %s\n", tempFloatRegisters[regIndex].name, current->result);
    
    deallocateFloatRegister(regIndex);
}

//ARRAY-SPECIFIC
//      STORE IN INDEX
void generateArrIntStore(TAC* current)
{
    //STORE (store temp var value in index of array)
    //  la $t0, arrayName (load address)
    //  lw $t1, indexVar (load word (int))
    //  sll $t1, $t1, 2  (byte offset of 4-- bit shift twice to multiply index by 4)
    //  add $t2, $t0, $t1 (add offset to arr address to get index address)
    //  lw $t3, tempVar (load word from var)
    //  sw $t3, 0($t2) (store word in arr index)


    //  la $t0, arrayName (load address)
    //  lw $t1, indexVar (load word (int))
    //  sll $t1, $t1, 2  (byte offset of 4-- bit shift twice to multiply index by 4)
    //  add $t2, $t0, $t1 (add offset to arr address to get index address)
    //  lw $t3, 0($t2) (load word (int))
    //  sw $t3, 0($t2) (store in result in)

    //registers required: 3
    int addressRegIndex, offsetRegIndex, resultRegIndex, addressSumRegIndex;

    addressRegIndex = allocateIntRegister();
    if (addressRegIndex == -1) {
        printf("Error: No available registers\n");
        return;
    }
    offsetRegIndex = allocateIntRegister();
    if (offsetRegIndex == -1) {
        printf("Error: No available registers\n");
        return;
    }
    addressSumRegIndex = allocateIntRegister();
    if (resultRegIndex == -1) {
        printf("Error: No available registers\n");
        return;
    }
    resultRegIndex = allocateIntRegister();
    if (resultRegIndex == -1) {
        printf("Error: No available registers\n");
        return;
    }

    fprintf(outputFile, "\tla %s, %s #STORE (int arr) [%s = %s %s %s]\n", tempIntRegisters[addressRegIndex].name, current->result, current->result, current->arg1, current->op, current->arg2);
    fprintf(outputFile, "\tlw %s, %s\n", tempIntRegisters[offsetRegIndex].name, current->arg2);
    fprintf(outputFile, "\tsll %s, %s, 2\n", tempIntRegisters[offsetRegIndex].name, tempIntRegisters[offsetRegIndex].name);
    fprintf(outputFile, "\tadd %s, %s, %s\n", tempIntRegisters[addressSumRegIndex].name, tempIntRegisters[addressRegIndex].name, tempIntRegisters[offsetRegIndex].name);
    fprintf(outputFile, "\tlw %s, %s\n", tempIntRegisters[resultRegIndex].name, current->arg1);
    fprintf(outputFile, "\tsw %s, 0(%s)\n", tempIntRegisters[resultRegIndex].name, tempIntRegisters[addressSumRegIndex].name);
    
    //  lw $t0, tempVar (load word (int))
    // printf("\tlw %s, %s #STORE [%s = %s %s %s]\n", tempIntRegisters[regIndex].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
    // fprintf(outputFile, "\tlw %s, %s #STORE [%s = %s %s %s]\n", tempIntRegisters[regIndex].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
    // //  sw $t0, var (load word (int))
    // printf("\tsw %s, %s\n", tempIntRegisters[regIndex].name, current->result);
    // fprintf(outputFile, "\tsw %s, %s\n", tempIntRegisters[regIndex].name, current->result);

    deallocateIntRegister(addressRegIndex);
    deallocateIntRegister(offsetRegIndex);
    deallocateIntRegister(addressSumRegIndex);
    deallocateIntRegister(resultRegIndex);
}

//      LOAD FROM INDEX
void generateArrIntLoad(TAC* current)
{
    //LOAD (load value from index of array)
    //  la $t0, arrayName (load address)
    //  lw $t1, indexVar (load word containing index)
    //  sll $t1, $t1, 2  (byte offset of 4-- bit shift twice to multiply index by 4)
    //  add $t2, $t0, $t1 (add offset to arr address to get index address)
    //  lw $t3, 0($t2) (load int in index)
    //  sw $t3, resultVar (store in result destination)

    //registers required: 4
    int addressRegIndex, offsetRegIndex, resultRegIndex, addressSumRegIndex;

    addressRegIndex = allocateIntRegister();
    if (addressRegIndex == -1) {
        printf("Error: No available registers\n");
        return;
    }
    offsetRegIndex = allocateIntRegister();
    if (offsetRegIndex == -1) {
        printf("Error: No available registers\n");
        return;
    }
    addressSumRegIndex = allocateIntRegister();
    if (resultRegIndex == -1) {
        printf("Error: No available registers\n");
        return;
    }
    resultRegIndex = allocateIntRegister();
    if (resultRegIndex == -1) {
        printf("Error: No available registers\n");
        return;
    }

    fprintf(outputFile, "\tla %s, %s #LOAD (int arr) [%s = %s %s %s]\n", tempIntRegisters[addressRegIndex].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
    fprintf(outputFile, "\tlw %s, %s\n", tempIntRegisters[offsetRegIndex].name, current->arg2);
    fprintf(outputFile, "\tsll %s, %s, 2\n", tempIntRegisters[offsetRegIndex].name, tempIntRegisters[offsetRegIndex].name);
    fprintf(outputFile, "\tadd %s, %s, %s\n", tempIntRegisters[addressSumRegIndex].name, tempIntRegisters[addressRegIndex].name, tempIntRegisters[offsetRegIndex].name);
    fprintf(outputFile, "\tlw %s, 0(%s)\n", tempIntRegisters[resultRegIndex].name, tempIntRegisters[addressSumRegIndex].name);
    fprintf(outputFile, "\tsw %s, %s\n", tempIntRegisters[resultRegIndex].name, current->result);
    
    //  lw $t0, tempVar (load word (int))
    // printf("\tlw %s, %s #STORE [%s = %s %s %s]\n", tempIntRegisters[regIndex].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
    // fprintf(outputFile, "\tlw %s, %s #STORE [%s = %s %s %s]\n", tempIntRegisters[regIndex].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
    // //  sw $t0, var (load word (int))
    // printf("\tsw %s, %s\n", tempIntRegisters[regIndex].name, current->result);
    // fprintf(outputFile, "\tsw %s, %s\n", tempIntRegisters[regIndex].name, current->result);

    deallocateIntRegister(addressRegIndex);
    deallocateIntRegister(offsetRegIndex);
    deallocateIntRegister(addressSumRegIndex);
    deallocateIntRegister(resultRegIndex);
}

//      STORE IN INDEX
void generateArrFloatStore(TAC* current)
{
    //STORE (store temp var value in index of array)
    //  la $t0, arrayName (load address)
    //  lw $t1, indexVar (load word (int))
    //  sll $t1, $t1, 2  (byte offset of 4-- bit shift twice to multiply index by 4)
    //  add $t2, $t0, $t1 (add offset to arr address to get index address)
    //  lw $t3, tempVar (load word from var)
    //  sw $t3, 0($t2) (store word in arr index)


    //  la $t0, arrayName (load address)
    //  lw $t1, indexVar (load word (int))
    //  sll $t1, $t1, 2  (byte offset of 4-- bit shift twice to multiply index by 4)
    //  add $t2, $t0, $t1 (add offset to arr address to get index address)
    //  lw $t3, 0($t2) (load word (int))
    //  sw $t3, 0($t2) (store in result in)

    //registers required: 3 int, 1 float
    int addressRegIndex, offsetRegIndex, resultRegIndex, addressSumRegIndex;

    addressRegIndex = allocateIntRegister();
    if (addressRegIndex == -1) {
        printf("Error: No available registers\n");
        return;
    }
    offsetRegIndex = allocateIntRegister();
    if (offsetRegIndex == -1) {
        printf("Error: No available registers\n");
        return;
    }
    addressSumRegIndex = allocateIntRegister();
    if (resultRegIndex == -1) {
        printf("Error: No available registers\n");
        return;
    }
    resultRegIndex = allocateFloatRegister();
    if (resultRegIndex == -1) {
        printf("Error: No available registers\n");
        return;
    }

    fprintf(outputFile, "\tla %s, %s #STORE (int arr) [%s = %s %s %s]\n", tempIntRegisters[addressRegIndex].name, current->result, current->result, current->arg1, current->op, current->arg2);
    fprintf(outputFile, "\tlw %s, %s\n", tempIntRegisters[offsetRegIndex].name, current->arg2);
    fprintf(outputFile, "\tsll %s, %s, 2\n", tempIntRegisters[offsetRegIndex].name, tempIntRegisters[offsetRegIndex].name);
    fprintf(outputFile, "\tadd %s, %s, %s\n", tempIntRegisters[addressSumRegIndex].name, tempIntRegisters[addressRegIndex].name, tempIntRegisters[offsetRegIndex].name);
    fprintf(outputFile, "\tlw %s, %s\n", tempFloatRegisters[resultRegIndex].name, current->arg1);
    fprintf(outputFile, "\tsw %s, 0(%s)\n", tempFloatRegisters[resultRegIndex].name, tempIntRegisters[addressSumRegIndex].name);
    
    //  lw $t0, tempVar (load word (int))
    // printf("\tlw %s, %s #STORE [%s = %s %s %s]\n", tempIntRegisters[regIndex].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
    // fprintf(outputFile, "\tlw %s, %s #STORE [%s = %s %s %s]\n", tempIntRegisters[regIndex].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
    // //  sw $t0, var (load word (int))
    // printf("\tsw %s, %s\n", tempIntRegisters[regIndex].name, current->result);
    // fprintf(outputFile, "\tsw %s, %s\n", tempIntRegisters[regIndex].name, current->result);

    deallocateIntRegister(addressRegIndex);
    deallocateIntRegister(offsetRegIndex);
    deallocateIntRegister(addressSumRegIndex);
    deallocateFloatRegister(resultRegIndex);
}

//      LOAD FROM INDEX
void generateArrFloatLoad(TAC* current)
{
    //LOAD (load value from index of array)
    //  la $t0, arrayName (load address)
    //  lw $t1, indexVar (load word containing index)
    //  sll $t1, $t1, 2  (byte offset of 4-- bit shift twice to multiply index by 4)
    //  add $t2, $t0, $t1 (add offset to arr address to get index address)
    //  lw $t3, 0($t2) (load int in index)
    //  sw $t3, resultVar (store in result destination)

    //registers required: 4
    int addressRegIndex, offsetRegIndex, resultRegIndex, addressSumRegIndex;

    addressRegIndex = allocateIntRegister();
    if (addressRegIndex == -1) {
        printf("Error: No available registers\n");
        return;
    }
    offsetRegIndex = allocateIntRegister();
    if (offsetRegIndex == -1) {
        printf("Error: No available registers\n");
        return;
    }
    addressSumRegIndex = allocateIntRegister();
    if (resultRegIndex == -1) {
        printf("Error: No available registers\n");
        return;
    }
    resultRegIndex = allocateFloatRegister();
    if (resultRegIndex == -1) {
        printf("Error: No available registers\n");
        return;
    }

    fprintf(outputFile, "\tla %s, %s #LOAD (int arr) [%s = %s %s %s]\n", tempIntRegisters[addressRegIndex].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
    fprintf(outputFile, "\tlw %s, %s\n", tempIntRegisters[offsetRegIndex].name, current->arg2);
    fprintf(outputFile, "\tsll %s, %s, 2\n", tempIntRegisters[offsetRegIndex].name, tempIntRegisters[offsetRegIndex].name);
    fprintf(outputFile, "\tadd %s, %s, %s\n", tempIntRegisters[addressSumRegIndex].name, tempIntRegisters[addressRegIndex].name, tempIntRegisters[offsetRegIndex].name);
    fprintf(outputFile, "\tlw %s, 0(%s)\n", tempFloatRegisters[resultRegIndex].name, tempIntRegisters[addressSumRegIndex].name);
    fprintf(outputFile, "\tsw %s, %s\n", tempFloatRegisters[resultRegIndex].name, current->result);
    
    //  lw $t0, tempVar (load word (int))
    // printf("\tlw %s, %s #STORE [%s = %s %s %s]\n", tempIntRegisters[regIndex].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
    // fprintf(outputFile, "\tlw %s, %s #STORE [%s = %s %s %s]\n", tempIntRegisters[regIndex].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
    // //  sw $t0, var (load word (int))
    // printf("\tsw %s, %s\n", tempIntRegisters[regIndex].name, current->result);
    // fprintf(outputFile, "\tsw %s, %s\n", tempIntRegisters[regIndex].name, current->result);

    deallocateIntRegister(addressRegIndex);
    deallocateIntRegister(offsetRegIndex);
    deallocateIntRegister(addressSumRegIndex);
    deallocateFloatRegister(resultRegIndex);
}

//Header Setup
void declareMipsVars(const SymbolTable* table)
{
    //Workaround: TABLE_SIZE can't be used here. Maybe declare it in SymbolTable?
    for (int i = 0; i < 100; i++) //TABLE_SIZE = 100
    {
        Symbol* current = table->table[i];
        while (current)
        {
            char* repeatToken;
            if (current->type == VarType_Int) {
                fprintf(outputFile, "\t%s: .word 0", current->name);
                if (current->isArray) repeatToken = ", 0";
            } else if (current->type == VarType_Float) {
                fprintf(outputFile, "\t%s: .float 0.0", current->name);
                if (current->isArray) repeatToken = ", 0.0";
            }
            
            if (current->isArray)
            {
                for (int i = 0; i < current->arrSize; i++)
                {
                    fprintf(outputFile, "%s", repeatToken);
                }
            }
            fprintf(outputFile, "\n");
            
            
            current = current->next;
        }
    }
    //Temp vars
    // for (int i = 0; i < getTempVarCount(); i++)
    // {
    //     fprintf(outputFile, "\tt%d: .word 0\n", i);
    // }
    for (int i = 0; i < getTempIntCount(); i++)
    {
        fprintf(outputFile, "\ti%d: .word 0\n", i);
    }
    for (int i = 0; i < getTempFloatCount(); i++)
    {
        fprintf(outputFile, "\tf%d: .float 0.0\n", i);
    }
    
}

// Finalize the code generation and close the output file
void finalizeCodeGenerator(const char* outputFilename) {
    if (outputFile) {
        fclose(outputFile);
        printf("MIPS code generated and saved to file %s\n", outputFilename);
        outputFile = NULL;
    }
}

// Allocate a register from the temp register pool
int allocateIntRegister() {
    for (int i = 0; i < NUM_TEMP_REGISTERS; i++) {
        if (!tempIntRegisters[i].inUse) {
            tempIntRegisters[i].inUse = true;
            return i; // Return the register index
        }
    }
    return -1; // Indicate failure (no available register)
}

int allocateFloatRegister() {
    for (int i = 0; i < NUM_TEMP_REGISTERS; i++) {
        if (!tempFloatRegisters[i].inUse) {
            tempFloatRegisters[i].inUse = true;
            return i; // Return the register index
        }
    }
    return -1; // Indicate failure (no available register)
}

// Deallocate a register (mark it as free)
void deallocateIntRegister(int regIndex) {
    if (regIndex >= 0 && regIndex < NUM_TEMP_REGISTERS) {
        tempIntRegisters[regIndex].inUse = false;
    }
}

void deallocateFloatRegister(int regIndex) {
    if (regIndex >= 0 && regIndex < NUM_TEMP_REGISTERS) {
        tempFloatRegisters[regIndex].inUse = false;
    }
}

//Create a const to be included at the end of the MIPS file
DataElement* createConst(const char* type, const char* contents) {
    dataConsts[constCount] = malloc(sizeof(DataElement));
    dataConsts[constCount]->dataType = type;
    dataConsts[constCount]->contents = contents;

    //Name the new constant
    char buffer[30];
    sprintf(buffer, "%s_const_%d", type, constCount);
    dataConsts[constCount]->varName = strdup(buffer);
    constCount++;

    return dataConsts[constCount - 1];
}

void printConstsToFile()
{
    for (int i = 0; dataConsts[i] != NULL; i++)
    {
        fprintf(outputFile, "\t%s: .%s %s\n", dataConsts[i]->varName, dataConsts[i]->dataType, dataConsts[i]->contents);
    }
}




