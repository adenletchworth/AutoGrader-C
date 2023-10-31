#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

#define NUM_STUDENTS 10

struct Student{
    char name[10];
    int grade;
    int doesCompile;
    char letterGrade[2];
};

struct Student students[NUM_STUDENTS];


int compileCheck(char* file_path){
    char compile[100];
    snprintf(compile,sizeof(compile), "gcc %s >/dev/null 2>&1", file_path);

    return system(compile) == 0;
}


int gradeSubmission(int total_score, FILE* rubric, FILE* submission){
    char submission_line[100];
    char rubric_line[100];
    while (fgets(rubric_line, sizeof(rubric_line), rubric) != NULL) {
        
        if (fgets(submission_line, sizeof(submission_line), submission) == NULL || strcmp(submission_line, rubric_line) != 0) {
            total_score -= 1;
            continue;
        }

    }

    return total_score;
}

void makeStudentGrades(){

    FILE *graded_ptr = fopen("student_grades","w");

    for (int i = 0; i < NUM_STUDENTS;i++){
        if (students[i].name[0] != '\0'){
            
            if (students[i].doesCompile){
                fprintf(graded_ptr,"%s %d %s\n",students[i].name,students[i].grade,students[i].letterGrade);
            }
            else{
                fprintf(graded_ptr,"%s %s %s\n",students[i].name,"DNC", "F");
            }
                
        }
    }
    fclose(graded_ptr);
}

const char* getGrade(int grade){
    if(grade >= 94){
        return "A";
    }
    else if(grade >= 89){
        return "A-";
    }
    else if(grade >= 86){
        return "B+";
    }
    else if(grade >= 82){
        return "B";
    }
    else if(grade >= 78){
        return "B-";
    }
    else if(grade >= 75){
        return "C+";
    }
    else if(grade >= 71){
        return "C";
    }
    else if(grade >= 68){
        return "C-";
    }
    else if(grade >= 65){
        return "D+";
    }
    else if(grade >= 61){
        return "D";
    }
    else if(grade >= 58){
        return "D-";
    }
    else{
        return "F";
    }
}

int main(){
    const char* path = ".";

    DIR* directory = opendir(path);
    FILE *rubric_ptr = fopen("answer_code.c","r");

    if (!directory){
        printf("Error opening directory \n");
        return 1;
    }

    struct dirent* file;

    while ((file = readdir(directory)) != NULL){
        if (strstr(file->d_name, "hw1.c")) {

            char* submission_path = (char*)malloc(strlen(path) + strlen(file->d_name) + 2);
            strcpy(submission_path, path);
            strcat(submission_path, "/");
            strcat(submission_path, file->d_name);

            FILE *submission_ptr = fopen(submission_path,"r");
            
            int studentNumber;
            int doesCompile = compileCheck(submission_path);
            int grade = gradeSubmission(100, rubric_ptr,submission_ptr);
            char gradeCharacter[3]; 
            strcpy(gradeCharacter, getGrade(grade));

            sscanf(file->d_name, "student%d-", &studentNumber);

            sprintf(students[studentNumber].name, "student%d", studentNumber);
            strcpy(students[studentNumber].letterGrade,gradeCharacter);
            students[studentNumber].grade = grade;
            students[studentNumber].doesCompile = doesCompile;
            

            // Clean up
            fseek(rubric_ptr, 0, SEEK_SET);
            fclose(submission_ptr);
            free(submission_path);
        }
    }

    if (closedir(directory) == -1){
        printf("Error closing directory \n");
        return 1;
    }

    fclose(rubric_ptr);

    makeStudentGrades();

    return 0;
}