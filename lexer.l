%{
#include "parser.tab.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int words = 0;
int chars = 0;
int lines = 1;

bool errorFound = false;
%}

%%

"int"               { yylval.sval = strdup(yytext); return TYPE; }
"char"              { yylval.sval = strdup(yytext); return TYPE; }
"write"             { return WRITE; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.sval = strdup(yytext); return ID; }
[0-9]+              { yylval.intVal = atoi(strdup(yytext)); return NUMBER; } //atoi: conversion to int
"+"                 { yylval.sval = strdup(yytext); return PLUS; }
"-"                 { yylval.sval = strdup(yytext); return MINUS; }
"="                 { yylval.sval = strdup(yytext); return ASSIGN; }
";"                 { return SEMI; }
[ \t\r]+            { /* Skip whitespace */ }
[\n]                { lines++; }
.                   { printf("Unrecognized symbol '%s' at line %d\n", yytext, lines); errorFound = true; }

%%

int yywrap(void) { return 1; }

