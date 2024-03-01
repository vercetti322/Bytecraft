%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <ctype.h>
    #include <string.h>
    typedef struct studID 
    {
        int total_count;
        int max_length;
        int upper_count;
        char recordBuffer[30]; // Adjust the size as needed
        int bufferIndex;
    }StudID;
    
    StudID studID = {0, 30, 0, {0}, 0};
%}
%x STUDID_UPPER
%x STUDID_DIGIT

%%
[^$]+ { /* Ignore input before $ with spaces */ }

[$][ ]* {
    BEGIN(STUDID_UPPER);
    studID.total_count = 0;
    studID.upper_count = 0;
    studID.bufferIndex = 0;
    memset(studID.recordBuffer, 0, sizeof(studID.recordBuffer));
}
<STUDID_UPPER>[A-Z] {
    if (studID.total_count < studID.max_length) {
        studID.recordBuffer[studID.bufferIndex++] = yytext[0];
        studID.total_count++;
        studID.upper_count++;
    } else {
        printf("\nStudent ID exceeds maximum length!\n");
        BEGIN(INITIAL);
    }
}
<STUDID_UPPER>[0-9] {
    if (studID.upper_count > 0)
    {
        BEGIN(STUDID_DIGIT);
        unput(yytext[0]);
    }

    else
    {
        printf("\nincorrect format\n");
        BEGIN(INITIAL);
    }
}
<STUDID_UPPER>.|\n {
    printf("\ninvalid string\n");
    BEGIN(INITIAL);
}
<STUDID_DIGIT>[A-Z] {
    printf("\nInvalid student ID format!\n");
    BEGIN(INITIAL);
}
<STUDID_DIGIT>[0-9] {
    if (studID.total_count < studID.max_length) {
        studID.recordBuffer[studID.bufferIndex++] = yytext[0];
        studID.total_count++;
    } else {
        printf("\nStudent ID exceeds maximum length!\n");
        BEGIN(INITIAL);
    }
}
<STUDID_DIGIT>[ ]|\n {
    studID.recordBuffer[studID.bufferIndex] = '\0'; 
    if (studID.total_count > 0) {
        printf("\nCompleted Record: %s\n", studID.recordBuffer);
    }
    exit(0);   
}
<STUDID_DIGIT>. {
    printf("\nInvalid student ID format!\n");
    BEGIN(INITIAL);
}
%%

int main() 
{
    yylex();
    return 0;
}

int yywrap()
{
    return 1;
}