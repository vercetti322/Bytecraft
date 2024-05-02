%{
#include "y.tab.h"
%}

%option case-insensitive

%%

[ \t]+               ;  // oversee white spaces
\n                   { yylineno++; }  // Increment line counter on new lines
"//"[^"\n"]*         ;  // Ignore single line comments
\"[^\"\n]*\"     { return STRING_LITERAL; }
"program"            { return PROGRAM; }
"begin"              { return BEGIN_KEYWORD; }
"end"                { return END; }
"var"                { return VAR; }
"integer"            { return INTEGER_KEYWORD; }
"real"               { return REAL_KEYWORD; }
"char"               { return CHAR_KEYWORD; }
"boolean"            { return BOOLEAN_KEYWORD; }
"if"                 { return IF; }
"then"               { return THEN; }
"else"               { return ELSE; }
"while"              { return WHILE; }
"do"                 { return DO; }
"for"                { return FOR; }
"to"                 { return TO; }
"downto"             { return DOWNTO; }
"read"               { return READ; }
"write"              { return WRITE; }
"array"              { return ARRAY_KEYWORD; }
"of"                 { return OF; }
":="                 { return ASSIGN; }
[0-9]+\.\.[0-9]+   { return ARRAY_RANGE; }
[a-zA-Z_][a-zA-Z0-9_]*  { return IDENTIFIER; }
[0-9]+              { return INTEGER; }
[0-9]+"."[0-9]*     { return REAL; }
"+"                 { return ADD; }
"-"                 { return SUBTRACT; }
"*"                 { return MULTIPLY; }
"/"                 { return DIVIDE; }
"%"                 { return MODULO; }
"="                 { return EQUALS; }
"<>"                { return NOTEQUAL; }
"<"                 { return LESS; }
">"                 { return GREATER; }
"<="                { return LESSEQUAL; }
">="                { return GREATEREQUAL; }
";"                 { return SEMICOLON; }
","                 { return COMMA; }
":"                 { return COLON; }
"("                 { return LPAREN; }
")"                 { return RPAREN; }
"."                 { return PERIOD; }
"["                 { return LBRAC; }
"]"                 { return RBRAC; }

%%

int yywrap() { return 1; }
