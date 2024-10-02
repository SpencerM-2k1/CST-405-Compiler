#include "codeGenerator.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


FILE* outputFile; // Moved outside the test case for global access

// Array of temporary registers, used for register allocation and tracking which registers are currently in use
MIPSRegister tempRegisters[NUM_TEMP_REGISTERS] = {
    {"$t0", false}, {"$t1", false}, {"$t2", false}, {"$t3", false},
    {"$t4", false}, {"$t5", false}, {"$t6", false}, {"$t7", false},
    {"$t8", false}, {"$t9", false}
};

// Initialize the code generator and open the file where the output will be saved
void initCodeGenerator(const char* outputFilename, const SymbolTable* table) {
    outputFile = fopen(outputFilename, "w"); // Open in write mode
    if (outputFile == NULL) {
        perror("Failed to open output file");
        exit(EXIT_FAILURE);
    }
    // Write the data section header to the file
    fprintf(outputFile, ".data\n"); // You can add variable declarations here if needed
    fprintf(outputFile, "   newline: .asciiz \"\\n\"\n"); //Text 

    //Iterate through the SymbolTable, and initialize each variable
    declareMipsVars(table);
}

// Translate TAC instructions to MIPS assembly and output to a file
//      True register allocation will have to be disregarded for this project, due to time constraints.
void generateMIPS(TAC* tacInstructions) {
    TAC* current = tacInstructions;
    
    // Start the MIPS code
    fprintf(outputFile, ".text\n");
    fprintf(outputFile, ".globl main\n");
    fprintf(outputFile, "main:\n");

    while (current != NULL) {
        if (strcmp(current->op, "assign") == 0) {
            //ASSIGN (assign constant value to var)
            //  *DON'T* load temp var, it's getting overwritten anyways
            //  li $t0, const (load immediate (const int))
            //  sw $t0, tempVar (store word (int))

            //registers required: 1
            int regIndex;

            // Handle assignment: t0 = constant or variable
            regIndex = allocateRegister();
            if (regIndex == -1) {
                printf("Error: No available registers\n");
                return;
            }
            printf("\tli %s, %s #ASSIGN [%s = %s %s %s]\n", tempRegisters[regIndex].name, current->arg1, current->result, current->arg1, current->op, current->arg2); // Load immediate
            fprintf(outputFile, "\tli %s, %s #ASSIGN [%s = %s %s %s]\n", tempRegisters[regIndex].name, current->arg1, current->result, current->arg1, current->op, current->arg2); // Load immediate
            printf("\tsw %s, %s\n", tempRegisters[regIndex].name, current->result); // Store word
            fprintf(outputFile, "\tsw %s, %s\n", tempRegisters[regIndex].name, current->result); // Store word
            deallocateRegister(regIndex);

        } else if (strcmp(current->op, "+") == 0) {
            //ADD (sum 2 vars)
            //  lw $t0, tempVar1 (load word (int))
            //  lw $t1, tempVar2
            //  add $t0, $t0, $t1 (sum registers)
            //  sw $t0, tempVar3 (store word (int))

            //registers required: 2
            int regIndex1, regIndex2;

            // Handle addition: t2 = t0 + t1
            regIndex1 = allocateRegister();
            if (regIndex1 == -1) {
                printf("Error: No available registers\n");
                return;
            }
            regIndex2 = allocateRegister();
            if (regIndex2 == -1) {
                printf("Error: No available registers\n");
                return;
            }

            printf("\tlw %s, %s #ADD [%s = %s %s %s]\n", tempRegisters[regIndex1].name, current->arg1, current->result, current->arg1, current->op, current->arg2); // Load word
            fprintf(outputFile, "\tlw %s, %s #ADD [%s = %s %s %s]\n", tempRegisters[regIndex1].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
            printf("\tlw %s, %s\n", tempRegisters[regIndex2].name, current->arg2); // Load word
            fprintf(outputFile, "\tlw %s, %s\n", tempRegisters[regIndex2].name, current->arg2);
            printf("\tadd %s, %s, %s\n", tempRegisters[regIndex1].name, tempRegisters[regIndex1].name, tempRegisters[regIndex2].name); // Add
            fprintf(outputFile, "\tadd %s, %s, %s\n", tempRegisters[regIndex1].name, tempRegisters[regIndex1].name, tempRegisters[regIndex2].name);
            printf("\tsw %s, %s\n", tempRegisters[regIndex1].name, current->result); // Store word
            fprintf(outputFile, "\tsw %s, %s\n", tempRegisters[regIndex1].name, current->result);

            deallocateRegister(regIndex1);
            deallocateRegister(regIndex2);

        } else if (strcmp(current->op, "-") == 0) {
            //SUB (sum 2 vars)
            //  lw $t0, tempVar1 (load word (int))
            //  lw $t1, tempVar2
            //  sub $t0, $t0, $t1 (sum registers)
            //  sw $t0, tempVar3 (store word (int))

            //registers required: 2
            int regIndex1, regIndex2;

            // Handle addition: t2 = t0 + t1
            regIndex1 = allocateRegister();
            if (regIndex1 == -1) {
                printf("Error: No available registers\n");
                return;
            }
            regIndex2 = allocateRegister();
            if (regIndex2 == -1) {
                printf("Error: No available registers\n");
                return;
            }

            printf("\tlw %s, %s #SUB [%s = %s %s %s]\n", tempRegisters[regIndex1].name, current->arg1, current->result, current->arg1, current->op, current->arg2); // Load word
            fprintf(outputFile, "\tlw %s, %s #SUB [%s = %s %s %s]\n", tempRegisters[regIndex1].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
            printf("\tlw %s, %s\n", tempRegisters[regIndex2].name, current->arg2);                          // Load word
            fprintf(outputFile, "\tlw %s, %s\n", tempRegisters[regIndex2].name, current->arg2);
            printf("\tsub %s, %s, %s\n", tempRegisters[regIndex1].name, tempRegisters[regIndex1].name, tempRegisters[regIndex2].name);      //Subtract
            fprintf(outputFile, "\tsub %s, %s, %s\n", tempRegisters[regIndex1].name, tempRegisters[regIndex1].name, tempRegisters[regIndex2].name); // Store word
            printf("\tsw %s, %s\n", tempRegisters[regIndex1].name, current->result); 
            fprintf(outputFile, "\tsw %s, %s\n", tempRegisters[regIndex1].name, current->result);

            deallocateRegister(regIndex1);
            deallocateRegister(regIndex2);

        } else if (strcmp(current->op, "write") == 0) {
            //WRITE (display var to console)
            //  lw $t0, tempVar (load word (int))
            //  li $v0, 1 (call 1: write int)
            //  *DON'T* save temp var, it hasn't changed

            //registers required: 0 (load directly into reserved output register)
            int regIndex;

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
            
            deallocateRegister(regIndex);

        } else if (strcmp(current->op, "store") == 0) {
            //STORE (store temp var value in named var)
            //  lw $t0, tempVar (load word (int))
            //  sw $t0, var (load word (int))

            //registers required: 1
            int regIndex;

            regIndex = allocateRegister();
            if (regIndex == -1) {
                printf("Error: No available registers\n");
                return;
            }

            //  lw $t0, tempVar (load word (int))
            printf("\tlw %s, %s #STORE [%s = %s %s %s]\n", tempRegisters[regIndex].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
            fprintf(outputFile, "\tlw %s, %s #STORE [%s = %s %s %s]\n", tempRegisters[regIndex].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
            //  sw $t0, var (load word (int))
            printf("\tsw %s, %s\n", tempRegisters[regIndex].name, current->result);
            fprintf(outputFile, "\tsw %s, %s\n", tempRegisters[regIndex].name, current->result);

            deallocateRegister(regIndex);

        } else if (strcmp(current->op, "load") == 0) {
            //LOAD (load named var value into temp var)
            //  lw $t0, tempVar (load word (int))
            //  sw $t0, var (load word (int))

            //registers required: 1
            int regIndex;

            regIndex = allocateRegister();
            if (regIndex == -1) {
                printf("Error: No available registers\n");
                return;
            }

            //  lw $t0, tempVar (load word (int))
            printf("\tlw %s, %s #LOAD [%s = %s %s %s]\n", tempRegisters[regIndex].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
            fprintf(outputFile, "\tlw %s, %s #LOAD [%s = %s %s %s]\n", tempRegisters[regIndex].name, current->arg1, current->result, current->arg1, current->op, current->arg2);
            //  sw $t0, var (load word (int))
            printf("\tsw %s, %s\n", tempRegisters[regIndex].name, current->result);
            fprintf(outputFile, "\tsw %s, %s\n", tempRegisters[regIndex].name, current->result);
            
            deallocateRegister(regIndex);
        }
        // Add more cases for other operators (*, /) and conditional jumps if necessary

        current = current->next;
    }

    // Exit the program
    fprintf(outputFile, "\tli $v0, 10\n\tsyscall\n");
}

void declareMipsVars(const SymbolTable* table)
{
    //Workaround: TABLE_SIZE can't be used here. Maybe declare it in SymbolTable?
    for (int i = 0; i < 100; i++) //TABLE_SIZE = 100
    {
        Symbol* current = table->table[i];
        while (current)
        {
            fprintf(outputFile, "\t%s: .word 0\n", current->name);
            current = current->next;
        }
    }
    //Temp vars
    //  TODO: Calculate a maximum for tempvars before this runs
    //      "i < 10" is just a placeholder value right now
    for (int i = 0; i < getTempVarCount(); i++) //tempVarCount defined in semantic.h... Will need to make this cleaner later
    // for (int i = 0; i < 10; i++) //tempVarCount defined in semantic.h... Will need to make this cleaner later
    {
        fprintf(outputFile, "\tt%d: .word 0\n", i);
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
int allocateRegister() {
    for (int i = 0; i < NUM_TEMP_REGISTERS; i++) {
        if (!tempRegisters[i].inUse) {
            tempRegisters[i].inUse = true;
            return i; // Return the register index
        }
    }
    return -1; // Indicate failure (no available register)
}

// Deallocate a register (mark it as free)
void deallocateRegister(int regIndex) {
    if (regIndex >= 0 && regIndex < NUM_TEMP_REGISTERS) {
        tempRegisters[regIndex].inUse = false;
    }
}




