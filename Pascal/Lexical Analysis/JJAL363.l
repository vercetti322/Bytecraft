%{
#include <stdio.h>
#include "tokens.h"  // Include the tokens header file
%}

%option case-insensitive

%%

[ \t]+               ;  // Oversee white spaces
\n                   { yylineno++; }  // Increment line counter on new lines

"//"[^"\n"]*         ;  // Ignore single line comments

"program"            { printf("%d\t\tprogram\t\tPROGRAM\n", yylineno); return PROGRAM; }
"begin"              { printf("%d\t\tbegin\t\tBEGIN_KEYWORD\n", yylineno); return BEGIN_KEYWORD; }
"end"                { printf("%d\t\tend\t\tEND\n", yylineno); return END; }
"var"                { printf("%d\t\tvar\t\tVAR\n", yylineno); return VAR; }
"integer"            { printf("%d\t\tinteger\t\tINTEGER_KEYWORD\n", yylineno); return INTEGER_KEYWORD; }
"real"               { printf("%d\t\treal\t\tREAL_KEYWORD\n", yylineno); return REAL_KEYWORD; }
"char"               { printf("%d\t\tchar\t\tCHAR_KEYWORD\n", yylineno); return CHAR_KEYWORD; }
"boolean"            { printf("%d\t\tboolean\t\tBOOLEAN_KEYWORD\n", yylineno); return BOOLEAN_KEYWORD; }
"if"                 { printf("%d\t\tif\t\tIF\n", yylineno); return IF; }
"then"               { printf("%d\t\tthen\t\tTHEN\n", yylineno); return THEN; }
"else"               { printf("%d\t\telse\t\tELSE\n", yylineno); return ELSE; }
"while"              { printf("%d\t\twhile\t\tWHILE\n", yylineno); return WHILE; }
"do"                 { printf("%d\t\tdo\t\tDO\n", yylineno); return DO; }
"for"                { printf("%d\t\tfor\t\tFOR\n", yylineno); return FOR; }
"to"                 { printf("%d\t\tto\t\tTO\n", yylineno); return TO; }
"read"               { printf("%d\t\tread\t\tREAD\n", yylineno); return READ; }
"write"              { printf("%d\t\twrite\t\tWRITE\n", yylineno); return WRITE; }
":="                 { printf("%d\t\t:=\t\tASSIGN\n", yylineno); return ASSIGN; }
"\"[^\n\"]*\""       { printf("%d\t\t%s\t\tSTRING_LITERAL\n", yylineno, yytext); return STRING_LITERAL; }
[a-zA-Z_][a-zA-Z0-9_]*  { printf("%d\t\t%s\t\tIDENTIFIER\n", yylineno, yytext); return IDENTIFIER; }
[0-9]+               { printf("%d\t\t%s\t\tINTEGER\n", yylineno, yytext); return INTEGER; }
[0-9]+"."[0-9]*      { printf("%d\t\t%s\t\tREAL\n", yylineno, yytext); return REAL; }
"+"                  { printf("%d\t\t+\t\tADD\n", yylineno); return ADD; }
"-"                  { printf("%d\t\t-\t\tSUBTRACT\n", yylineno); return SUBTRACT; }
"*"                  { printf("%d\t\t*\t\tMULTIPLY\n", yylineno); return MULTIPLY; }
"/"                  { printf("%d\t\t/\t\tDIVIDE\n", yylineno); return DIVIDE; }
"%"                  { printf("%d\t\t%%\t\tMODULO\n", yylineno); return MODULO; }
"="                  { printf("%d\t\t=\t\tEQUALS\n", yylineno); return EQUALS; }
"<>"                 { printf("%d\t\t<>\t\tNOTEQUAL\n", yylineno); return NOTEQUAL; }
"<"                  { printf("%d\t\t<\t\tLESS\n", yylineno); return LESS; }
">"                  { printf("%d\t\t>\t\tGREATER\n", yylineno); return GREATER; }
"<="                 { printf("%d\t\t<=\t\tLESSEQUAL\n", yylineno); return LESSEQUAL; }
">="                 { printf("%d\t\t>=\t\tGREATEREQUAL\n", yylineno); return GREATEREQUAL; }
";"                  { printf("%d\t\t;\t\tSEMICOLON\n", yylineno); return SEMICOLON; }
","                  { printf("%d\t\t,\t\tCOMMA\n", yylineno); return COMMA; }
":"                  { printf("%d\t\t:\t\tCOLON\n", yylineno); return COLON; }
"("                  { printf("%d\t\t(\t\tLPAREN\n", yylineno); return LPAREN; }
")"                  { printf("%d\t\t)\t\tRPAREN\n", yylineno); return RPAREN; }
"."                  { printf("%d\t\t.\t\tPERIOD\n", yylineno); return PERIOD; }
"["                  { printf("%d\t\t.\t\tLBRAC\n", yylineno); return LBRAC; }
"]"                  { printf("%d\t\t.\t\tRBRAC\n", yylineno); return RBRAC; }
"\""                 { }
%%

int yywrap() { return 1; }