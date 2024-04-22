%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <ctype.h>
    #include <string.h>
    #include <math.h>
    #define MAX_COURSES 50
    #define MAX_RECORDS 50

    int courseCount = 0;
    int wrongid = 0;
    char student_id[31];
    char student[31];
    double CGPA;
    int num_courses = 0;

    struct course 
    {
        char ID[31];
        char grade[3];
        int credit;
    };

    struct record
    {
        struct course courses[MAX_COURSES];
        int num_courses;
        char student_id[31];
    };


    // Define FILE pointers
    FILE *inputFile;
    FILE *outputFile;
    struct record studentRecords[MAX_RECORDS];
    int numRecords = 0;

    // Define function prototypes
    int yywrap(void);
    void yyerror(const char *s);

%}

%x STUD_ID
%x COURSE

%%
"//".*"$" {
    wrongid++;
}
[^$]+ { 

}

[$][ ]* {
    BEGIN(STUD_ID);
}

<STUD_ID>[A-Z]+[0-9]+ {
    if (yyleng > 30)
    {
        fprintf(outputFile, "wrong student id\n");
        wrongid++;
        BEGIN(INITIAL);
    }
    else
    {
        strncpy(student, yytext, yyleng);
        student[yyleng] = '\0';
        if (strcmp(student, student_id) == 0) {
        if (numRecords < MAX_RECORDS) {
            strncpy(studentRecords[numRecords].student_id, student, sizeof(studentRecords[numRecords].student_id));
            studentRecords[numRecords].num_courses = 0;
            numRecords++;
        } else {
            fprintf(stderr, "Too many records for student %s\n", student);
            exit(1);
        }
    }
        courseCount = 0;
        BEGIN(COURSE);
    }
}

<STUD_ID>[\/][\/] {
    student[0] = '\0'; 
    wrongid++;
    BEGIN(INITIAL);
}

<STUD_ID>.|\n {
    wrongid++;
    BEGIN(INITIAL);
}

<COURSE>[\/][\/] {
    if (courseCount == 0)
    {
        student[0] = '\0';
        wrongid++;
    }
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

    struct course c;
    strncpy(c.ID, courseID, sizeof(c.ID));
    strncpy(c.grade, grad, sizeof(c.grade));
    c.credit = cred;

    // Find the record with the matching student ID
    int recordIndex = -1;
    for (int i = 0; i < numRecords; i++) {
        if (strcmp(studentRecords[i].student_id, student) == 0) {
            recordIndex = i;
            break;
        }
    }

    /// If a matching record is found, add the course to it
    if (recordIndex != -1) {
        if (studentRecords[recordIndex].num_courses < MAX_COURSES) {
            studentRecords[recordIndex].courses[studentRecords[recordIndex].num_courses] = c;
            studentRecords[recordIndex].num_courses++;
        } else {
            fprintf(stderr, "Too many courses for student %s\n", student);
            exit(1);
        }
    }

    courseCount++;
    BEGIN(COURSE);
}

<COURSE>[$] {
    if (courseCount == 0)
    {
        student[0] = '\0';
    }
    if (student[0] == '\0') // Check if student ID is empty
    {
        wrongid++; // Increment the counter
    }

    unput('$');
    BEGIN(INITIAL);
}

<COURSE>[\n] {
    if (courseCount > 0) 
    {
        BEGIN(INITIAL);
        courseCount = 0;
    }

    else
    {
        student[0] = '\0';
        if (student[0] == '\0') // Check if student ID is empty
        {
            wrongid++; // Increment the counter
        }
        BEGIN(INITIAL);
    }

}
<COURSE>[.]* {
    // Ignore invalid input
    if (courseCount == 0)
    {
        student[0] = '\0';
        wrongid++;
    }
}
%%

int countComments(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1; // Return -1 on error
    }

    char line[1024];
    int totalComments = 0;

    while (fgets(line, sizeof(line), file)) {
        char *commentPtr = strstr(line, "//"); // Search for '//'
        while (commentPtr != NULL) {
            totalComments++; // Increment comment counter
            commentPtr = strstr(commentPtr + 2, "//"); // Move to next occurrence
        }
    }

    fclose(file);
    return totalComments;
}

int countRecords(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1; // Return -1 on error
    }

    char line[1024];
    int totalRecords = 0;

    while (fgets(line, sizeof(line), file)) {
        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] == '$') { // Check if character is $
                totalRecords++; // Increment record counter
            }
        }
    }

    fclose(file);
    return totalRecords;
}

void readStudentId(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening student file");
        exit(1);
    }

    // Read student ID
    if (fgets(student_id, sizeof(student_id), file) == NULL) {
        perror("Error reading student ID");
        exit(1);
    }

    // Remove newline character
    student_id[strcspn(student_id, "\n")] = '\0';

    fclose(file);
}

int courseExists(struct course courses[], int numCourses, char* courseID) {
    for (int i = 0; i < numCourses; i++) {
        if (strcmp(courses[i].ID, courseID) == 0) {
            return i;
        }
    }
    return -1;
}

void printStudentRecords(struct record studentRecords[]) {
    struct course courses[30];
    int numCourses = 0;

    for (int i = 0; i < 50; i++) {
        if (studentRecords[i].student_id[0] != '\0' && studentRecords[i].num_courses != 0)
        {
            for (int j = 0; j < studentRecords[i].num_courses; j++) 
            {
                // Check if the course already exists in the courses array
                int index = courseExists(courses, numCourses, studentRecords[i].courses[j].ID);
                if (index == -1) {
                    // If the course does not exist, add it to the courses array
                    courses[numCourses] = studentRecords[i].courses[j];
                    numCourses++;
                } else {
                    // If the course does exist, replace it with the current course
                    courses[index] = studentRecords[i].courses[j];
                }
            }

        }
    }

    double cgpa = 0.0;
    int sum = 0;
    int credsum = 0;
    for (int i = 0; i < numCourses; i++)
    {
        int x = 0;

        if (strcmp(courses[i].grade, "A") == 0)
            x = 10;

        else if (strcmp(courses[i].grade, "A-") == 0)
            x = 8;

        else if (strcmp(courses[i].grade, "B") == 0)
            x = 6;

        else if (strcmp(courses[i].grade, "B-") == 0)
            x = 4;

        else if (strcmp(courses[i].grade, "C") == 0)
            x = 2;

        sum += x * courses[i].credit;
        credsum += courses[i].credit;
    }

    double gpa = (double)sum / credsum;
    CGPA = gpa;
    num_courses = numCourses;
}

int main()
{
    // Open input file for reading
    inputFile = fopen("data.txt", "r");
    if (inputFile == NULL) 
    {
        perror("Error opening input file");
        return 1;
    }

    outputFile = fopen("output.txt", "w");
    if (inputFile == NULL) 
    {
        perror("Error opening input file");
        fclose(inputFile);
        return 1;
    }

    readStudentId("input.txt");

    // Redirect input and output
    yyin = inputFile;

    // Run the lexer
    yylex();
    printStudentRecords(studentRecords);

    if (CGPA > 0)
        fprintf(outputFile, "@%d@%d@%d@%d#", countRecords("data.txt") - wrongid, countComments("data.txt"), num_courses, (int)(round(100 * CGPA)));
    else
        fprintf(outputFile, "@%d@%d@0@0#", countRecords("data.txt") - wrongid, countComments("data.txt"));

    // Close files
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}

int yywrap()
{
    return 1;
}
