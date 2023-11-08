#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

#define CAPACITY 10

struct Student{
    char name[10];
    int grade;
    int doesCompile;
    char letterGrade[3];
};

struct Student* students;


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

void makeStudentGrades(int count){

    FILE *graded_ptr = fopen("student_grades","w");

    for (int i = 0; i < count+1;i++){
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

    int count = 0;
    students = (struct Student*)malloc(CAPACITY * sizeof(struct Student));

    int capacity = CAPACITY;

    struct dirent* file;

    while ((file = readdir(directory)) != NULL){
        if (strstr(file->d_name, "hw1.c")) {
            if(count >= capacity){
                capacity *= 2;
                students = (struct Student*)realloc(students, capacity * sizeof(struct Student));
            }
            char* submission_path = (char*)malloc(strlen(path) + strlen(file->d_name) + 2);
            strcpy(submission_path, path);
            strcat(submission_path, "/");
            strcat(submission_path, file->d_name);

            FILE *submission_ptr = fopen(submission_path,"r");
            
            int studentNumber;
            sscanf(file->d_name, "student%d-", &studentNumber);

            if (studentNumber >= capacity) {
                capacity = studentNumber + 1; 
                students = (struct Student*)realloc(students, capacity * sizeof(struct Student));
            }

            int doesCompile = compileCheck(submission_path);
            int grade = gradeSubmission(100, rubric_ptr,submission_ptr);
            sprintf(students[studentNumber].name, "student%d", studentNumber);
            strcpy(students[studentNumber].letterGrade,getGrade(grade));
            students[studentNumber].grade = grade;
            students[studentNumber].doesCompile = doesCompile;
            
            count++;

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

    makeStudentGrades(count);

    free(students);

    return 0;
}