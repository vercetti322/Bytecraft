%{
#include <stdio.h>
%}

%%
"hello"     printf("Hello, World!\n");
"world"     printf("Lex and Yacc\n");
.           printf("Unknown input\n");
%%

int main() {
    yylex();
    return 0;
}
