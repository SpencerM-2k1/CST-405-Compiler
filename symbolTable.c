#include "symbolTable.h"
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
            free(sym->type);
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
void addSymbol(SymbolTable* table, const char* name, const char* type) {
    if (lookupSymbolInCurrentScope(table, name)) {
        printf("Error: Symbol '%s' already exists in the current scope.\n", name);
        return;
    }

    unsigned int hashval = hash(name, TABLE_SIZE);
    Symbol* newSymbol = (Symbol*)malloc(sizeof(Symbol));
    newSymbol->name = strdup(name);
    newSymbol->type = strdup(type);
    newSymbol->scopeLevel = table->currentScope;
    newSymbol->next = table->table[hashval];
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
            printf("Name: %s, Type: %s, Scope Level: %d\n", sym->name, sym->type, sym->scopeLevel);
            sym = sym->next;
        }
    }
}
