%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <ctype.h>
    #include <string.h>

    int courseCompleted = 0;
    char student[31];

    // Define FILE pointers
    FILE *inputFile;
    FILE *outputFile;

    // Define function prototypes
    int yywrap(void);
    void yyerror(const char *s);

%}

%x STUD_ID
%x COURSE

%%
[^$]+ { /* Ignore input before $ with spaces */ }

[$][ ]* {
    BEGIN(STUD_ID);
}

<STUD_ID>[A-Z]+[0-9]+ {
    if (yyleng > 30)
    {
        fprintf(outputFile, "wrong student id\n");
        BEGIN(INITIAL);
    }
    else
    {
        char id[31];
        strncpy(id, yytext, yyleng);
        strncpy(student, yytext, yyleng);
        id[yyleng] = '\0';
        student[yyleng] = '\0';
        fprintf(outputFile, "student id: %s\n", id);
        BEGIN(COURSE);
    }
}

<STUD_ID>.|\n {
    fprintf(outputFile, "wrong student id\n");
    BEGIN(INITIAL);
}

<COURSE>[\/][\/] {
    fprintf(outputFile, "comment!\n");
    BEGIN(INITIAL);
}

<COURSE>[ ]*[<][ ]*[A-Z]+[0-9]+[ ]+[2-4][ ]+[A|B][-]?[ ]*[>][ ]*|[ ]*[<][ ]*[A-Z]+[0-9]+[ ]+[2-4][ ]+[C][ ]*[>][ ]* {
    int j;
    for (int i = 0; i < yyleng; i++)
    {
        if (isupper(yytext[i]))
        {
            j = i;
            break;
        }
    }

    int len = 0;
    char courseID[31];
    int k = 0;
    while (isdigit(yytext[j]) || isupper(yytext[j])) {len++; courseID[k++] = yytext[j++];}
    courseID[k] = '\0';

    if (len > 30)
    {
        fprintf(outputFile, "wrong course id!\n");
        exit(1);
    }

    while (yytext[j] == ' ') {j++;}

    int cred;
    char grad[3];
    for (int i = j; i < yyleng; i++)
    {
        if (yytext[i] == '2' || yytext[i] == '3' || yytext[i] == '4')
        {
            cred = atoi(&yytext[i]);
        }
        else if (yytext[i] == 'A' || yytext[i] == 'B' || yytext[i] == 'C')
        {
            if (yytext[i + 1] == '-')
            {
                grad[0] = yytext[i];
                grad[1] = yytext[i + 1];
                grad[2] = '\0';
            }
            else 
            {
                grad[0] = yytext[i];
                grad[1] = '\0';
            }
        }
    }

    fprintf(outputFile, "course id: %s, credits: %d, grade: %s\n", courseID, cred, grad);
    BEGIN(COURSE);
    courseCompleted = 1;
}

<COURSE>[$] {
    unput('$');
    BEGIN(INITIAL);
}

<COURSE>[\n] {
    if (courseCompleted) 
    {
        fprintf(outputFile, "record parsed!\n");
        BEGIN(INITIAL);
    }

    courseCompleted = 0;
    student[0] = '\0';
}
%%

int main()
{
    // Open input file for reading
    inputFile = fopen("input.txt", "r");
    if (inputFile == NULL) 
    {
        perror("Error opening input file");
        return 1;
    }

    // Open output file for writing
    outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) 
    {
        perror("Error opening output file");
        fclose(inputFile);
        return 1;
    }

    // Redirect input and output
    yyin = inputFile;
    yyout = outputFile;

    // Run the lexer
    yylex();

    // Close files
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}

int yywrap()
{
    return 1;
}
