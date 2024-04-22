#!/bin/bash
lex tokenizer.l
yacc -d -t parser.y
gcc -c lex.yy.c -o lex.yy.o
gcc -c y.tab.c -o y.tab.o
gcc lex.yy.o y.tab.o -o parser -ll
./parser < sample_prog.txt