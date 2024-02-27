%{
#include <stdio.h>
%}

digit [0-9]

%%
[digit]?     printf("Hello, World!\n");
"world"     printf("Lex and Yacc\n");
.           printf("Unknown input\n");
%%

int main() {
    yylex();
    return 0;
}

int yywrap() {
    return 1; // indicate end of input
}
