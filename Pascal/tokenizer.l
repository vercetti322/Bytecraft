%{
#include "y.tab.h"
%}

%option case-insensitive

%%

[ \t]+               ;  // oversee white spaces
\n                   { yylineno++; }  // Increment line counter on new lines

"//"[^"\n"]*         ;  // Ignore single line comments

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
"read"               { return READ; }
"write"              { return WRITE; }
":="                 { return ASSIGN; }
"\"[^\n\"]*\""    { return STRING_LITERAL; }
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

%%

int yywrap() { return 1; }