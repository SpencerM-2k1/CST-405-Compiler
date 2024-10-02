#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define TABLE_SIZE 100  // Adjust size as needed

// Define the structure for a symbol
typedef struct Symbol {
    char* name;
    char* type;
    int scopeLevel;
    struct Symbol* next;  // For linked list of symbols in case of hash collisions
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
void addSymbol(SymbolTable* symTab, const char* varName, const char* varType);
Symbol* lookupSymbol(SymbolTable* symTab, const char* varName);
Symbol* lookupSymbolInCurrentScope(SymbolTable* symTab, const char* varName);
void printSymbolTable(SymbolTable* symTab);

#endif
