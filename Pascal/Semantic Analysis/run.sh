#!/bin/bash
lex tokenizer.l
yacc -d -t parser.y
gcc -c lex.yy.c -o lex.yy.o
gcc -c y.tab.c -o y.tab.o
gcc lex.yy.o y.tab.o -o parser -ll
output=$(./parser 2>&1)

if echo "$output" | grep -q 'Error'; then
    echo "$output" | awk '/Error/{flag=1} flag'
else
    echo 'CODE PARSED!!!'
fi
