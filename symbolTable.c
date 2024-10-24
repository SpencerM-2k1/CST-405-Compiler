#include "symbolTable.h"
#include "commons/types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Hash function for symbol table (simple mod hash)
unsigned int hash(const char* str, int tableSize) {
    unsigned int hashval = 0;
    while (*str != '\0') {
        hashval = (hashval << 5) + *str++;  // Left shift and add char value
    }
    return hashval % tableSize;
}

// Create a new symbol table
SymbolTable* createSymbolTable(int size) {
    SymbolTable* newTable = (SymbolTable*)malloc(sizeof(SymbolTable));
    if (!newTable) {
        perror("Failed to create symbol table");
        exit(EXIT_FAILURE);
    }
    newTable->table = (Symbol**)malloc(sizeof(Symbol*) * size);
    if (!newTable->table) {
        perror("Failed to allocate memory for symbol table");
        free(newTable);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++) {
        newTable->table[i] = NULL;
    }
    newTable->currentScope = 0;  // Global scope starts at 0
    return newTable;
}

// Initialize the symbol table with initial capacity
void initSymbolTable(SymbolTable* symTab) {
    symTab->table = (Symbol**)malloc(sizeof(Symbol*) * TABLE_SIZE);
    if (!symTab->table) {
        perror("Failed to allocate memory for symbol table");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        symTab->table[i] = NULL;
    }
    symTab->currentScope = 0;
}

// Free the memory for the symbol table
void freeSymbolTable(SymbolTable* symTab) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Symbol* sym = symTab->table[i];
        while (sym) {
            Symbol* nextSym = sym->next;
            free(sym->name);
            // free(sym->type);
            free(sym);
            sym = nextSym;
        }
    }
    free(symTab->table);
    free(symTab);
}

// Enter a new scope
void enterScope(SymbolTable* table) {
    table->currentScope++;
}

// Exit the current scope
void exitScope(SymbolTable* table) {
    table->currentScope--;
}

// Add a symbol to the symbol table
void addSymbol(SymbolTable* table, const char* name, const char* typeString) {
    if (lookupSymbolInCurrentScope(table, name)) {
        printf("Error: Symbol '%s' already exists in the current scope.\n", name);
        return;
    }

    VarType type;
    if (strcmp(typeString, "int") == 0) {
        type = VarType_Int;
    } else if (strcmp(typeString, "float") == 0) {
        type = VarType_Float;
    } else {
        printf("ERROR: VarType not recognized. Halting compilation...");
        exit(1);
    }

    unsigned int hashval = hash(name, TABLE_SIZE);
    Symbol* newSymbol = (Symbol*)malloc(sizeof(Symbol));
    newSymbol->name = strdup(name);
    printf("newSymbol->name: %s\n",newSymbol->name);
    newSymbol->type = type;
    newSymbol->scopeLevel = table->currentScope;
    newSymbol->next = table->table[hashval];

    //Array elements are unused
    newSymbol->isArray = false;
    newSymbol->arrSize = 0;

    table->table[hashval] = newSymbol;
}

// Add an array symbol to the symbol table
void addArrSymbol(SymbolTable* table, const char* name, const char* typeString, int size) {
    if (size <= 0) {
        printf("ERROR: Array size must be greater than zero.");
        exit(1);
    }
    if (lookupSymbolInCurrentScope(table, name)) {
        printf("Error: Symbol '%s' already exists in the current scope.\n", name);
        return;
    }

    VarType type;
    if (strcmp(typeString, "int") == 0) {
        type = VarType_Int;
    } else if (strcmp(typeString, "float") == 0) {
        type = VarType_Float;
    } else {
        printf("ERROR: VarType not recognized. Halting compilation...");
        exit(1);
    }

    unsigned int hashval = hash(name, TABLE_SIZE);
    Symbol* newSymbol = (Symbol*)malloc(sizeof(Symbol));
    newSymbol->name = strdup(name);
    printf("newSymbol->name: %s\n",newSymbol->name);
    newSymbol->type = type;
    newSymbol->scopeLevel = table->currentScope;
    newSymbol->next = table->table[hashval];

    //Set array elements
    newSymbol->isArray = true;
    newSymbol->arrSize = size;

    table->table[hashval] = newSymbol;
}

// Lookup a symbol in the symbol table
Symbol* lookupSymbol(SymbolTable* table, const char* name) {
    unsigned int hashval = hash(name, TABLE_SIZE);
    for (Symbol* sym = table->table[hashval]; sym != NULL; sym = sym->next) {
        if (strcmp(name, sym->name) == 0) {
            return sym;
        }
    }
    return NULL;  // Symbol not found
}

// Lookup a symbol in the current scope only
Symbol* lookupSymbolInCurrentScope(SymbolTable* table, const char* name) {
    unsigned int hashval = hash(name, TABLE_SIZE);
    for (Symbol* sym = table->table[hashval]; sym != NULL; sym = sym->next) {
        if (strcmp(name, sym->name) == 0 && sym->scopeLevel == table->currentScope) {
            return sym;
        }
    }
    return NULL;
}

// Print the symbol table contents
void printSymbolTable(SymbolTable* table) {
    printf("Symbol Table:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        Symbol* sym = table->table[i];
        while (sym != NULL) {
            if (!sym->isArray) { //Non-array
                printf("Name: %s, Type: %s, Scope Level: %d\n", sym->name, varTypeToString(sym->type), sym->scopeLevel);
            } else { //Array
                printf("Name: %s, Type: %s[%d], Scope Level: %d\n", sym->name, varTypeToString(sym->type), sym->arrSize, sym->scopeLevel);
            }
            sym = sym->next;
        }
    }
}
