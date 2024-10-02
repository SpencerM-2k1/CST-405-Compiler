#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include "AST.h" // Include your AST definition
#include "semantic.h" // Include your TAC definition
#include <stdbool.h>

#define NUM_TEMP_REGISTERS 10

// MIPSRegister struct definition should go here
typedef struct {
    char* name;  // Name of the register, e.g., "$t0"
    bool inUse;  // Whether the register is currently in use

    //Temp vars are directly mapped to registers
    
    //Each `MappedTemps` struct includes a list of temp vars mapped
    //to that specific register
    // struct MappedTemps{
    //     char* tempIDs[10];
    //     int cursor;
    // }mappedTemps;
} MIPSRegister;  // Define MIPSRegister in header only once



// Function prototypes
// void initCodeGenerator(const char* outputFilename);
void initCodeGenerator(const char* outputFilename, const SymbolTable* table); //SymbolTable required for MIPS header
void generateMIPS(TAC* tacInstructions);
void declareMipsVars(const SymbolTable* table);
void finalizeCodeGenerator(const char* outputFilename);
int allocateRegister();
void deallocateRegister(int regIndex);

//MappedTemps management
void mapTemp(MIPSRegister** reg, char* tempID);

#endif // CODE_GENERATOR_H

