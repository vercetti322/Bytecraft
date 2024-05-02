#!/bin/bash
lex JJAL363.l
yacc -d JJAL363.y
gcc -c lex.yy.c -o lex.yy.o
gcc -c y.tab.c -o y.tab.o
gcc lex.yy.o y.tab.o -o parser -ll

# Use the first argument as the input file
output=$(./parser 2>&1)

if echo "$output" | grep -q 'Error'; then
    echo 'SYNTAX ERROR!!!'
else
    echo 'VALID INPUT!!!'
fi

# Remove output files
rm lex.yy.c lex.yy.o y.tab.c y.tab.o y.tab.h parser