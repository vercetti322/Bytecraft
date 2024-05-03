/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    INTEGER = 258,                 /* INTEGER  */
    REAL = 259,                    /* REAL  */
    CHAR = 260,                    /* CHAR  */
    BOOLEAN = 261,                 /* BOOLEAN  */
    KEYWORD = 262,                 /* KEYWORD  */
    IDENTIFIER = 263,              /* IDENTIFIER  */
    ADD = 264,                     /* ADD  */
    SUBTRACT = 265,                /* SUBTRACT  */
    MULTIPLY = 266,                /* MULTIPLY  */
    DIVIDE = 267,                  /* DIVIDE  */
    MODULO = 268,                  /* MODULO  */
    EQUALS = 269,                  /* EQUALS  */
    NOTEQUAL = 270,                /* NOTEQUAL  */
    LESS = 271,                    /* LESS  */
    GREATER = 272,                 /* GREATER  */
    LESSEQUAL = 273,               /* LESSEQUAL  */
    GREATEREQUAL = 274,            /* GREATEREQUAL  */
    SEMICOLON = 275,               /* SEMICOLON  */
    COMMA = 276,                   /* COMMA  */
    COLON = 277,                   /* COLON  */
    LPAREN = 278,                  /* LPAREN  */
    RPAREN = 279,                  /* RPAREN  */
    STRING_LITERAL = 280,          /* STRING_LITERAL  */
    PROGRAM = 281,                 /* PROGRAM  */
    BEGIN_KEYWORD = 282,           /* BEGIN_KEYWORD  */
    END = 283,                     /* END  */
    VAR = 284,                     /* VAR  */
    INTEGER_KEYWORD = 285,         /* INTEGER_KEYWORD  */
    REAL_KEYWORD = 286,            /* REAL_KEYWORD  */
    CHAR_KEYWORD = 287,            /* CHAR_KEYWORD  */
    BOOLEAN_KEYWORD = 288,         /* BOOLEAN_KEYWORD  */
    IF = 289,                      /* IF  */
    THEN = 290,                    /* THEN  */
    ELSE = 291,                    /* ELSE  */
    WHILE = 292,                   /* WHILE  */
    DO = 293,                      /* DO  */
    FOR = 294,                     /* FOR  */
    TO = 295,                      /* TO  */
    READ = 296,                    /* READ  */
    WRITE = 297,                   /* WRITE  */
    ASSIGN = 298,                  /* ASSIGN  */
    PERIOD = 299,                  /* PERIOD  */
    DOWNTO = 300,                  /* DOWNTO  */
    OF = 301,                      /* OF  */
    ARRAY_KEYWORD = 302,           /* ARRAY_KEYWORD  */
    LBRAC = 303,                   /* LBRAC  */
    RBRAC = 304,                   /* RBRAC  */
    ARRAY_RANGE = 305              /* ARRAY_RANGE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define INTEGER 258
#define REAL 259
#define CHAR 260
#define BOOLEAN 261
#define KEYWORD 262
#define IDENTIFIER 263
#define ADD 264
#define SUBTRACT 265
#define MULTIPLY 266
#define DIVIDE 267
#define MODULO 268
#define EQUALS 269
#define NOTEQUAL 270
#define LESS 271
#define GREATER 272
#define LESSEQUAL 273
#define GREATEREQUAL 274
#define SEMICOLON 275
#define COMMA 276
#define COLON 277
#define LPAREN 278
#define RPAREN 279
#define STRING_LITERAL 280
#define PROGRAM 281
#define BEGIN_KEYWORD 282
#define END 283
#define VAR 284
#define INTEGER_KEYWORD 285
#define REAL_KEYWORD 286
#define CHAR_KEYWORD 287
#define BOOLEAN_KEYWORD 288
#define IF 289
#define THEN 290
#define ELSE 291
#define WHILE 292
#define DO 293
#define FOR 294
#define TO 295
#define READ 296
#define WRITE 297
#define ASSIGN 298
#define PERIOD 299
#define DOWNTO 300
#define OF 301
#define ARRAY_KEYWORD 302
#define LBRAC 303
#define RBRAC 304
#define ARRAY_RANGE 305

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
