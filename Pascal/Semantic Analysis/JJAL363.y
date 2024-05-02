%{
#include <stdio.h>
extern int yylex();
extern int yylineno;
void yyerror(char *s);
extern int yydebug;
%}


%token INTEGER REAL CHAR BOOLEAN KEYWORD IDENTIFIER ADD SUBTRACT MULTIPLY DIVIDE MODULO EQUALS NOTEQUAL LESS GREATER LESSEQUAL GREATEREQUAL SEMICOLON COMMA COLON LPAREN RPAREN STRING_LITERAL
%token PROGRAM BEGIN_KEYWORD END VAR INTEGER_KEYWORD REAL_KEYWORD CHAR_KEYWORD BOOLEAN_KEYWORD IF THEN ELSE WHILE DO FOR TO READ WRITE ASSIGN PERIOD DOWNTO OF ARRAY_KEYWORD LBRAC RBRAC
%token ARRAY_RANGE
%%

program:
    PROGRAM IDENTIFIER SEMICOLON declarations BEGIN_KEYWORD statements END PERIOD
    ;

declarations:
    | VAR declaration_list
    ;

declaration_list:
    declaration SEMICOLON
    | declaration_list declaration SEMICOLON
    ;

declaration:
    IDENTIFIER_LIST COLON type
    ;

IDENTIFIER_LIST:
    IDENTIFIER 
    | IDENTIFIER_LIST COMMA IDENTIFIER
    ;

type:
    INTEGER_KEYWORD
    | REAL_KEYWORD
    | CHAR_KEYWORD
    | BOOLEAN_KEYWORD
    | ARRAY_KEYWORD LBRAC ARRAY_RANGE RBRAC OF type
    ;

statements:
    statement 
    | statements statement
    ;

statement:
    assignment_statement
    | conditional_statement
    | loop_statement
    | write_statement
    | read_statement
    ;

assignment_statement:
    IDENTIFIER ASSIGN expression SEMICOLON
    | array_access ASSIGN expression SEMICOLON
    ;

expression:
    term
    | expression ADD term
    | expression SUBTRACT term
    ;

term:
    factor
    | term MULTIPLY factor
    | term DIVIDE factor
    | term MODULO factor
    ;

factor:
    INTEGER
    | REAL
    | IDENTIFIER
    | LPAREN expression RPAREN
    | array_access
    ;

array_access:
    IDENTIFIER LBRAC expression RBRAC
    ;

conditional_statement:
    IF condition THEN BEGIN_KEYWORD statements END SEMICOLON
    | IF condition THEN BEGIN_KEYWORD statements END ELSE BEGIN_KEYWORD statements END SEMICOLON
    ;

condition:
    expression EQUALS expression
    | expression NOTEQUAL expression
    | expression LESS expression
    | expression GREATER expression
    | expression LESSEQUAL expression
    | expression GREATEREQUAL expression
    | IDENTIFIER
    ;

loop_statement:
    WHILE condition DO BEGIN_KEYWORD statements END SEMICOLON
    | FOR IDENTIFIER ASSIGN expression TO expression DO BEGIN_KEYWORD statements END SEMICOLON
    | FOR IDENTIFIER ASSIGN expression DOWNTO expression DO BEGIN_KEYWORD statements END SEMICOLON
    ;

write_statement: 
    WRITE LPAREN write_argument RPAREN SEMICOLON
    ;

write_argument:
    STRING_LITERAL
    | IDENTIFIER_LIST
    | array_access
    ;

read_statement:
    READ LPAREN IDENTIFIER RPAREN SEMICOLON
    | READ LPAREN array_access RPAREN SEMICOLON
    ;

%%

void yyerror(char *s) {
    fprintf(stderr, "Error: %s at line %d\n", s, yylineno);
}

int main() {
    yydebug = 1;
    return yyparse();
}
