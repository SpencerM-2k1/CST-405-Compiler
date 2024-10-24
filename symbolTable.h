#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "commons/types.h"
#include <stdbool.h>

#define TABLE_SIZE 100  // Adjust size as needed

// Define the structure for a symbol
typedef struct Symbol {
    char* name;
    VarType type;
    int scopeLevel;
    struct Symbol* next;  // For linked list of symbols in case of hash collisions

    //Array fields, ignored for non-array symbols
    bool isArray; //If false, array fields are ignored
    int arrSize; //Length of array. Note: arrays are one-dimensional

} Symbol;

// Define the structure for the symbol table
typedef struct SymbolTable {
    Symbol** table;  // Array of pointers to symbols (hash table)
    int currentScope;  // To track scope levels
} SymbolTable;

// Function prototypes
SymbolTable* createSymbolTable(int size);
void initSymbolTable(SymbolTable* symTab);
void freeSymbolTable(SymbolTable* symTab);
void enterScope(SymbolTable* table);
void exitScope(SymbolTable* table);
void addSymbol(SymbolTable* symTab, const char* varName, const char* typeString);
void addArrSymbol(SymbolTable* symTab, const char* varName, const char* typeString, int size);
Symbol* lookupSymbol(SymbolTable* symTab, const char* varName);
Symbol* lookupSymbolInCurrentScope(SymbolTable* symTab, const char* varName);
void printSymbolTable(SymbolTable* symTab);

#endif
