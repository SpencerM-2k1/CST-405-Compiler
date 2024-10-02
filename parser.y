%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AST.h"
#include "symbolTable.h"
#include "semantic.h"
#include "codeGenerator.h"
#include "optimizer.h"

#define TABLE_SIZE 100
#define MAX_ID_LENGTH 10
#define VAR_SUFFIX_LENGTH 4

bool drawVertical[100] = { false }; // Adjust size as needed

extern int yylex();   // Declare yylex, the lexer function
extern int yyparse(); // Declare yyparse, the parser function
extern FILE* yyin;    // Declare yyin, the file pointer for the input file
extern int yylineno;  // Declare yylineno, the line number counter
extern TAC* tacHead;  // Declare the head of the linked list of TAC entries

void yyerror(const char* s);
// void getMipsVarName(char* returnBuffer, unsigned int bufferSize, char* varName);
char* getMipsVarName(char* varName);

ASTNode* root = NULL; 
SymbolTable* symTab = NULL;
Symbol* symbol = NULL;

%}

%union {
	char* sval;
	int intVal;
	struct ASTNode* ast;
}

%token <sval> TYPE
%token <sval> ID
%token <sval> SEMI
%token <sval> ASSIGN
%token <sval> PLUS
%token <sval> MINUS
%token <intVal> NUMBER
%token <sval> WRITE

%printer { fprintf(yyoutput, "%s", $$); } ID;

%type <ast> Program VarDecl VarDeclList Stmt StmtList Expr BinOp Term
%start Program

%%

Program: VarDeclList StmtList    { printf("The PARSER has started\n"); 
									root = createNode(NodeType_Program);
									root->data.program.varDeclList = $1;
									root->data.program.stmtList = $2;
									// Set other fields as necessary
								}

;

VarDeclList:  {/*empty, i.e. it is possible not to declare a variable*/
				$$ = createNode(NodeType_VarDeclList); //Empty ASTNode prevents Unknown Node Error
				}
	| VarDecl VarDeclList {  printf("PARSER: Recognized variable declaration list\n"); 

							// Create AST node for VarDeclList
							$$ = createNode(NodeType_VarDeclList);
							$$->data.varDeclList.varDecl = $1;
							$$->data.varDeclList.varDeclList = $2;
				
							// Set other fields as necessary

							
							}
;

VarDecl: TYPE ID SEMI { printf("PARSER: Recognized variable declaration: %s\n", $2);

								printf("PARSER: Printing symbol table\n");
								printSymbolTable(symTab);

								printf("PARSER: Checking if variable has already been declared\n");

								//First, append _var to the end of the variable name
								//This will prevent conflicts with reserved instruction names in MIPS (e.g. "b")
								// char varName[MAX_ID_LENGTH];
								char* varName = getMipsVarName($2);
								printf("Name with suffix: %s\n", varName);
								
								
								// Check if variable has already been declared
								
								// symbol = lookupSymbol(symTab, $2);
								symbol = lookupSymbol(symTab, varName);
							
								if (symbol != NULL) {	// Check if variable has already been declared
									printf("PARSER: Variable %s at line %d has already been declared - COMPILATION HALTED\n", $2, yylineno);
									exit(0);
								} else {	
										// Variable has not been declared yet	
										// Create AST node for VarDecl

										$$ = createNode(NodeType_VarDecl);
										$$->data.varDecl.varType = strdup($1);
										// $$->data.varDecl.varName = strdup($2);
										$$->data.varDecl.varName = strdup(varName);
										// Set other fields as necessary

										// Add variable to symbol table
										// addSymbol(symTab, $2, $1);
										addSymbol(symTab, varName, $1);
										printSymbolTable(symTab);
									}
								
							 }
		| TYPE ID {
                  printf ("Missing semicolon after declaring variable: %s\n", $2);
             }

StmtList:  {/*empty, i.e. it is possible not to have any statement*/
			$$ = createNode(NodeType_StmtList); //Empty ASTNode prevents Unknown Node Error
			}
	| Stmt StmtList { printf("PARSER: Recognized statement list\n");
						$$ = createNode(NodeType_StmtList);
						$$->data.stmtList.stmt = $1;
						$$->data.stmtList.stmtList = $2;
						// Set other fields as necessary
					}
;

Stmt: ID ASSIGN Expr SEMI { /* code TBD */
								printf("PARSER: Recognized assignment statement\n");
								$$ = createNode(NodeType_AssignStmt);
								char* varName = getMipsVarName($1);

								$$->data.assignStmt.varName = strdup(varName);
								$$->data.assignStmt.operator = strdup($2);
								$$->data.assignStmt.expr = $3;
								// Set other fields as necessary
 }
	| WRITE ID SEMI { printf("PARSER: Recognized write statement\n"); 
							$$ = createNode(NodeType_WriteStmt);
							
							//Append _var to the end of the variable name
							// char varName[MAX_ID_LENGTH];
							char* varName = getMipsVarName($2);
								
							// $$->data.writeStmt.varName = strdup($2);
							$$->data.writeStmt.varName = strdup(varName);
					}
;

Expr: Expr BinOp Term { printf("PARSER: Recognized expression\n");
						$$ = createNode(NodeType_Expr);
						$$->data.expr.left = $1;
						$$->data.expr.right = $3;
						$$->data.expr.operator = strdup($2->data.binOp.operator);
						
						// Set other fields as necessary
					  }
 					
	| Term {/*No code necessary, this just makes Expr left-recursive*/}
;

Term: ID { printf("ASSIGNMENT statement \n"); 
			$$ = malloc(sizeof(ASTNode));
			$$->type = NodeType_SimpleID;

			//Append _var to the end of the variable name
			// char varName[MAX_ID_LENGTH];
			char* varName = getMipsVarName($1);

			// $$->data.simpleID.name = $1;
			$$->data.simpleID.name = varName;
			// Set other fields as necessary	
		}
	| NUMBER { 
				printf("PARSER: Recognized number\n");
				$$ = malloc(sizeof(ASTNode));
				$$->type = NodeType_SimpleExpr;
				$$->data.simpleExpr.number = $1;
				// Set other fields as necessary
			 }

BinOp: PLUS {
				printf("PARSER: Recognized binary operator\n");
				$$ = malloc(sizeof(ASTNode));
				$$->type = NodeType_BinOp;
				// $$->data.binOp.operator = *$1;
				$$->data.binOp.operator = $1;
				// printf(" - PARSER: %s\n", $$->data.binOp.operator);
				// Set other fields as necessary
            }
	| MINUS {
				printf("PARSER: Recognized binary operator\n");
				$$ = createNode(NodeType_BinOp);
				// $$->data.binOp.operator = *$1;
				$$->data.binOp.operator = $1;
				// printf(" - PARSER: %s\n", $$->data.binOp.operator);
				// Set other fields as necessary
            }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main(int argc, char **argv) {
    ++argv, --argc;  /* Skip over program name */
    if (argc > 0)
        yyin = fopen(argv[0], "r");
    else
        yyin = stdin;
	
	// Initialize symbol table
	symTab = createSymbolTable(TABLE_SIZE);
    if (symTab == NULL) {
        // Handle error
        return EXIT_FAILURE;
    }

	/* initializeTempVars(); */

    if (yyparse() == 0)
	{
        // Successfully parsed
		printf("Parsing successful!\n");
        traverseAST(root, 0, drawVertical, false);
        /* traverseAST(root, 0); */
		// Print symbol table for debugging
		printSymbolTable(symTab);
		// Semantic analysis
		printf("\n=== SEMANTIC ANALYSIS ===\n\n");
		semanticAnalysis(root, symTab);
		printf("\n=== TAC GENERATION ===\n");
		printTACToFile("output/TAC.ir", tacHead);

		// Code optimization
		printf("\n=== CODE OPTIMIZATION ===\n");
		// Traverse the linked list of TAC entries and optimize
		// But - you MIGHT need to traverse the AST again to optimize

		optimizeTAC(tacHead);
		printOptimizedTAC("output/TACOptimized.ir", tacHead);

		// Code generation
		printf("\n=== CODE GENERATION ===\n");
		initCodeGenerator("output/output.asm", symTab);
		generateMIPS(tacHead);
		finalizeCodeGenerator("output/output.asm");

        freeAST(root);
		freeSymbolTable(symTab);
	}
    else
        printf("Parsing failed.\n");

    return 0;
}

//Append _var to name of variable
//This will prevent conflicts with reserved instruction names in MIPS (e.g. "b")
/* void getMipsVarName(char* returnBuffer, unsigned int bufferSize, char* varName)
{
	if ((strlen(varName) + VAR_SUFFIX_LENGTH) < bufferSize)
	{
		strcpy(returnBuffer, varName);
		strcat(returnBuffer, "_var");
	}
	else
	{
		fprintf(stderr, "Error: ID length too long (%s)\n", varName);
		exit(1);
	}
} */
char* getMipsVarName(char* varName) {
    int idLength = snprintf(NULL, 0, "%s%s", varName, "_var"); //Get length of const sum so we can allocate appropriate string length
	char* idWithSuffix = malloc(idLength + 1);
    snprintf(idWithSuffix, idLength + 1, "%s%s", varName, "_var"); //Get length of const sum so we can allocate appropriate string length
	return idWithSuffix;
}
