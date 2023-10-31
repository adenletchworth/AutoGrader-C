#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

int doesCompile(FILE* submission){
    //check if code compiles
    return 0;
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


int main(){
    const char* path = ".";

    DIR* directory = opendir(path);
    FILE *rubric_ptr = fopen("answer_code.c","r");
    FILE *graded_ptr = fopen("student_grades","w");

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

            int grade = gradeSubmission(100, rubric_ptr,submission_ptr);
            fprintf(graded_ptr,"%d\n",grade);
            
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
    fclose(graded_ptr);

    return 0;
}