%{
    #include <stdio.h>
%}
%x IN_COMMENT

%%
"/*" {BEGIN(IN_COMMENT);}
<IN_COMMENT>"*/" {BEGIN(INITIAL);}
<IN_COMMENT>[^\n]* {}
"//" {while (yyin && (yylex() != '\n'));}
.|\n {fprintf(yyout, "%s", yytext);}
%%

int main(int argc, char *argv[]) 
{
    if (argc != 3)
    {
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (!input)
    {
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (!output)
    {
        fclose(input);
        return 1;
    }

    yyin = input;
    yyout = output;

    yylex();

    fclose(input);
    fclose(output);

    return 0;
}

int yywrap()
{
    return 1;
}