#!/bin/bash
lex JJAL363.l
gcc -o lexer lex.yy.c -ll
if ./lexer < $1; then
    rm lex.yy.c lexer
fi