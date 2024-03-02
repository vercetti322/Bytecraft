%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <ctype.h>
    #include <string.h>
    #include <stdbool.h>

    typedef struct Record
    {
        char studID[30];
        char course[30][35];
        int courses;
    }Record;

    Record record;
    int total_count = 0;
    int upper_count = 0;
    int buffer_index = 0;
    int type = 0;
    int course_index = 0;
    bool course_found = false;
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
    course_found = false;
}

<UPPER>[A-Z] {
    if (type == 0)
    {
        if (total_count < 30)
        {   
            record.studID[buffer_index++] = yytext[0];
            total_count++;
            upper_count++;
        }

        else
        {
            printf("\nwrong ID!\n");
            BEGIN(INITIAL);
        }
    }

    else
    {
        if (total_count < 35)
        {   
            record.course[course_index][buffer_index++] = yytext[0];
            total_count++;
            upper_count++;
        }

        else
        {
            printf("\nwrong ID!\n");
            BEGIN(INITIAL);
        }
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
    if (type == 0)
        record.studID[0] = '\0';
    else
        record.course[course_index][0] = '\0';
    printf("\nwrong ID!\n");
    BEGIN(INITIAL);
}

<DIGIT>[A-Z]|\n {
    if (type == 0)
    {
        record.studID[0] = '\0';
        printf("\nwrong ID!\n");
        BEGIN(INITIAL);
    }

    else
    {
        record.course[course_index][0] = '\0';
        printf("\nwrong ID!\n");
        while (input() != '<') 
        {
            if (input() == YY_NULL)
            {
                BEGIN(INITIAL);
                break;
            }
        }

        unput('<');
        BEGIN(GREATER_THAN);
    }
}

<DIGIT>[0-9] {
    if (type == 0)
    {
        if (total_count < 30)
        {   
            record.studID[buffer_index++] = yytext[0];
            total_count++;
            upper_count++;
        }

        else
        {
            printf("\nwrong ID!\n");
            BEGIN(INITIAL);
        }
    }

    else
    {
        if (total_count < 35)
        {   
            record.course[course_index][buffer_index++] = yytext[0];
            total_count++;
            upper_count++;
        }

        else
        {
            printf("\nwrong ID!\n");
            BEGIN(INITIAL);
        }
    }
}

<DIGIT>[ ]* {
    if (type == 0)
    {
        record.studID[buffer_index] = '\0';
        buffer_index = 0;
        BEGIN(GREATER_THAN);
    }
        
    else
    {
        record.course[course_index][buffer_index++] = ' ';
        BEGIN(CREDS);
    }
}

<GREATER_THAN>[<][ ]* {
    BEGIN(UPPER);
    type = 1;
    total_count = 0;
    upper_count = 0;
    buffer_index = 0;
}

<GREATER_THAN>.|\n {
    record.course[course_index][0] = '\0';
    printf("\nno course info found 1!\n");
    BEGIN(INITIAL);
}

<CREDS>[2-4][ ](A-?|B-?|C) {
    for (int i = 0; i < yyleng; ++i) {
        record.course[course_index][buffer_index++] = yytext[i];
    }

    course_found = true;
    BEGIN(LESSER_THAN);
}

<CREDS>. {
    printf("\nwrong course info found 2!\n");
    record.course[course_index][0] = '\0';
    while (input() != '<') 
    {
        if (input() == YY_NULL)
        {
            BEGIN(INITIAL);
            break;
        }
    }
    unput('<');
    BEGIN(GREATER_THAN);
}

<CREDS>\n {
    printf("\nwrong course info found 2!\n");
    record.studID[0] = '\0';
    record.course[course_index][0] = '\0';
    BEGIN(INITIAL);
}

<LESSER_THAN>[ ]*[>][ ]*[<] {
    printf("\ncompleted record\n");
    ++course_index;
    unput(yytext[yyleng - 1]);
    BEGIN(GREATER_THAN);
}

<LESSER_THAN>[ ]*[>]\n {
    printf("\ncompleted record\n");
    if (course_found)
        record.studID[0] = '\0';
    ++course_index;
    BEGIN(INITIAL);
}
%%

int main()
{
    yylex();

    printf("record ID: %s\n", record.studID);
    for (int i = 0; i < course_index; ++i) 
    {
        printf("course %d: %s\n", i, record.course[i]);
    }

    return 0;
}

int yywrap()
{
    return 1;
}