%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <ctype.h>
    #include <string.h>

    typedef struct Record
    {
        char studID[30];
        char course[30][35];
    }Record;

    Record record;
    int total_count = 0;
    int upper_count = 0;
    int buffer_index = 0;
    int type = 0;
    int course_index = 0;
%}

%x UPPER
%x DIGIT
%x GREATER_THAN
%x LESSER_THAN
%x CREDS

%%
[^$]+ { /* Ignore input before $ with spaces */ }

[$][ ]* {
    BEGIN(UPPER);
    total_count = 0;
    upper_count = 0;
    buffer_index = 0;
    type = 0;
    course_index = 0;
}

<UPPER>[A-Z] {
    if (total_count < 30)
    {   
        if (type == 0)
            record.studID[buffer_index++] = yytext[0];
        else
        {
            record.course[course_index][buffer_index++] = yytext[0];
            printf("%c", yytext[0]);
        }

        total_count++;
        upper_count++;
    }

    else
    {
        printf("\nwrong ID!\n");
        BEGIN(INITIAL);
    }
}

<UPPER>[0-9] {
    if (upper_count > 0)
    {
        BEGIN(DIGIT);
        unput(yytext[0]);
    }

    else
    {
        printf("\nwrong ID!\n");
        BEGIN(INITIAL);
    }
}

<UPPER>.|\n {
    printf("\nwrong ID!\n");
    BEGIN(INITIAL);
}

<DIGIT>[A-Z] {
    printf("\nwrong ID!\n");
    BEGIN(INITIAL);
}

<DIGIT>[0-9] {
    if (total_count < 30) 
    {
        if (type == 0)
            record.studID[buffer_index++] = yytext[0];
        else
        {
            record.course[course_index][buffer_index++] = yytext[0];
            printf("%c", yytext[0]);
        }

        total_count++;
    } 
    
    else 
    {
        printf("\nwrong ID!\n");
        BEGIN(INITIAL);
    }
}

<DIGIT>[ ]* {
    if (type == 0)
    {
        record.studID[buffer_index] = '\0';
        BEGIN(GREATER_THAN);
    }
        
    else
    {
        BEGIN(CREDS);
    }
}

<GREATER_THAN>[<][ ]* {
    BEGIN(UPPER);
    type = 1;
}

<GREATER_THAN>. {
    printf("\nno course info found!\n");
    BEGIN(INITIAL);
}

<CREDS>[2-4][ ](A-?|B-?|C) {
    record.course[course_index][buffer_index++] = ' ';
    for (int i = 0; i < yyleng; ++i) {
        record.course[course_index][buffer_index++] = yytext[i];
        printf("%c", yytext[i]); 
    }

    ++course_index;
    BEGIN(LESSER_THAN);
}

<CREDS>. {
    printf("\nwrong course info found!\n");
    BEGIN(INITIAL);
}

<LESSER_THAN>[ ]*[>] {
    printf("\ncompleted record\n");
    BEGIN(INITIAL);
}
%%

int main()
{
    yylex();

    printf("record ID: %s", record.studID);
    for (int i = 0; i < course_index; ++i) {
        printf("course %d: %s\n", i, record.course[i]);
    }

    return 0;
}

int yywrap()
{
    return 1;
}